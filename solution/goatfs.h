#ifndef __fs_h__
#define __fs_h__

# include "disk.h"
# include <stdint.h> // uint32_t

// internal error codes
#define ERR_BAD_MAGIC_NUMBER (-1)
#define ERR_NOT_ENOUGH_BLOCKS (-2)
#define ERR_NOT_ENOUGH_INODES (-3)
#define ERR_CREATE_INODE (-4)

#define SUCCESS_GOOD_MOUNT (0)


// fs meta data
#define MAGIC_NUMBER (0xf0f03410)
#define INODES_PER_BLOCK (128)
#define POINTERS_PER_INODE (5) // direct pointers
#define POINTERS_PER_BLOCK (1024)


// functions
extern void debug();
extern bool format();
extern int mount();
extern ssize_t create();
extern bool wremove(size_t inumber);
extern ssize_t stat(size_t inumber);
ssize_t wfsread(size_t inumber, char *data, size_t length, size_t offset);
ssize_t wfswrite(size_t inumber, char *data, size_t length, size_t offset);
//

struct _SuperBlock {		// Superblock structure
    unsigned int MagicNumber;	// File system magic number
    unsigned int Blocks;	// Number of blocks in file system
    unsigned int InodeBlocks;	// Number of blocks reserved for inodes
    unsigned int Inodes;	// Number of inodes in file system
};

typedef struct _SuperBlock SuperBlock;

struct _Inode {
    unsigned int Valid;		// Whether or not inode is valid
    unsigned int Size;		// Size of file
    unsigned int Direct[POINTERS_PER_INODE]; // Direct pointers
    unsigned int Indirect;	// Indirect pointer
};

typedef struct _Inode Inode;


union _Block {
    SuperBlock  Super;			    // Superblock
    Inode	    Inodes[INODES_PER_BLOCK];	    // Inode block
    unsigned int    Pointers[POINTERS_PER_BLOCK];   // Pointer block
    char	    Data[BLOCK_SIZE];	    // Data block
};

typedef union _Block Block;

DISK* _disk;    // disk handler

// ----- ADDED CODE -----
int *mounted_bitmap;

// ----- END -----

#endif