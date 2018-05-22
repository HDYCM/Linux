#include <stdio.h>
#include <stddef.h>
#include "common.h"

int main(){
    int msgid = MsgOpen();
    while(1){
        char buf[1024] = {0};
        printf("Please Enter：");
        fflush(stdout);
        //从标准输入读数据
        ssize_t read_size = read(0, buf, sizeof(buf)-1);
        if(read_size < 0){
            perror("read error");
            return 1;
        }
        if(read_size == 0){
            printf("read done!\n");
            return 0;
        }
        buf[read_size] = '\0';
        //2.把数据发送给服务器
        MsgSend(msgid, CLIENT_TYPE, buf, strlen(buf));
        printf("send done, wait recv...\n");
        //3.读取服务器返回结果
        char output_buf[1024] = {0};
        MsgRecv(msgid, SERVER_TYPE, output_buf, sizeof(output_buf));
        //4.将结果打印在标准输出上
        printf("server# %s\n", output_buf);
    }
    return 0;
}

