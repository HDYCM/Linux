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

int CommonPV(int semid, int index, int op){
    struct sembuf _sembuf;
    _sembuf.sem_num = index;
    _sembuf.sem_op = op;
    _sembuf.sem_flg = 0;
    int ret = semop(semid, &_sembuf, 1);
    if(ret < 0){
        perror("semop error");
        return -1;
    }
    return 0;
}

void P(int semid, int index){
    CommonPV(semid, index, -1);
}

void V(int semid, int index){
    CommonPV(semid, index, 1);
}
//////////////////////////////////////////////////////////////////
//以下是测试函数
///////////////////////////////////////////////////////////////////
#if 0
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
    SemDestroy(semid);
}

void TestPV(){
    int semid = SemCreate(1);
    SemSetValue(semid, 0, 1);
    P(semid, 0);
    int value = -1;
    SemGetValue(semid, 0, &value);
    printf("value expect 0, actual %d\n", value);
    V(semid, 0);
    SemGetValue(semid, 0, &value);
    printf("value expect 1, actual %d\n", value);
    SemDestroy(semid);
}

int main(){
    //TestCreate();
    //TestOpen();
    //TestDestroy();
    //TestSetGet();
    TestPV();
    return 0;
}
#endif
