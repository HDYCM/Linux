//////////////////////////////////////////////////////////////////
//客户端逻辑：
//1.从标准输入读两个数字
//2.构造 Request 结构，并把请求发送到服务器上
//3.尝试从服务器读取响应
//4.把响应结果打印到标准输出上
//////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "proto.h"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage ./client [ip] [port]\n");
        return 1;
    }
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0){
        perror("socket");
        return 1;
    }
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    while(1){
        //从标准输入读数据
        Request req;
        printf("请输入第一个数字：");
        fflush(stdout);
        scanf("%d", &req.a);
        printf("请输入第二个数字：");
        fflush(stdout);
        scanf("%d", &req.b);
        //注意！！此处读到的a,b是主机字节序的变量，需要先转换成网络字节序再传输
        req.a = htonl(req.a);
        req.b = htonl(req.b);

        //构造请求并发送给服务器
        sendto(fd, &req, sizeof(req), 0, (sockaddr*)&server_addr, sizeof(server_addr));
        //尝试从服务器读取返回结果并解析
        Response resp;
        //此处收到的数据一定是服务器发来的数据，也就不用关注对端的ip地址和端口号
        ssize_t read_size = recvfrom(fd, &resp, sizeof(resp), 0, NULL, NULL);
        if(read_size < 0){
            perror("recvfrom");
            continue;
        }

        //此处也要注意！！读到的结果需转换回主机字节序
        resp.sum = ntohl(resp.sum);

        //将结果打印到标准输出上
        printf("server resp: sum=%d\n", resp.sum);
    }
    return 0;
}

