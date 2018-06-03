#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void MyHandler(int sig){
    (void) sig;
    //int ret = wait(NULL);
    //printf("MyHandler:%d\n", ret);
    while(1){
        int ret = waitpid(-1, NULL, WNOHANG);
        if(ret > 0){
            printf("waitpid %d\n", ret);
            continue;
        }
        else if(ret == 0){
            //子进程还存在，没执行完，直接让信号处理函数返回
            //如果后续的子进程再结束，还会再次发送 SIGCHLD 信号
            break;
        }
        else{
            //子进程都结束
            break;
        }
    }
}

int main(){
    signal(SIGCHLD, MyHandler);
    int i = 0;
    for(; i < 20; ++i){
        pid_t ret = fork();
        if(ret < 0){
            perror("fork");
            return 1;
        }
        if(ret == 0){
            //child
            printf("child %d\n", getpid());
            sleep(3);
            exit(0);
        }
    }
    while(1){
        printf("father working\n");
        sleep(1);
    }
    return 0;
}
