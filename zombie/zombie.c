#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    int ret = fork();
    if(ret < 0){
        perror("fork");
        return 1;
    }
    else if(ret > 0){
        //father
        printf("parent[%d] is sleeping...\n", getpid());
        sleep(30);
    }
    else{
        //child
        printf("child[%d] is begin z...\n",getpid());
        sleep(5);
        exit(EXIT_SUCCESS);
    }
    return 0;
}
