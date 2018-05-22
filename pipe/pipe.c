#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
    int fd[2];
    int ret = pipe(fd);
    if(ret < 0){
        perror("pipe error");
        return 1;
    }
    while(1){
        //借助管道完成读写功能
        //1.先从标准输入读数据
        printf("> ");
        fflush(stdout);
        char buf[1024] = {0};
        size_t read_size = read(0, buf, sizeof(buf) - 1);      //防止\0没地方放
        if(read_size < 0){
            //读取失败
            perror("read error");
            return 1;
        }
        if(read_size == 0){
            //读到文件结束EOF
            printf("read done\n");
            return 0;
        }
        buf[read_size] = '\0';
        //2.把数据写到管道中
        write(fd[1], buf, strlen(buf));
        //3.从管道中读数据
        char buf_output[1024] = {0};
        read_size = read(fd[1], buf_output, sizeof(buf_output) - 1);
        if(read_size < 0){
            //读失败
            perror("read error");
            return 1;
        }
        if(read_size == 0){
            //如果这个管道的所有写端都关闭，那么从读端读数据就会read返回0
            printf("pipe close!\n");
            return 0;
        }
        buf_output[read_size] = '\0';
        //4.把数据写到标准输出
        write(1, buf_output, strlen(buf_output));
    }
    return 0;
}
