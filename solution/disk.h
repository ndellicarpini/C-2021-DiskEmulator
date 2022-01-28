#ifndef __disk_h__
#define __disk_h__

#define BLOCK_SIZE (4096)

struct _wdisk {
    int	    FileDescriptor;
    size_t  Blocks;	    // Number of blocks in disk image
    size_t  Reads;	    // Number of reads performed
    size_t  Writes;	    // Number of writes performed
    size_t  Mounts;	    // Number of mounts
};

typedef struct _wdisk DISK;


extern void winit();
extern void wdestroy();

extern DISK* wopen(const char *path, size_t nblocks);

extern void wread(int blocknum, char *data);
extern void wwrite(int blocknum, char *data);

#endif
