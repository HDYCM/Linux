#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_rwlock_t rwlock;
int count = 0;

void* writer(void* arg){
    (void)arg;
    while(1){
        pthread_rwlock_wrlock(&rwlock);
        ++count;
        printf("write:%d\n", count);
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
    return NULL;
}

void* reader(void* arg){
    (void)arg;
    while(1){
        pthread_rwlock_rdlock(&rwlock);
        printf("read:%d\n", count);
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
    return NULL;
}

int main(){
    pthread_t tid[8];
    pthread_rwlock_init(&rwlock, NULL);
    int i = 0;
    for(; i < 3; ++i){
        pthread_create(&tid[i], NULL, writer, NULL);
    }
    for(i = 0; i < 5; ++i){
        pthread_create(&tid[i], NULL, reader, NULL);
    }
    for(i = 0; i < 8; ++i){
        pthread_join(tid[i], NULL);
    }
    pthread_rwlock_destroy(&rwlock);
    return 0;
}
