/////////////////////////////////////////////////////////////////////////////
//服务器流程
//1.初始化(UDP)
//2.循环处理请求
//   a)读取请求并解析
//     读到的内容本来是一个字符串，从字符串中解析出两个数字
//   b)进行计算
//   c)构造响应并返回
//
//   使用方案二来进行实现，需要定义两个结构体分别表示Request和Response
////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "proto.h"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

int UdpServerInit(char* ip, short port){
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0){
        perror("socket");
        return -1;
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    int ret = bind(fd, (sockaddr*)&addr, sizeof(addr));
    if(ret < 0){
        perror("bind");
        return -1;
    }
    return fd;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage ./server [ip] [port]\n");
        return 1;
    }
    //创建一个 UDP 服务器
    int fd = UdpServerInit(argv[1], atoi(argv[2]));
    if(fd < 0){
        printf("UdpServerInit failed!\n");
    }
    printf("UdpServerInit OK!\n");
    while(1){
        //a)读取请求
        sockaddr_in peer;
        socklen_t len = sizeof(peer);
        Request req;
        ssize_t read_size = recvfrom(fd, &req, sizeof(req), 0, (sockaddr*)&peer, &len);
        if(read_size < 0){
            perror("recvfrom");
            continue;
        }
        //此处需要注意！！进行字节序的转换
        req.a = ntohl(req.a);
        req.b = ntohl(req.b);
        
        //此处是否要给read_size对应的下标填 '\0'??
        //不需要！！此处读出的数据虽然放到一个字符数组中，但本质上并不是一个C风格字符串
        printf("[Request] a=%d, b=%d\n", req.a, req.b);

        //b)根据读到的请求进行计算
        int sum = req.a + req.b;

        //c)把响应结果写到客户端
        Response resp;
        resp.sum = sum;
        printf("[Response] sum=%d\n", resp.sum);

        //此处需要注意！！发送之前需要转换成网络字节序
        resp.sum = htonl(resp.sum);

        sendto(fd, &resp, sizeof(resp), 0, (sockaddr*)&peer, len);
    }
    return 0;
}
