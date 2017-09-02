#include <ramfs.h>
#include <alloc.h>
#include <string.h>

#include <print.h>
#include <threads.h>
#include <debug.h>

static struct inode* root_inode;

typedef struct file_descriptor
{
    struct inode* node;
    uint8_t is_free;
} file_descriptor;


static file_descriptor file_descriptors[MAX_NUM_OF_FILEDESC];
static struct spinlock filedesc_lock;
static struct spinlock make_dir_or_file_lock;


static DIR* opendir_from_inode(struct inode*);
static void test_ram_filesystem(void);
static void _dump_filesystem(struct inode* root, int shift);


static void add_child_to_inode(struct inode* parent, struct inode* child)
{
    spin_lock(&(parent->lock));
    if (parent->first_child == NULL)
    {
        parent->first_child = child;
    }
    else
    {
        list_add(&(child->neighbours_in_same_dir), 
                 &(parent->first_child->neighbours_in_same_dir));
    }
    spin_unlock(&(parent->lock));
}


static struct inode* create_inode(const char* name)
{
    struct inode* ret = mem_alloc(sizeof(struct inode));

    ret->buf_base = NULL;
    ret->size = 0;
    ret->capacity = 0;
    memcpy(ret->name, name, strlen(name));

    ret->is_dir = 0;
    spin_setup(&(ret->lock));

    list_init(&(ret->neighbours_in_same_dir));
    ret->first_child = NULL;

    return ret;
}


static struct inode* create_directory_inode(const char* name)
{
    struct inode* ret = create_inode(name);
    ret->is_dir = 1;
    return ret;
}


static struct inode* find_child_by_name(struct inode* dir_node, const char* name_start,
                                        const char* name_end)
{
    DIR* cur_dir = opendir_from_inode(dir_node);
    struct inode* cur_node;
    struct inode* result_node = NULL;

    while ((cur_node = readdir(cur_dir)) != NULL)
    {
        int len_cur_name = strlen(cur_node->name);
        if (len_cur_name != name_end - name_start)
        {
            continue;
        }

        if (!strncmp(name_start, cur_node->name, len_cur_name))
        {
            result_node = cur_node;
        }
    }

    closedir(cur_dir);

    return result_node;
}


static struct inode* find_dir_inode(const char* name_start, const char* name_end, int need_construct)
{
    // Example input: "/usr/bin/env".
    struct inode* cur_node = root_inode;
    struct inode* next_node;

    // Invariant: `name_start` points to something right after slash.
    ++name_start;
    while (name_start < name_end)
    {
        const char* next_slash = name_start;
        while (next_slash < name_end && *next_slash != '/')
        {
            ++next_slash;
        }

        next_node = find_child_by_name(cur_node, name_start, next_slash);
        if (next_node == NULL)
        {
            if (need_construct)
            {
                char buf[MAX_FILE_NAME + 1];
                memcpy(buf, name_start, next_slash - name_start);
                buf[next_slash - name_start] = '\0';
                struct inode* child = create_directory_inode(buf);
                add_child_to_inode(cur_node, child);
                next_node = child;
            }
            else
            {
                return NULL;
            }
        }
        cur_node = next_node;

        name_start = next_slash + 1;
    }

    return cur_node;
}


static const char* pnt_to_last_slash(const char* name)
{
    int len = strlen(name);
    const char* end = name + len - 1;
    while (*end != '/')
    {
        --end;
    }
    return end;
}


static struct inode* find_parent_dir(const char* name, int need_construct)
{
    return find_dir_inode(name, pnt_to_last_slash(name), need_construct);
}


void initialize_ram_filesystem(void)
{
    // This is the "/" directory.
    root_inode = create_directory_inode("");
    spin_setup(&filedesc_lock);
    spin_setup(&make_dir_or_file_lock);

    for (int i = 0; i < MAX_NUM_OF_FILEDESC; ++i)
    {
        file_descriptors[i].is_free = 1;
    }

    if (0)
    {
        test_ram_filesystem();
    }
}


static DIR* opendir_from_inode(struct inode* node)
{
    DIR* dir = mem_alloc(sizeof(DIR));
    dir->dir_node = node;
    // Don't allow adding subdirectories while someone is reading.
    spin_lock(&(node->lock));
    rewinddir(dir);
    return dir;
}


DIR* opendir(const char* dir_name)
{
    struct inode* dir_node = find_dir_inode(dir_name, dir_name + strlen(dir_name), 0);
    if (dir_node == NULL)
    {
        return NULL;
    }
    return opendir_from_inode(dir_node);
}


void rewinddir(DIR* dir)
{
    struct inode* child = dir->dir_node->first_child;
    if (child)
    {
        dir->cur_file = &(child->neighbours_in_same_dir);
    }
    else 
    {
        dir->cur_file = NULL;
    }
}


void closedir(DIR* dir)
{
    spin_unlock(&(dir->dir_node->lock));
    mem_free(dir);
}


struct inode* readdir(DIR* dir)
{
    if (dir->cur_file == NULL)
    {
        return NULL;
    }
    struct inode* dir_node = dir->dir_node;
    struct inode* ret = LIST_ENTRY(dir->cur_file, struct inode, neighbours_in_same_dir);

    dir->cur_file = dir->cur_file->next;
    if (dir->cur_file == &(dir_node->first_child->neighbours_in_same_dir))
    {
        dir->cur_file = NULL;
    }

    return ret;
}


int open(const char* file_name)
{
    struct inode* parent = find_parent_dir(file_name, 0);
    if (parent == NULL)
    {
        return FS_ERROR;
    }

    spin_lock(&make_dir_or_file_lock);
    const char* after_last_slash = pnt_to_last_slash(file_name) + 1;
    struct inode* child = find_child_by_name(parent, after_last_slash,
           file_name + strlen(file_name)); 

    if (child == NULL)
    {
        // Create file.
        child = create_inode(after_last_slash);
        add_child_to_inode(parent, child);
    }
    spin_unlock(&make_dir_or_file_lock);

    spin_lock(&filedesc_lock);
    int num_free_desc = FS_ERROR;
    for (int i = 0; i < MAX_NUM_OF_FILEDESC; i++)
    {
        if (file_descriptors[i].is_free)
        {
            file_descriptors[i].is_free = 0;
            file_descriptors[i].node = child;
            num_free_desc = i;
            break;
        }
    }
    spin_unlock(&filedesc_lock);

    return num_free_desc;
}


void close(int fd)
{
    if (fd != FS_ERROR)
    {
        file_descriptors[fd].is_free = 1;
    }
}


int mkdir(const char* dir_name)
{
    spin_lock(&make_dir_or_file_lock);
    struct inode* parent = find_parent_dir(dir_name, 1);
    if (parent == NULL)
    {
        spin_unlock(&make_dir_or_file_lock);
        return FS_ERROR;
    }

    const char* after_last_slash = pnt_to_last_slash(dir_name) + 1;
    struct inode* child = find_child_by_name(parent, after_last_slash,
           dir_name + strlen(dir_name)); 

    if (child == NULL)
    {
        child = create_directory_inode(after_last_slash);
        add_child_to_inode(parent, child);
    }

    spin_unlock(&make_dir_or_file_lock);

    return 0;
}


void write(int fd, size_t offset, const void* buf, size_t nbyte)
{
    struct inode* node = file_descriptors[fd].node;
    spin_lock(&(node->lock));
    if (offset + nbyte > node->size)
    {
        node->size = offset + nbyte;
        while (node->size > node->capacity)
        {
            size_t new_capacity = node->capacity ? node->capacity * 2 : 3;
            node->buf_base = mem_realloc(node->buf_base, new_capacity);
            node->capacity = new_capacity;
        }
    }
    memcpy((void*)((uintptr_t)node->buf_base + offset), buf, nbyte);
    spin_unlock(&(node->lock));
}


void read(int fd, size_t offset, void* buf, size_t nbyte)
{
    struct inode* node = file_descriptors[fd].node;
    spin_lock(&(node->lock));
    size_t end_pos = offset + nbyte;
    if (end_pos > node->size)
    {
        if (end_pos - node->size > nbyte)
        {
            nbyte = 0;
        }
        else
        {
            nbyte -= end_pos - node->size;
        }
    }
    memcpy(buf, (void*)((uintptr_t)node->buf_base + offset), nbyte);
    spin_unlock(&(node->lock));
}


static void simple_test_ram_filesystem(void* data)
{
    (void) data;
    BUG_ON(mkdir("/root"));
    BUG_ON(mkdir("/root/fs"));
    BUG_ON(mkdir("/tpc"));
    BUG_ON(mkdir("/root/fs/myfs"));

    BUG_ON(mkdir("/root/tpc/my/folder") == FS_ERROR);
    BUG_ON(open("/root/myfile.txt") == FS_ERROR);
    BUG_ON(open("/root/nonexisting/file.txt") != FS_ERROR);
    BUG_ON(open("/root/my_second_file.txt") == FS_ERROR);
    BUG_ON(open("/root/my_third_file.png") == FS_ERROR);

    int fd = open("/root/test_file.txt");
    
    int thread_id = (int)(uintptr_t)data;
    char buf[222];
    snprintf(buf, 100, "%d\n", thread_id);

    write(fd, thread_id * 4, buf, 1);

    close(fd);
}

static void test_ram_filesystem(void)
{
    struct thread* threads[10];
    for (int i = 0; i < 10; i++)
    {
        threads[i] = thread_create(&simple_test_ram_filesystem, (void *)(uintptr_t)i);
    }

    for (int i = 0; i < 10; i++)
    {
        thread_activate(threads[i]);
    }

    for (int i = 0; i < 10; i++)
    {
        thread_join(threads[i]);
    }

    for (int i = 0; i < 10; i++)
    {
        thread_destroy(threads[i]);
    }

    _dump_filesystem(root_inode, 0);

    char buf[222];
    int fd = open("/root/test_file.txt");
    printf("File size is %d\n", file_descriptors[fd].node->size);
    printf("File capacity is %d\n", file_descriptors[fd].node->capacity);
    read(fd, 0, buf, 40);
    read(fd, 30, buf, 15);
    read(fd, 500, buf, 15);
    for (int i = 0; i < 40; i++)
        printf("%d", buf[i]);
    printf("\n");
    close(fd);
}


static void _dump_filesystem(struct inode* root, int shift)
{
    for (int i = 0; i < shift; i++)
        printf(" ");
    printf("%s", root->name);
    if (root->is_dir)
        printf("/");
    else
    {
        printf("\t\t(size = %lld)", root->size);
    }

    printf("\n");
    if (root->is_dir == 0)
    {
        return;
    }
    DIR* d = opendir_from_inode(root);
    struct inode* child;
    while ((child = readdir(d)) != NULL)
    {
        _dump_filesystem(child, shift + 2);
    }
}


void dump_filesystem(void)
{
    _dump_filesystem(root_inode, 0);
}
