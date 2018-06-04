#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

void usage(const char* proc){
    printf("Usage: %s signo pid\n", proc);
}

int main(int argc, char* argv[]){
    if(argc != 3){
        usage(argv[0]);
        return 0;
    }
    int signo = atoi(argv[1]);
    int pid = atoi(argv[2]);
    kill(pid, signo);
    return 0;
}
