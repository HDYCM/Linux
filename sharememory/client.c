#include <stdio.h>
#include <unistd.h>
#include "common.h"

int main(){
    int shmid = ShmOpen();
    if(shmid < 0){
        perror("ShmOpen failed!");
        return 1;
    }
    sleep(1);
    //把物理内存关联(挂接，attach)到某个进程的虚拟地址空间之中
    char* addr = shmat(shmid, NULL, 0);
    sleep(2);
    int i = 0;
    while(i < 26){
        addr[i] = 'A' + i;
        i++;
        addr[i] = 0;
        sleep(1);
    }
    //将共享内存与当前进程脱离
    shmdt(addr);
    sleep(2);
    return 0;
}
