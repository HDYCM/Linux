#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_cond_t cond;
pthread_mutex_t lock;

void* Pass(void* arg){
    (void)arg;
    while(1){
        printf("传球！\n");
        usleep(123456);
        pthread_cond_wait(&cond, &lock);
    }
    return NULL;
}

void* Shot(void* arg){
    (void)arg;
    while(1){
        pthread_cond_signal(&cond);
        printf("投篮！\n");
        usleep(789123);
    }
    return NULL;
}

int main(){
    //模拟蓝球训练场景
    //球员1，球员2
    //球员1负责传球，球员2负责投篮
    pthread_t tid1, tid2;
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&lock, NULL);
    pthread_create(&tid1, NULL, Pass, NULL);
    pthread_create(&tid2, NULL, Shot, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
}
