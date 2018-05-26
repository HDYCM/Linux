#include <stdio.h>
#include <unistd.h>
#include "common.h"

int main(){
    int shmid = ShmCreate(1024);
    if(shmid < 0){
        perror("ShmCreate failed!");
        return 1;
    }
    char* addr = shmat(shmid, NULL, 0);
    sleep(2);
    int i = 0;
    while(i++ < 26){
        printf("client# %s\n", addr);
        sleep(1);
    }
    shmdt(addr);
    sleep(2);
    ShmDestroy(shmid);
    return 0;
}
