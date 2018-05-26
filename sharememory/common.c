#include "common.h"

#include <stdio.h>

int ShmCommon(int size, int flags){
    key_t key = ftok(PATHNAME, PROJ_ID);
    if(key < 0){
        perror("ftok error");
        return -1;
    }
    int shmid = shmget(key, size, flags);
    if(shmid < 0){
        perror("shmget error");
        return 1;
    }
    return shmid;
}

int ShmCreate(int size){
    return ShmCommon(size, IPC_CREAT | IPC_EXCL | 0666);
}

int ShmOpen(){
    return ShmCommon(0, IPC_CREAT);
}

int ShmDestroy(int shmid){
    int ret = shmctl(shmid, IPC_RMID, NULL);
    if(ret < 0){
        perror("shmctl error");
        return -1;
    }
    return 0;
}

////////////////////////////////////////////////////////
//以下是测试代码
///////////////////////////////////////////////////////
#if 0

void TestCreate(){
    int shmid = ShmCreate(1024);
    printf("shmid = %d\n", shmid);
}

void TestOpen(){
    int shmid = ShmOpen();
    printf("shmid = %d\n", shmid);
}

void TestDestroy(){
    int shmid = ShmOpen();
    int ret = ShmDestroy(shmid);
    printf("ret expect 0, actual %d\n", ret);
}

int main(){
    //TestCreate();
    //TestOpen();
    TestDestroy();
    return 0;
}
#endif
