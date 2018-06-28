#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

void PackageResponse(char http_resp[]){
    int cur_len = 0;
    const char* html = "<html>hello</html>";
    cur_len += sprintf(http_resp, "HTTP/1.1 200 OK\n");
    cur_len += sprintf(http_resp + cur_len, "Content-Length: %lu\n",strlen(html));
    cur_len += sprintf(http_resp + cur_len, "\n");
    cur_len += sprintf(http_resp + cur_len, "%s", html);
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage ./server [ip] [port]\n");
        return 1;
    }
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        perror("socket");
        return 1;
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int ret = bind(fd, (sockaddr*)&addr, sizeof(addr));
    if(ret < 0){
        perror("bind");
        return 1;
    }

    ret = listen(fd, 5);
    if(ret < 0){
        perror("listen");
        return 1;
    }

    while(1){
        sockaddr_in peer_addr;
        socklen_t len = sizeof(peer_addr);
        int new_fd = accept(fd, (sockaddr*)&peer_addr, &len);
        if(new_fd < 0){
            perror("accept");
            continue;
        }
        char buf[100 * 1024] = {0};
        ssize_t read_size = read(new_fd, buf, sizeof(buf) - 1);
        if(read_size < 0){
            perror("read");
            close(new_fd);
            continue;
        }
        if(read_size == 0){
            close(new_fd);
            continue;
        }
        buf[read_size] = '\0';
        printf("[Request]\n%s\n", buf);

        char http_resp[100 * 1024] = {0};
        PackageResponse(http_resp);

        write(new_fd, http_resp, strlen(http_resp));
        close(new_fd);
    }
    return 0;
}
