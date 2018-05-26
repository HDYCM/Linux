#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PATHNAME "."
#define PROJ_ID 1

int ShmCreate(int size);

int ShmOpen();

int ShmDestroy(int shmid);
