#!/bin/bash

if [ $# -eq 0 ];then
    echo "Usage: $0 arg1 arg2 arg3 ... argn"
    exit 1
fi

min=$1
max=$1
total=0

for i in $@
do
    if [ $min -gt $i ];then
        min=$i
    fi

    if [ $max -lt $i ];then
        max=$i
    fi

    let total+=$i
done

echo "max: $max"
echo "min: $min"
echo "avg: `echo "ibase=10;scale=2;$total/$#" | bc`"
