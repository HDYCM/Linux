#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* ThreadEntry(void* arg){
    (void)arg;
    while(1);
    return NULL;
}

int main(){
    pthread_t tid[20];
    int n = 4;
    int i = 0;
    for(; i < n; ++i){
        pthread_create(&tid[i], NULL, ThreadEntry, NULL);
    }
    for(i = 0; i < n; ++i){
        pthread_join(tid[i], NULL);
    }
    return 0;
}
