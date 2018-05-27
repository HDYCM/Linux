#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "common.h"

int main(){
    int semid = SemCreate(1);
    SemSetValue(semid, 0, 1);
    int ret = fork();
    if(ret == 0){
        //child
        while(1){
            P(semid, 0);
            printf("A");
            fflush(stdout);
            usleep(123456);
            printf("A ");
            fflush(stdout);
            usleep(323456);
            V(semid, 0);
        }
    }
    else if(ret > 0){
        //father
        while(1){
            P(semid, 0);
            printf("B");
            fflush(stdout);
            usleep(223456);
            printf("B ");
            fflush(stdout);
            usleep(121456);
            V(semid, 0);
        }
        wait(NULL);
    }
    SemDestroy(semid);
    return 0;
}
