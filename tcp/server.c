///////////////////////////////////////////////////////////////
//服务器基本流程：
//1.从socket中读取数据(Request)
//2.根据 Request 计算生成 Response
//3.把 Response 写回客户端
//由于目前实现的是 echo_server, 计算生成 Response 步骤就省略了
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

void ProcessConnect(int new_sock){
    //完成一次连接的处理
    //需要循环的来处理客户端发送的数据
    while(1){
        //a)从客户端读取数据
        char buf[1024] = {0};
        ssize_t read_size = read(new_sock, buf, sizeof(buf) - 1);
        if(read_size < 0){
            perror("read");
            continue;
        }
        if(read_size == 0){
            //TCP中，如果read的返回值是0，说明对端关闭了连接
            printf("[client %d] disconnect!\n", new_sock);
            close(new_sock);
            return;
        }
        buf[read_size] = '\0';
        //b)根据请求计算响应(省略)
        printf("[client %d] %s\n", new_sock, buf);
        //c)把响应写回到客户端
        write(new_sock, buf, strlen(buf));
    }
}

//./server [ip] [port]
int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage ./server [ip] [port]\n");
        return 1;
    }
    //1.创建 socket
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sock < 0){
        perror("socket");
        return 1;
    }
    //2.绑定端口号
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));
    int ret = bind(listen_sock, (sockaddr*)&server, sizeof(server));
    if(ret < 0){
        perror("bind");
        return 1;
    }
    //3.使用 listen 允许服务器被客户端连接
    ret = listen(listen_sock, 5);
    if(ret < 0){
        perror("listen");
        return 1;
    }
    //4.服务器初始化完成，进入事件循环
    printf("Server Init OK!\n");
    while(1){
        //把小板凳上的顾客请到屋里吃饭
        sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int new_sock = accept(listen_sock, (sockaddr*)&peer, &len);
        if(new_sock < 0){
            perror("accept");
            continue;
        }
        printf("[client %d] connect!\n", new_sock);
        ProcessConnect(new_sock);
    }
    return 0;
}

