#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "disk.h"
#include "goatfs.h"


// ---- DO NOT MODIFY -----
// test cases: helper functions

void _do_debug(){
    debug();
}

void _do_format(){

    if (format()){
        printf("disk formatted.\n");
    } else {
        printf("format failed!\n");
    }

}

void _do_mount(){

    if (mount() == SUCCESS_GOOD_MOUNT) {
    	printf("disk mounted.\n");
    } else {
    	printf("mount failed!\n");
    }

}

void _do_create(){

    ssize_t inumber = create();
    if (inumber >= 0) {
        printf("created inode %ld.\n", inumber);
    } else {
        printf("create failed!\n");
    }

}

void _do_remove(ssize_t inumber){

    if (wremove(inumber)) {
    	printf("removed inode %ld.\n", inumber);
    } else {
    	printf("remove failed!\n");
    }

}


void _do_stat(ssize_t inumber){

    ssize_t bytes   = stat(inumber);
    if (bytes >= 0) {
    	printf("inode %ld has size %ld bytes.\n", inumber, bytes);
    } else {
    	printf("stat failed!\n");
    }

}


// both the path and inumber will be passed from the test cases...
bool _do_copyout(const char *path, size_t inumber){

     FILE *stream = fopen(path, "w");

     if (stream == NULL){
         return false;
     }

     // use the macro BUFSIZ to make I/O efficient
     //gcc will fill the remaining entries with zeros
     char buffer[4*BUFSIZ] = {0};

    size_t offset = 0;

    while (true){
        ssize_t result = wfsread(inumber, buffer, sizeof(buffer), offset);
        if (result <= 0){ //TODO: should check for errors?
            break;
        }

        fwrite(buffer, 1, result, stream);
        offset += result;
    }

    printf("%lu bytes copied\n", offset);
    fclose(stream);

    return true;
}


bool _do_copyin(const char *path, size_t inumber){

    FILE *stream = fopen(path, "r");

    if (stream == NULL){
        return false;
    }

    char buffer[4*BUFSIZ] = {0};
    size_t offset = 0;


    while (true){
        ssize_t result = fread(buffer, 1, sizeof(buffer), stream);
        if (result <= 0){
            break;
        }

        ssize_t actual = wfswrite(inumber, buffer, result, offset);
        if (actual < 0){
            break;
        }

        offset += actual;
        if (actual != result){
            break;
        }
    }

    printf("%lu bytes copied in to disk...\n", offset);
    fclose(stream);
    return true;

}
// end of test cases: helper function



// test cases:
void do_debug(){
    debug();
}

void do_format(){
    _do_format();
    _do_debug();
}


// test cases for mount
void do_mount(){
    _do_mount();
}


void do_mount_mount(){
    _do_mount();
    // try to mount it the second time
    _do_mount();
}

void do_mount_format(){
    _do_mount();
    _do_format();
}
// end for mount


void do_create(){
    _do_debug();
    _do_mount();
    // create 128 times
    for (int i = 0; i < 128; i++){
        _do_create();
    }
    _do_debug();
}


void do_remove_test1(){
    _do_debug();
    _do_mount();

    // create 3 times
    for (int i = 0; i < 3; i++){
        _do_create();
    }

    _do_remove(0);
    _do_debug();

    _do_create();

    _do_remove(0);
    _do_remove(0);
    _do_remove(1);
    _do_remove(3);

    _do_debug();
}


void do_stat_test1(){

    _do_mount();
    _do_stat(1);
    _do_stat(2);
    _do_stat(3);

}

void do_stat_test2(){

    do_stat_test1();
    _do_stat(9);

}


void do_copyout_test1(const char* path){
    _do_mount();
    char* filename = "/1.txt";

    char fullfilepath[strlen(filename) + strlen(path) + 1];
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);

    _do_copyout(fullfilepath, 1);

}

void do_copyout_test2(const char* path){
    char* filename;

    _do_mount();

    filename = "/2.txt";
    char fullfilepath[strlen(filename) + strlen(path) + 1];
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 2);

    filename = "/3.txt";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 3);
}

void do_copyout_test3(const char* path){
    char* filename;

    _do_mount();

    filename = "/1.txt";
    char fullfilepath[strlen(filename) + strlen(path) + 1];
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 1);

    filename = "/2.txt";
    char fullfilepath_one[strlen(filename) + strlen(path) + 1];
    snprintf(fullfilepath_one, sizeof(fullfilepath_one), "%s%s", path, filename);
    _do_copyout(fullfilepath_one, 2);

    filename = "/9.txt";
    char fullfilepath_two[strlen(filename) + strlen(path) + 1];
    snprintf(fullfilepath_two, sizeof(fullfilepath_two), "%s%s", path, filename);
    _do_copyout(fullfilepath_two, 9);
}


// test image.5
void do_copyin_test1(const char* path){

    _do_debug();
    _do_mount();

    char* filename = "/1.txt";
    // +2 to account for longer file name
    char fullfilepath[strlen(filename) + strlen(path) + 2];
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 1);

    _do_create();
    _do_debug();

    _do_copyin(fullfilepath, 0);
    _do_debug();

    filename = "/1.copy";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 0);
}

// test image.20
void do_copyin_test2(const char* path){

    _do_debug();
    _do_mount();

    // copyout 2 $SCRATCH/2.txt
    char* filename = "/2.txt";
    // +2 to account for longer file name
    char fullfilepath[strlen(filename) + strlen(path) + 2];
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 2);

    // copyout 3 $SCRATCH/3.txt
    filename = "/3.txt";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 3);

    _do_create();

    filename = "/3.txt";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyin(fullfilepath, 0);

    _do_create();

    filename = "/2.txt";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyin(fullfilepath, 1);

    filename = "/3.copy";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);

    _do_copyout(fullfilepath, 0);

    filename = "/2.copy";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);

    _do_copyout(fullfilepath, 1);

    _do_debug();
}


void do_copyin_test3(const char* path){

    char* filename;

    _do_debug();
    _do_mount();

    filename = "/1.txt";
    // +2 to account for longer file name
    char fullfilepath[strlen(filename) + strlen(path) + 2];
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 1);


    _do_create();  // return 0
    filename = "/1.txt";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyin(fullfilepath, 0);

    _do_debug();

    filename = "/1.copy";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);

    _do_copyout(fullfilepath, 0);

}

void do_copyin_test4(const char* path){

    char* filename;

    _do_debug();
    _do_mount();

    // copyout 2 $SCRATCH/2.txt
    filename = "/2.txt";
    // +2 to account for longer file name
    char fullfilepath[strlen(filename) + strlen(path) + 2];
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 2);

    _do_create();  // return 0

    _do_create(); // return 3
    filename = "/2.txt";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyin(fullfilepath, 3);

    _do_debug();

    filename = "/2.copy";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 3);

}


void do_copyin_test5(const char* path){

    char* filename;

    _do_debug();
    _do_mount();

    filename = "/1.txt";
    // +2 to account for longer file name
    char fullfilepath[strlen(filename) + strlen(path) + 2];
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 1);

    // copyout 2 $SCRATCH/2.txt
    filename = "/2.txt";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 2);

    filename = "/9.txt";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 9);

    _do_create();  // return 0
    filename = "/1.txt";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyin(fullfilepath, 0);

    _do_create(); // return 3
    filename = "/2.txt";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyin(fullfilepath, 3);

    _do_create(); // return 4
    filename = "/9.txt";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyin(fullfilepath, 4);

    _do_debug();

    filename = "/1.copy";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 0);

    filename = "/2.copy";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 3);

    filename = "/9.copy";
    snprintf(fullfilepath, sizeof(fullfilepath), "%s%s", path, filename);
    _do_copyout(fullfilepath, 4);
}

// end testing functions ----



int main(int argc, char *argv[]){

    if (argc < 4){
        printf("[usage]: ./fs disk blocks test [filepath]\n");
        exit(EXIT_FAILURE);
    }

    char* diskpath = argv[1];
    int blocks = atoi(argv[2]);
    char* testcase = argv[3];

    _disk = wopen(diskpath, blocks);

    // matching the test command passed from the bash script
    // note: some test cases take one more argument
    if (strcmp(testcase, "debug") == 0){
        do_debug();
    } else if (strcmp(testcase, "format") == 0){
        do_format();
    } else if (strcmp(testcase, "mount") == 0 || strcmp(testcase, "bad-mount") == 0){
        do_mount();
    } else if (strcmp(testcase, "mount-mount") == 0){
        do_mount_mount();
    } else if (strcmp(testcase, "mount-format") == 0){
        do_mount_format();
    } else if (strcmp(testcase, "create") == 0){
        do_create();
    } else if (strcmp(testcase, "remove-test-1") == 0){
        do_remove_test1();
    } else if (strcmp(testcase, "stat-test-image.5") == 0 || strcmp(testcase, "stat-test-image.20") == 0){
        do_stat_test1();
    } else if (strcmp(testcase, "stat-test-image.200") == 0){
        do_stat_test2();
    } else if (strcmp(testcase, "copyout-test-1") == 0){
        char* filepath = argv[4];
        do_copyout_test1(filepath);
    }else if (strcmp(testcase, "copyout-test-2") == 0){
        char* filepath = argv[4];
        do_copyout_test2(filepath);
    }else if (strcmp(testcase, "copyout-test-3") == 0){
        char* filepath = argv[4];
        do_copyout_test3(filepath);
    } else if (strcmp(testcase, "copyin-test-1") == 0){
        char* filepath = argv[4];
        do_copyin_test1(filepath);
    } else if (strcmp(testcase, "copyin-test-2") == 0){
        char* filepath = argv[4];
        do_copyin_test2(filepath);
    } else if (strcmp(testcase, "copyin-test-3") == 0){
        char* filepath = argv[4];
        do_copyin_test3(filepath);
    } else if (strcmp(testcase, "copyin-test-4") == 0){
        char* filepath = argv[4];
        do_copyin_test4(filepath);
    }  else if (strcmp(testcase, "copyin-test-5") == 0){
        char* filepath = argv[4];
        do_copyin_test5(filepath);
    }

    // destroy the disk before terminating
    wdestroy(_disk);

    return 0;
}