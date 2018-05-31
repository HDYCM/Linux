#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void MyHandler(int sig){
    (void) sig;
}

void MySleep1(int second){
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

//这个版本的函数来解决竞态条件问题
void MySleep2(int second){
    //1.捕捉 SIGALRM 信号，并且要把原来的信号处理方法备份下来
    __sighandler_t old_handler = signal(SIGALRM, MyHandler);

    //2.屏蔽闹钟信号
    sigset_t set, oset;
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    sigprocmask(SIG_BLOCK, &set, &oset);

    //3.通过 alarm 注册一个闹钟
    alarm(second);

    //4.需要借助一个函数来等待闹钟信号的到来
    //  在闹钟信号到来之前，挂起等待
    //  一旦闹钟信号来了，继续往后执行
    //  此处不能使用 pause, 而要使用 sigsuspend 来进行替换
    //  sigsuspend 能够做到原子的进行解除屏蔽+等待
    sigset_t tmp_set = oset;
    //  此处我们基于oset备份了一份tmp_set
    //  但是 tmp_set 里面可能也包含了对 SIGALRM 的屏蔽
    sigdelset(&tmp_set, SIGALRM);
    sigsuspend(&tmp_set);

    //5.恢复 SIGALRM 的信号处理方式
    //  如果不恢复的话，就可能影响到其他信号
    signal(SIGALRM, old_handler);

    //6.恢复信号屏蔽字
    sigprocmask(SIG_SETMASK, &oset, NULL);
    return;
}

int main(){
    printf("before sleep...\n");
    //MySleep1(3);
    MySleep2(3);
    printf("after sleep...\n");
    return 0;
}
