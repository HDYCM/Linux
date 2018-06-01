#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
    printf("father %d \n", getpid());
    int ret = fork();
    //第一个子进程
    if(ret == 0){
        printf("child1 %d\n", getpid());
        sleep(3);
        exit(0);
    }
    //第二个子进程
    ret = fork();
    if(ret == 0){
        printf("child2 %d\n", getpid());
        sleep(5);
        exit(0);
    }
    while(1){
        ret = waitpid(-1, NULL, WNOHANG);
        printf("ret = %d\n", ret);
        if(ret > 0){
            printf("wait child %d\n\n", ret);
        }
        else if(ret < 0){
            break;
        }
        else{
            printf("father do work!\n\n");
        }
        sleep(1);
    }
    return 0;
}
