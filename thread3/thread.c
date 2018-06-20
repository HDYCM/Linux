#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* ThreadEntry1(void* arg){
    (void)arg;
    printf("Thread1!\n");
    pthread_exit(NULL);
    return NULL;
}

void* ThreadEntry2(void* arg){
    (void)arg;
    printf("Thread2!\n");
    pthread_exit(NULL);
    return NULL;
}

int main(){
    printf("Main Thread!\n");
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, ThreadEntry1, NULL);
    pthread_create(&tid2, NULL, ThreadEntry2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
