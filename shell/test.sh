#!/bin/zsh

read x
if [ $x -eq 100 ];then
    echo "success"
elif [ $x -eq 200 ];then
    echo "success200"
elif [ $x -eq 300 ];then
    echo "success300"
else
    if [ $x -eq 400 ];then
        echo "success400"
    elif [ $x -eq 500 ];then
        echo "success500"
    else
        echo "failed"
    fi
fi

#read x
# -eq相等 -ne不相等 -lt -gt -le -ge
#test $x -eq 100
#y=100
#[ $x -eq 100 -a $y -eq 100 ]

#read str
##test "X$str" == "Xaaa"
##[ "X$str" == "aaa" ]
##[ -z "$str" ]     #str是否为空
#[ -n "$str" ]     #str是否不为空
#echo $?

#read file
#[ -c $file ]   #判断文件是否是字符设备
#[ -d $file ]   #判断是否是目录
#echo $?

##myDate=`date +%Y.%m.%d`
#myDate=$(date +%Y.%m.%d)   推荐使用
#echo "date is:"$myDate
#
#x=100
#y=200
##z=$(( x+y ))
#(( z=x+y  ))
#echo $z

#i=0
#for (( ; i <= 100; ++i ))
#do
#    touch file$i
#done

#str='a'
#str+='b'
#str+='b'
#str+='b'
#str+='b'
#echo ${#str}

#echo $g_x

#x=200
#echo $x
#unset x     #unset为删除shell脚本变量
#echo $x

#str1='hello'  在父bash中定义
#str2=' world'
#
#echo $str1$str2" hello bit"
#echo $str1${str2} hello bit

#echo "hello shell"

#myint=1234
#myfloat=3.14
#mystr='hello'
#mychar='c'
#
#echo $myint
#echo $myfloat
#echo $mystr
#echo $mychar
