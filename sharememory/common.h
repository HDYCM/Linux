#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PATHNAME "."
#define PROJ_ID 1

//如果不存在，就创建
//如果存在，就失败
int ShmCreate(int size);

//如果不存在就失败
//如果存在就打开
int ShmOpen();

//销毁共享内存
int ShmDestroy(int shmid);
