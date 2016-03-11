#!/bin/sh

PROC_C=$(cat /proc/cpuinfo | grep processor | wc -l)

BASE=$(dirname $0)

if [ -d $BASE/build ]; then
        rm -r $BASE/build/
fi

mkdir $BASE/build
cd $BASE/build
cmake ..
make -j$PROC_C
cd -
