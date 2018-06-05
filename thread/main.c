#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* ThreadEntry1(void* arg){
    (void)arg;
    printf("Thread1\n");
    return (void*)1;
}

void* ThreadEntry2(void* arg){
    (void)arg;
    printf("Thread2");
//    pthread_exit((void*)2);
    return NULL;
}

int main(){
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, ThreadEntry1, NULL);
    pthread_create(&tid2, NULL, ThreadEntry2, NULL);
    while(1);
    return 0;
}
