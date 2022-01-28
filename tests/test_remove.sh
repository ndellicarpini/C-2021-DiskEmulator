#!/bin/bash

FSBIN="./solution/test_goatfs"

SCRATCH=$(mktemp -d)

test-output() {
    cat <<EOF
SuperBlock:
    magic number is valid
    5 blocks
    1 inode blocks
    128 inodes
Inode 1:
    size: 965 bytes
    direct blocks: 2
disk mounted.
created inode 0.
created inode 2.
created inode 3.
removed inode 0.
SuperBlock:
    magic number is valid
    5 blocks
    1 inode blocks
    128 inodes
Inode 1:
    size: 965 bytes
    direct blocks: 2
Inode 2:
    size: 0 bytes
    direct blocks:
Inode 3:
    size: 0 bytes
    direct blocks:
created inode 0.
removed inode 0.
remove failed!
removed inode 1.
removed inode 3.
SuperBlock:
    magic number is valid
    5 blocks
    1 inode blocks
    128 inodes
Inode 2:
    size: 0 bytes
    direct blocks:
25 disk block reads
8 disk block writes
EOF
}

cp data/image.5 $SCRATCH/image.5
trap "rm -fr $SCRATCH" INT QUIT TERM EXIT

echo -n "Testing remove in $SCRATCH/image.5 ... "
if diff -u <($FSBIN $SCRATCH/image.5 5 remove-test-1 2> /dev/null) <(test-output) > $SCRATCH/test.log; then
    echo -e "\e[32m passed\e[0m"
else
    echo -e "\e[31m failed\e[0m"
    cat $SCRATCH/test.log
fi
