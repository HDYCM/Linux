#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

void MyDaemon(){
    //1.创建子进程，子进程继续执行，父进程终止
    int ret = fork();
    if(ret < 0){
        perror("fork");
        return;
    }
    if(ret > 0){
        exit(0);
    }
    //2.子进程调用 setsid
    setsid();
    //3.忽略 SIGHUP 信号
    signal(SIGHUP, SIG_IGN);
    //4.忽略 SIGCHLD 信号
    signal(SIGCHLD, SIG_IGN);
    //5.修改工作目录为 /
    chdir("/");
    //6.重定向文件描述符
    int fd = open("/dev/null", O_RDWR);
    if(fd < 0){
        perror("open");
        exit(1);
    }
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    //7.修改 umask
    umask(0);
}

int main(){
    MyDaemon();
    //daemon(0, 0);
    while(1){
        sleep(1);
    }
    return 0;
}
