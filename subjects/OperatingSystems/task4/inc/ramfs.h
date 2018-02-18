#ifndef __RAMFS_H__
#define __RAMFS_H__

#include <stddef.h>
#include <stdint.h>

#include <list.h>
#include <spinlock.h>

#define MAX_FILE_NAME 256
#define FS_ERROR -1
#define MAX_NUM_OF_FILEDESC 256

struct inode
{
    void* buf_base;
    size_t size;
    size_t capacity;
    char name[MAX_FILE_NAME + 1];

    struct list_head neighbours_in_same_dir;
    struct inode* first_child;

    uint8_t is_dir;

    struct spinlock lock;
};


typedef struct DIR
{
    struct inode* dir_node;
    struct list_head* cur_file;
} DIR;


void initialize_ram_filesystem(void);


DIR* opendir(const char* dir_name);
void rewinddir(DIR* dir);
void closedir(DIR* dir);
// Return pointer to inode instead of 
// pointer to `struct dirent*`, as in UNIX API.
struct inode* readdir(DIR* dir);


int mkdir(const char* dir_name);

int open(const char* file_name);
void close(int fd);

void write(int fd, size_t offset, const void* buf, size_t nbyte);
void read(int fd, size_t offset, void* buf, size_t nbyte);

void dump_filesystem(void);

#endif /* __RAMFS_H__ */
