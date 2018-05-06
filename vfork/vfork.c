#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int glob = 100;

int main(){
    pid_t pid;
    if((pid = vfork()) == -1){
        perror("fork");
        _exit(1);
    }
    if(pid == 0){
        glob = 200;
        printf("child glob %d\n", glob);
        sleep(5);
        _exit(0);
    }
    else{
        printf("father glob %d\n", glob);
    }
    return 0;
}
