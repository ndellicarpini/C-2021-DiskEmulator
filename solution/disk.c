#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h> //errno
#include "disk.h"

DISK disk;

// explict initialize the disk
// even though by default the struct will be initialized to the same values
void winit(){
    disk.FileDescriptor = 0;
    disk.Blocks = 0;
    disk.Reads = 0;
    disk.Writes = 0;
    disk.Mounts = 0;
}


void wdestroy(){
    if (disk.FileDescriptor > 0){
        printf("%lu disk block reads\n", disk.Reads);
        printf("%lu disk block writes\n", disk.Writes);
        // check close errors
        close(disk.FileDescriptor);
        disk.FileDescriptor = 0;
    }

}


DISK* wopen(const char *path, size_t nblocks){
    disk.FileDescriptor = open(path, O_RDWR|O_CREAT, 0600);

    if (disk.FileDescriptor == -1){
         printf("...error: failed to open %s!", path);
         exit(EXIT_FAILURE);
    }

    disk.Blocks = nblocks;
    disk.Reads = 0;
    disk.Writes = 0;

    return &disk;
}


void sanity_check(int blocknum, char *data) {

    if (blocknum < 0) {
        printf("blocknum (%d) is negative!", blocknum);
        exit(EXIT_FAILURE);
    }

    // printf(">>debug: %d\n", (int)disk.Blocks);
    if (blocknum >= (int)disk.Blocks){
        printf("blocknum (%d) is too big!", blocknum);
        exit(EXIT_FAILURE);
    }

    if (data == NULL){
        printf("null data pointer!");
        exit(EXIT_FAILURE);
    }

}


void wread(int blocknum, char *data) {
    sanity_check(blocknum, data);

    if(lseek(disk.FileDescriptor, blocknum*BLOCK_SIZE, SEEK_SET) <0){
        // save errno first
        int curerrno = errno;
        printf("Unable to lseek %d: %d\n", blocknum, curerrno);
        exit(curerrno);
    }

    // check if read is success
    // returning less bytes than what is asking for is not an error...
    if(read(disk.FileDescriptor, data, BLOCK_SIZE) == -1){
        int curerrno = errno;
        printf("...error: Unable to read block [%d] with error no [%d]\n", blocknum, curerrno);
        exit(curerrno);
    }

    disk.Reads++;

}


void wwrite(int blocknum, char *data) {
    sanity_check(blocknum, data);

    if(lseek(disk.FileDescriptor, blocknum*BLOCK_SIZE, SEEK_SET) <0){
        // save errno first
        int curerrno = errno;
        printf("Unable to lseek %d: %d", blocknum, curerrno);
        exit(curerrno);
    }

    if(write(disk.FileDescriptor, data, BLOCK_SIZE) != BLOCK_SIZE){
        int curerrno = errno;
        printf("Unable to write %d: %d", blocknum, curerrno);
        exit(curerrno);
    }

    disk.Writes++;

}
