#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void PrintSigset(sigset_t* set){
    int i = 1;
    for(; i < 32; ++i){
        if(sigismember(set, i)){   //判断指定信号是否在目标集合中
            printf("1");
        }
        else{
            printf("0");
        }
    }
    printf("\n");
}

void MyHandler(int sig){
    printf("sig = %d\n", sig);
}

int main(){
    //1.捕捉 SIGINT 信号
    signal(SIGINT, MyHandler);
    //2.把 SIGINT 信号屏蔽掉
    sigset_t set;
    sigset_t oset;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_BLOCK, &set, &oset);
    //3.循环读取未决信号集
    int count = 0;
    while(1){
        ++count;
        if(count >= 6){
            count = 0;
            //解除信号屏蔽字，再设置上
            printf("解除信号屏蔽字！\n");
            sigprocmask(SIG_SETMASK, &oset, NULL);
            sleep(1);
            printf("再次设置信号屏蔽字！\n");
            sigprocmask(SIG_BLOCK, &set, &oset);
        }
        sigset_t pending_set;
        sigpending(&pending_set);
        PrintSigset(&pending_set);
        sleep(1);
    }
    return 0;
}
