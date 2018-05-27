#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define PATHNAME "."
#define PROJ_ID 1

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

//创建信号量集
int SemCreate();

//打开信号量集
int SemOpen();

//销毁信号量集
int SemDestroy(int semid);

//set 设置(初始化)
//semid 对哪个信号量集操作
//index 对信号量集中的那个信号量操作
//value 把对应的信号量设置成多少
void SemSetValue(int semid, int index, int value);

//get 获取
void SemGetValue(int semid, int index, int* value);

//P 操作
int P(int semid, int index);

//V 操作
int V(int semid, int index);
