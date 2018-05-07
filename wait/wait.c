#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
    printf("father %d \n", getpid());
    int ret = fork();
    if(ret == 0){
        printf("child1 %d\n", getpid());
        sleep(3);
        exit(0);
    }
    ret = fork();
    if(ret == 0){
        printf("child2 %d\n", getpid());
        sleep(5);
        exit(0);
    }
    wait(NULL);
    wait(NULL);
    printf("father wait!\n");
    while(1){
        sleep(1);
    }
    return 0;
}
