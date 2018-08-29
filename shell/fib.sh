#!/bin/bash

if [ $# -ne 1 ];then
    echo "Usage: $0 num"
    exit 1
fi

arr[1]=1
arr[2]=1

i=3
while [ $i -le $1 ]
do
    let arr[i]=arr[i-1]+arr[i-2]
    let i++
done

echo "reault: ${arr[$1]}"

#if [ $# -ne 1 ];then
#    echo "Usage: $0 num"
#    exit 1
#fi
#
#first=1
#second=1
#third=2
#
#i=$1
#while [ $i -ge 3 ]
#do
#    let third=first+second
#    let first=second
#    let second=third
#    let i--
#done
#
#echo "result: $third"
