#!/bin/bash

FSBIN="./solution/test_goatfs"

SCRATCH=$(mktemp -d)
trap "rm -fr $SCRATCH" INT QUIT TERM EXIT

# Test: data/image.5

echo -n "Testing copyout in data/image.5 ... "

$FSBIN data/image.5 5 copyout-test-1 $SCRATCH > /dev/null 2>&1
if [ $(md5sum $SCRATCH/1.txt | awk '{print $1}') = '1edec6bc701059c45053cf79e7e16588' ]; then
    echo -e "\e[32m passed\e[0m"
else
    echo -e "\e[31m failed\e[0m"
fi


# Test: data/image.20
echo -n "Testing copyout in data/image.20 ... "

$FSBIN data/image.20 20 copyout-test-2 $SCRATCH > /dev/null 2>&1

if [ $(md5sum $SCRATCH/2.txt | awk '{print $1}') = 'bfd6a31563edfd2a943edc29c37366b1' ] &&
   [ $(md5sum $SCRATCH/3.txt | awk '{print $1}') = 'd083a4be9fde347b98a8dbdfcc196819' ]; then
    echo -e "\e[32m passed\e[0m"
else
    echo -e "\e[31m failed\e[0m"
fi


# Test: data/image.200
echo -n "Testing copyout in data/image.200 ... "

$FSBIN data/image.200 200 copyout-test-3 $SCRATCH > /dev/null 2>&1

if [ $(md5sum $SCRATCH/1.txt | awk '{print $1}') = '0af623d6d8cb0a514816e17c7386a298' ] &&
   [ $(md5sum $SCRATCH/2.txt | awk '{print $1}') = '307fe5cee7ac87c3b06ea5bda80301ee' ] &&
   [ $(md5sum $SCRATCH/9.txt | awk '{print $1}') = 'cc4e48a5fe0ba15b13a98b3fd34b340e' ]; then
    echo -e "\e[32m passed\e[0m"
else
    echo -e "\e[31m failed\e[0m"
fi
