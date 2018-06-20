//////////////////////////////////////////////////////
//客户端逻辑：
//1.用户输入数据，从标准输入输入一个字符串
//2.把这个字符串发送给服务器（请求Request）
//3.从服务器读取返回结果（响应Response）
//4.把返回结果打印到标准输出上
//////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

//./client [server_ip][server_port]
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
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    //点分十进制的字符串ip地址转换成数字（网络字节序）
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    while(1){
        //1.从标准输入读取数据
        char buf[1024] = {0};
        ssize_t read_size = read(0, buf, sizeof(buf) - 1);
        if(read_size < 0){
            //此处由于是客户端，所以处理方式和服务器不相同
            //服务器要保证不能因为单个客户端的异常数据导致服务器直接终止程序
            //但是客户端的话就可以根据需要决定是终止还是忽略错误
            perror("read");
            return 1;
        }
        if(read_size == 0){
            printf("read done!\n");
            return 0;
        }
        //2.发送数据到服务器
        sendto(fd, buf, strlen(buf), 0, (sockaddr*)&server_addr, sizeof(server_addr));
        //3.尝试从服务器读取响应
        char buf_resp[1024] = {0};
        //recvform第五个第六个参数表示对端的ip和端口号
        //此时由于客户端收到的数据一定是服务器返回的响应数据
        //所以此时就可以忽略掉对端的ip和端口号
        read_size = recvfrom(fd, buf_resp, sizeof(buf_resp), 0, NULL, NULL);
        if(read_size < 0){
            perror("recvform");
            return 1;
        }
        buf_resp[read_size] = '\0';
        //4.把响应写到标准输出上
        printf("server resp:%s\n", buf_resp);
    }
    close(fd);
    return 0;
}
