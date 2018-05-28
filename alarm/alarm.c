#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>

void MyHandler(int sig){
    (void) sig;
    printf("count = %lu\n", count);
    exit(0);
}

int main(){
    uint64_t count = 0;
    signal(SIGALRM, MyHandler);
    alarm(3);
    while(1){
        ++count;
    }
    return 0;
}
