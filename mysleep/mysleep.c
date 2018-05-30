#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void MyHandler(int sig){
    (void) sig;
}

void MySleep(int second){
    //1.捕捉 SIGALRM 信号，并且要把原来的信号处理方法备份下来
    __sighandler_t old_handler = signal(SIGALRM, MyHandler);
    //2.通过 alarm 注册一个闹钟
    alarm(second);
    //3.需要借助一个函数来等待闹钟信号的到来
    //  在闹钟信号到来之前，挂起等待
    //  一旦闹钟信号来了，继续往后执行
    pause();
    //4.恢复 SIGALRM 的信号处理方式
    //  如果不恢复的话，就可能影响到其他信号
    signal(SIGALRM, old_handler);
    return;
}

int main(){
    printf("before sleep...\n");
    MySleep(3);
    printf("after sleep...\n");
    return 0;
}
