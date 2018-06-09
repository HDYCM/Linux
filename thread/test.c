#include <stdio.h>
#include <signal.h>

void MyHandler(int sig){
    printf("sig = %d\n", sig);
}

int main(){
    signal(SIGSEGV, MyHandler);
    int* p = NULL;
    *p = 10;
    return 0;
}
