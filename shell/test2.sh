#!/bin/zsh

source api.sh
read d1 d2 
 
res=$(intAdd $d1 $d2) 
 
echo $res

#trap 'echo hello world; trap 2' 2     #捕捉信号并恢复默认
#while :
#do
#    :
#done

#src=$(ls *.c)
#cc=gcc
#bin=mytest
#
#cat<<EOF >Makefile
#$bin:$src
#    $cc -o \$@ \$^
#.PHONY:clean
#clean:
#    rm -f $bin
#EOF

#read file
#while read line
#do 
#    echo hello:$line
#done<$filename>$filename.bak 
#i=0
#while [ $i -le 10 ]
#do 
#    echo $i 
#    let i++
#done >file

#myarray=('c' "hello" 3.14 89)
#myarray[4]=100
#myarray[100]=20
#echo ${myarray[*]}
#echo ${#myarray[@]}
#
#echo "#######################################"
#for (( i = 0; i < ${#myarray[@]}; ++i ))
#do
#    echo ${myarray[$i]}
#done
#echo "#######################################"
#for i in ${myarray[@]}
#do 
#    echo $i
#done

#echo "aaa"
#set -x
#echo "bbb"
#echo "ccc"
#set +x
#echo "ddd"

#把函数当命令或小脚本
#function myfun()
#{
#    echo "\$0 -> $0"
#    echo "\$1 -> $1"
#    echo "\$2 -> $2"
#    echo "\$3 -> $3"
#    echo "\$4 -> $4"
#    echo "\$5 -> $5"
#    echo "\$6 -> $6"
#    echo "\$# -> $#"
#    echo "\$@ -> $@"
#    echo "I am function!"
#}
#myfun -a -b -c 1 2 3
#function myfun()
#{
#    return 0
#}
#if [ $? -eq 0 ];then
#    echo OK
#fi

#while [ $# -gt 0 ]
#do
#    echo $i
#    shift 1
#done
#for i in $@
#do 
#    echo $i
#done

#echo "\$0 -> $0"
#echo "\$1 -> $1"
#echo "\$2 -> $2"
#echo "\$3 -> $3"
#echo "\$4 -> $4"
#echo "\$# -> $#"
#echo "\$@ -> $@"
#echo "\$? -> $?"
#echo "\$$ -> $$"
#echo "##################### shift 1 ###############"
#shift 1   #左移一位
#echo "\$0 -> $0"
#echo "\$1 -> $1"
#echo "\$2 -> $2"
#echo "\$3 -> $3"
#echo "\$4 -> $4"
#echo "\$# -> $#"
#echo "\$@ -> $@"
#echo "\$? -> $?"
#echo "\$$ -> $$"

#i=1
#sum=0
#str=''
#while [ $i -le 100 ]
#do
#    if [ $i -ne 100 ];then
#        str+=$i'+'
#    else
#        str+=$i
#    fi
#    let sum+=i
#    let i++
#    #(( sum+=i ))
#    #(( i++ ))
#done
#echo $str=$sum

#read x
#case $x in 
#    1 | [Ss]tart | -[Ss])
#        echo 1
#        ;;
#    2)
#        echo 2
#        ;;
#    3)
#        echo 3
#        ;;
#    *)
#        echo \*
#        ;;
#esac

#for i in {1..10}
#do
#    echo $i
#done
#
#for i in {a..z} {A..Z}    #有空格按顺序打印
#do 
#    echo $i
#done
#for i in {a..z}{A..Z}    #无空格按排列组合打印
#do 
#    echo $i
#done

#for (( i = 0; i <= 10; ++i ))
#do 
#    echo $i
#done
#
#i=0
#until [ $i -gt 10 ]
#do
#    echo $i
#    let i++
#done

#死循环
#shell脚本中不能出现空的代码块
#for (( ; ; ))
#do
#    echo "AAAA"
#done
#while :
#do 
#    echo "AAAAAA"
#done
