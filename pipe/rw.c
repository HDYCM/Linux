#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 

int main(){
    int fd[2];
    int ret = pipe(fd);
    if(ret < 0){
        perror("pipe error");
        return 1;
    }
    pid_t pid = fork();
    if(pid < 0){
        perror("fork error");
        return -1;
    }
    if(pid == 0){
        close(fd[0]);
        write(fd[1], "hello", 5);
        close(fd[1]);
    }
    close(fd[1]);
    char buf[1024] = {0};
    ssize_t read_size =  read(fd[0], buf, sizeof(buf) - 1);
    if(read_size < 0){
        perror("read error");
        return 0;
    }
    if(read_size == 0){
        printf("read done!\n");
        return 0;
    }
    buf[read_size] = '\0';
    printf("#%s\n", buf);
    return 0;
}
