#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int fd = open("./myfifo", O_RDONLY);
    if(fd < 0){
        perror("open error");
        return 1;
    }
    while(1){
        char buf[1024] = {0};
        printf("Please wait ...\n");
        ssize_t read_size = read(fd, buf, sizeof(buf) - 1);
        if(read_size < 0){
            perror("read error");
            return 1;
        }
        if(read_size == 0){
            printf("read done!\n");
            return 0;
        }
        buf[read_size] = '\0';
        printf("client say# %s", buf);
    }
    close(fd);
    return 0;
}


