#!/bin/bash

FSBIN="./solution/test_goatfs"

# begin: expected outputs
image-5-output() {
    cat <<EOF
disk formatted.
SuperBlock:
    magic number is valid
    5 blocks
    1 inode blocks
    128 inodes
2 disk block reads
5 disk block writes
EOF
}

image-20-output() {
    cat <<EOF
disk formatted.
SuperBlock:
    magic number is valid
    20 blocks
    2 inode blocks
    256 inodes
3 disk block reads
20 disk block writes
EOF
}

image-200-output() {
    cat <<EOF
disk formatted.
SuperBlock:
    magic number is valid
    200 blocks
    20 inode blocks
    2560 inodes
21 disk block reads
200 disk block writes
EOF
}
# end

test-format() {
    DISK=$1
    BLOCKS=$2
    OUTPUT=$3

    cp $DISK $DISK.formatted
    echo -n "Testing format on $DISK.formatted ... "
    if diff <($FSBIN $DISK.formatted $BLOCKS format 2> /dev/null) <($OUTPUT) > test.log; then
        echo -e "\e[32m passed\e[0m"
    else
        echo -e "\e[31m failed\e[0m"
    	cat test.log
    fi
    rm -f $DISK.formatted test.log
}

test-format data/image.5   5   image-5-output
test-format data/image.20  20  image-20-output
test-format data/image.200 200 image-200-output
