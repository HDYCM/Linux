#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(){
    int sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sfd == -1){
        perror("socket");
        exit(1);
    }
    int opt = 1;
    setsockopt(sfd, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt));
    char buf[1500];              //1500是数据链路层大小
    while(1){
        memset(buf, 0x00, sizeof(buf));
        int r = read(sfd, buf, 1500);
        if(r <= 0){
            break;
        }
        struct iphdr* pip = (struct iphdr*)(buf);
        struct in_addr ad;
        ad.s_addr = pip->saddr;            //源IP
        printf("protocol:%hhd,%s  <-------------> ",pip->protocol, inet_ntoa(ad));
        ad.s_addr = pip->daddr;            //目标IP
        printf("%s\n", inet_ntoa(ad));
    }
    return 0;
}

