#include "common.h"
#include <stdio.h>

int SemCommon(int nums, int flags){
    key_t key = ftok(PATHNAME, PROJ_ID);
    if(key < 0){
        perror("ftok error");
        return -1;
    }
    int semid = semget(key, nums, flags);
    if(semid < 0){
        perror("semget error");
        return -1;
    }
    return semid;
}

int SemCreate(int nums){
    return SemCommon(nums, IPC_CREAT | IPC_EXCL | 0666);
}

int SemOpen(){
    return SemCommon(0, IPC_CREAT);
}

int SemDestroy(int semid){
    int ret = semctl(semid, 0, IPC_RMID);
    if(ret < 0){
        perror("semctl error");
        return -1;
    }
    return 0;
}

void SemSetValue(int semid, int index, int value){
    union semun _semun;
    _semun.val = value;
    int ret = semctl(semid, index, SETVAL, _semun);
    if(ret < 0){
        perror("semctl SETVAL");
        return;
    }
}

void SemGetValue(int semid, int index, int* value){
    int ret = semctl(semid, index, GETVAL);
    if(ret < 0){
        perror("semctl GETVAL");
        return;
    }
    *value = ret;
}



//////////////////////////////////////////////////////////////////
//以下是测试函数
///////////////////////////////////////////////////////////////////
#if 1
void TestCreate(){
    int semid = SemCreate(1);
    printf("semid = %d\n", semid);
}

void TestOpen(){
    int semid = SemOpen();
    printf("semid = %d\n", semid);
}

void TestDestroy(){
    int semid = SemOpen();
    int ret = SemDestroy(semid);
    printf("ret expect 0, actual %d\n", ret);
}

void TestSetGet(){
    int semid = SemCreate(1);
    SemSetValue(semid, 0, 1);
    int value = 0;
    SemGetValue(semid, 0, &value);
    printf("value expect 1, actual %d\n", value);
}

int main(){
    //TestCreate();
    //TestOpen();
    //TestDestroy();
    TestSetGet();
    return 0;
}
#endif
