#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(){
    while(1){
        sleep(1);
        //raise(3);
        abort();
    }
    return 0;
}
