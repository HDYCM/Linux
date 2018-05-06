#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    int ret = fork();
    if(ret < 0){
        perror("fork");
        return 1;
    }
    else if(ret == 0){
        printf("I am child:  pid: %d, ppid %d, ret %d\n",getpid(), getppid(), ret);
    }
    else{
        printf("I am father: pid: %d, ppid %d, ret %d\n",getpid(), getppid(), ret);
    }
    sleep(60);
    return 0;
}
