//////////////////////////////////////////////////////////////
//服务器逻辑：
//1.启动（初始化）
//2.进入一个死循环（事件循环）
// a）从 socket 中读取请求（Request）
// b）根据 Request 的内容计算生成 Response
// c）把 Response 响应写回到socket
//
// 此处为了实现方便，我们实现一个 echo_server（回显服务器）
///////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

// ./server 127.0.0.1 9090
int main(int argc, char* argv[]){
    //0.校验命令行参数
    if(argc != 3){
        printf("Usage ./server [ip] [port]\n");
        return 1;
    }
    //1.服务器的初始化
    // a）创建 socket
    // AF_INET:ipv4协议
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0){
        perror("socket");
        return 1;
    }
    //b）绑定ip地址端口号
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    //点分十进制的字符串ip地址转换成数字（网络字节序）
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    int ret = bind(fd, (sockaddr*)&addr, sizeof(addr));
    if(ret < 0){
        perror("bind");
        return 1;
    }
    //2.进入死循环
    while(1){
        //a)读取请求
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        char buf[1024] = {0};
        ssize_t read_size = recvfrom(fd, buf, sizeof(buf) - 1, 0, (sockaddr*)&peer, &len);
        if(read_size < 0){
            perror("recvfrom");
            continue;
        }
        buf[read_size] = '\0';
        //b)根据请求进行计算(此处由于实现的是echo_server,所以省略这一步)
        printf("[client %s:%d] say:%s\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port), buf);
        //c)把响应结果写回到socket
        //缓冲区长度最好不要写成sizeof,由于我们传输数据只传输有效部分就可以了
        sendto(fd, buf, strlen(buf), 0, (sockaddr*)&peer, sizeof(peer));
    }
    close(fd);
    return 0;
}

