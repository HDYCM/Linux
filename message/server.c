#include "common.h"
#include <stdio.h>

int main(){
    int msgid = MsgCreate();
    while(1){
        char buf[1024] = {0};
        //1.从客户端接收数据
        MsgRecv(msgid, CLIENT_TYPE, buf, sizeof(buf));
        //2.读取接收的数据
        printf("client# %s\n", buf);
        //3.服务器发送数据给客户端
        char input_buf[1024] = {0};
        printf("Please Enter：");
        fflush(stdout);
        ssize_t read_size = read(0, input_buf, sizeof(input_buf) - 1);
        if(read_size < 0){
            perror("read error");
            return 1;
        }
        if(read_size == 0){
            printf("read done!\n");
            return 0;
        }
        input_buf[read_size] = '\0';
        MsgSend(msgid, SERVER_TYPE, input_buf, strlen(input_buf));
        printf("send done, wait recv...\n");
    }
}

