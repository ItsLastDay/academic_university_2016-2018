# Operating Systems

I implemented several parts of a simple Linux kernel (based on QEMU):
- wrote my own [printf](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/OperatingSystems/task1/src/printf.c)
- [initialized](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/OperatingSystems/task1/src/interrupt.c) interrupt controllers and [serial port](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/OperatingSystems/task1/src/serial_port.c)
- wrote memory management subsystem:
  - [initialized](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/OperatingSystems/task2/src/memory_map.c) virtual memory mapping
  - implemented [buddy allocator](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/OperatingSystems/task2/src/buddy_alloc.c)
  - implemented [slab allocator](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/OperatingSystems/task2/src/slab_alloc.c)
  - implemented [malloc](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/OperatingSystems/task2/src/mymalloc.c)
- wrote utility data structure: [hashtable](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/OperatingSystems/task2/src/hashtable.c)
- wrote simple filesystem:
  - [parsed](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/OperatingSystems/task4/src/initramfs.c) initramfs image
  - [implemented](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/OperatingSystems/task4/src/ramfs.c) 
    file operations(mkdir, write, read, open, close, readdir, closedir, ...)
