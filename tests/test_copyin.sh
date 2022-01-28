#!/bin/bash

FSBIN="./solution/test_goatfs"
SCRATCH=$(mktemp -d)
trap "rm -fr $SCRATCH" INT QUIT TERM EXIT

# Test: data/image.5
cp data/image.5 $SCRATCH/image.5

$FSBIN $SCRATCH/image.5 5 copyin-test-1 $SCRATCH > /dev/null 2>&1
echo -n "Testing copyin in $SCRATCH/image.5 ... "

if [ $(md5sum $SCRATCH/1.copy | awk '{print $1}') = '1edec6bc701059c45053cf79e7e16588' ]; then
    echo -e "\e[32m passed\e[0m"
else
    echo -e "\e[31m failed\e[0m"
fi



# Test: data/image.20
cp data/image.20 $SCRATCH/image.20

$FSBIN $SCRATCH/image.20 20 copyin-test-2 $SCRATCH > /dev/null 2>&1

echo -n "Testing copyin in $SCRATCH/image.20 ... "
if [ $(md5sum $SCRATCH/2.copy | awk '{print $1}') = '1adf08d52e0f1a162a3a887a19fcf1f8' ] &&
   [ $(md5sum $SCRATCH/3.copy | awk '{print $1}') = 'd083a4be9fde347b98a8dbdfcc196819' ]; then
    echo -e "\e[32m passed\e[0m"
else
    echo -e "\e[31m failed\e[0m"
fi


# Test: data/image.200

cp data/image.200 $SCRATCH/image.200

$FSBIN $SCRATCH/image.200 200 copyin-test-3 $SCRATCH > /dev/null 2>&1

echo -n "Testing copyin in $SCRATCH/image.200 ... "
if [ $(md5sum $SCRATCH/1.copy | awk '{print $1}') = '0af623d6d8cb0a514816e17c7386a298' ]; then
    echo -e "\e[32m passed\e[0m"
else
    echo -e "\e[31m failed\e[0m"
fi


# different files
cp data/image.200 $SCRATCH/image.200
$FSBIN $SCRATCH/image.200 200 copyin-test-4 $SCRATCH > /dev/null 2>&1

echo -n "Testing copyin in $SCRATCH/image.200 ... "
if [ $(md5sum $SCRATCH/2.copy | awk '{print $1}') = '307fe5cee7ac87c3b06ea5bda80301ee' ]; then
    echo -e "\e[32m passed\e[0m"
else
    echo -e "\e[31m failed\e[0m"
fi


# test case: what if the file system is full...
cp data/image.200 $SCRATCH/image.200
$FSBIN $SCRATCH/image.200 200 copyin-test-5 $SCRATCH > /dev/null 2>&1

echo -n "Testing copyin in $SCRATCH/image.200 ... "
if [ $(md5sum $SCRATCH/1.copy | awk '{print $1}') = '0af623d6d8cb0a514816e17c7386a298' ] &&
   [ $(md5sum $SCRATCH/2.copy | awk '{print $1}') = '307fe5cee7ac87c3b06ea5bda80301ee' ] &&
   [ $(md5sum $SCRATCH/9.copy | awk '{print $1}') = 'fa4280d88da260281e509296fd2f3ea2' ]; then
    echo -e "\e[32m passed\e[0m"
else
    echo -e "\e[31m failed\e[0m"
fi