#include <stdio.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define BUF_SZ 1024
#define PACK_LEN 56

int sendnum = 0;
int recvnum = 0;
char sendpack[BUF_SZ];
char recvpack[BUF_SZ];
struct sockaddr_in from;

void Handler(int sig){
    (void)sig;
    printf("--- www.a.shifen.com ping statistics ---\n");
    printf("%d packets transmitted, %d received, %.3f%% packet loss.\n", sendnum, recvnum, ((sendnum - recvnum) / sendnum) * 100);

    exit(0);
}

//两个时间相差多少毫秒
long diftime(const struct timeval* end, const struct timeval* begin){
    return (end->tv_sec - begin->tv_sec) * 1000 + (end->tv_usec - begin->tv_usec) / 1000;
}

//校验和算法
unsigned short chksum(unsigned short* addr, int len){
    unsigned int ret = 0;
    while(len > 1){
        ret += *addr++;
        len -=2;
    }
    if(len == 1){
        ret += *(char*)addr;
    }
    ret = (ret >> 16) + (ret & 0xffff);
    ret = ret + (ret >> 16);
    return (unsigned short)~ret;
}

//封包
int pack(int num, pid_t pid){
    memset(sendpack, 0x00, sizeof(sendpack));

    struct icmp* p = (struct icmp*)sendpack;
    p->icmp_type = ICMP_ECHO;
    p->icmp_code = 0;
    p->icmp_cksum = 0;
    p->icmp_seq = num;
    p->icmp_id = pid;
    struct timeval tval;
    gettimeofday(&tval, NULL);
    memcpy((void*)p->icmp_data, (void*)&tval, sizeof(tval));

    p->icmp_cksum = chksum((unsigned short*)sendpack, PACK_LEN + 8);
    return PACK_LEN + 8;
}

//发包
void send_packet(int sfd, pid_t pid, struct sockaddr_in ad){
    sendnum++;
    int r = pack(sendnum, pid);
    sendto(sfd, sendpack, r, 0, (struct sockaddr*)&ad, sizeof(ad));
}

//解包
void unpack(char* buf, int len, pid_t pid){
    struct ip* pip = (struct ip*)buf;
    struct icmp* picmp = (struct icmp*)(buf + (pip->ip_hl << 2));
    struct timeval end;
    gettimeofday(&end, NULL);
    printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%ld ms\n", 
               PACK_LEN, inet_ntoa(from.sin_addr), picmp->icmp_seq, pip->ip_ttl, diftime(&end, (struct timeval*)(picmp->icmp_data)));
}

//收包
void recv_packet(int sfd, pid_t pid){
    memset(recvpack, 0x00, sizeof(recvpack));
    socklen_t len = sizeof(from);
    int r;
    recvnum++;
    r = recvfrom(sfd, recvpack, BUF_SZ, 0, (struct sockaddr*)&from, &len);
    unpack(recvpack, r, pid);
}

//./a.out ip
int main(int argc, char* argv[]){
    if(argc != 2){
        return 1;
    }
    struct in_addr addr;
    struct sockaddr_in ad;

    if((addr.s_addr = inet_addr(argv[1])) == INADDR_NONE){
        struct hostent* pent = gethostbyname(argv[1]);
        if(pent == NULL){
            perror("gethostbyname");
            exit(1);
        }
        memcpy((char*)&addr, (char*)pent->h_addr, pent->h_length);
    }

    //PF面向协议
    int sfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sfd == -1){
        perror("socket");
        exit(1);
    }

    signal(SIGINT, Handler);
    ad.sin_family = AF_FILE;
    ad.sin_addr = addr;
    pid_t pid = getpid();

    printf("PING %s (%s) %d(84) bytes of data.\n", argv[1], inet_ntoa(ad.sin_addr), PACK_LEN);
    while(1){
        send_packet(sfd, pid, ad);
        recv_packet(sfd, pid);
        sleep(1);
    }

    //struct timeval begin;
    //gettimeofday(&begin, NULL);
    //sleep(1);
    //struct timeval end;
    //gettimeofday(&end, NULL);
    //long ret = diftime(&end, &begin); 
    //printf("ret: %ld\n", ret);
    
    return 0;
}
