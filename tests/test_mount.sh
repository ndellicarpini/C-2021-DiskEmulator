#!/bin/bash

FSBIN="./solution/test_goatfs"

# begin: expected output
mount-output() {
    cat <<EOF
disk mounted.
2 disk block reads
0 disk block writes
EOF
}

mount-mount-output() {
    cat <<EOF
disk mounted.
mount failed!
2 disk block reads
0 disk block writes
EOF
}


mount-format-output() {
    cat <<EOF
disk mounted.
format failed!
2 disk block reads
0 disk block writes
EOF
}
# end

test-mount () {
    TEST=$1
    DISK=$2

    echo -n "Testing $TEST on $DISK ... "
    if diff -u <($FSBIN $DISK 5 $TEST 2> /dev/null) <($TEST-output) > test.log; then
        echo -e "\e[32m passed\e[0m"
    else
        echo -e "\e[31m failed\e[0m"
	cat test.log
    fi
    rm -f test.log
}

# test mount on image.5
test-mount mount data/image.5
test-mount mount-mount data/image.5
test-mount mount-format data/image.5

SCRATCH=$(mktemp -d)
# deleting after the test
trap "rm -fr $SCRATCH" INT QUIT TERM EXIT


# expected output for bad mount!
bad-mount-output() {
    cat <<EOF
mount failed!
1 disk block reads
0 disk block writes
EOF
}

# test case: bad magic number
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x10 0x34 0xf1 0xf0) >  $SCRATCH/image.5
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x05 0x00 0x00 0x00) >> $SCRATCH/image.5
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x01 0x00 0x00 0x00) >> $SCRATCH/image.5
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x80 0x00 0x00 0x00) >> $SCRATCH/image.5

echo -n "Testing bad-mount on $SCRATCH/image.5 ... "
test-mount bad-mount $SCRATCH/image.5


# test case: not enough inodes!
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x10 0x34 0xf0 0xf0) >  $SCRATCH/image.5
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x00 0x00 0x00 0x00) >> $SCRATCH/image.5
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x01 0x00 0x00 0x00) >> $SCRATCH/image.5
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x80 0x00 0x00 0x00) >> $SCRATCH/image.5

echo -n "Testing bad-mount on $SCRATCH/image.5 ... "
test-mount bad-mount $SCRATCH/image.5


# ...error: inodes count does not match (256), should be (128)
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x10 0x34 0xf0 0xf0) >  $SCRATCH/image.5
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x05 0x00 0x00 0x00) >> $SCRATCH/image.5
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x02 0x00 0x00 0x00) >> $SCRATCH/image.5
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x80 0x00 0x00 0x00) >> $SCRATCH/image.5

echo -n "Testing bad-mount on $SCRATCH/image.5 ... "
test-mount bad-mount $SCRATCH/image.5

#...error: inodes count does not match (112), should be (128)
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x10 0x34 0xf0 0xf0) >  $SCRATCH/image.5
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x05 0x00 0x00 0x00) >> $SCRATCH/image.5
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x01 0x00 0x00 0x00) >> $SCRATCH/image.5
echo -n -e $(printf '\\x%x\\x%x\\x%x\\x%x' 0x70 0x00 0x00 0x00) >> $SCRATCH/image.5

echo -n "Testing bad-mount on $SCRATCH/image.5 ... "
test-mount bad-mount $SCRATCH/image.5