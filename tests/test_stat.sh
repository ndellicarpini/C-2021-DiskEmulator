#!/bin/bash

FSBIN="./solution/test_goatfs"

# begin: expected output
image-5-output() {
    cat <<EOF
disk mounted.
inode 1 has size 965 bytes.
stat failed!
stat failed!
5 disk block reads
0 disk block writes
EOF
}


image-20-output() {
    cat <<EOF
disk mounted.
stat failed!
inode 2 has size 27160 bytes.
inode 3 has size 9546 bytes.
7 disk block reads
0 disk block writes
EOF
}

image-200-output() {
    cat <<EOF
disk mounted.
inode 1 has size 1523 bytes.
inode 2 has size 105421 bytes.
stat failed!
inode 9 has size 409305 bytes.
27 disk block reads
0 disk block writes
EOF
}
# end

test-stat() {
    BLOCKS=$1

    echo -n "Testing stat on data/image.$BLOCKS ... "
    if diff -u <($FSBIN data/image.$BLOCKS $BLOCKS stat-test-image.$BLOCKS 2> /dev/null) <(image-$BLOCKS-output) > test.log; then
        echo -e "\e[32m passed\e[0m"
    else
        echo -e "\e[31m failed\e[0m"
    	cat test.log
    fi
    rm -f test.log
}

test-stat 5
test-stat 20
test-stat 200
