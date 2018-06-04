#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void* ThreadEntry(void* arg){
    (void)arg;
    while(1){
        printf("New Thread!\n");
        sleep(1);
    }
    return NULL;
}

int main(){
    //创建一个线程
    pthread_t tid;
    pthread_create(&tid, NULL, ThreadEntry, NULL);
    //主线程进行死循环
    while(1){
        printf("Main Thread!\n");
        sleep(1);
    }
    return 0;
}
