#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int g_count = 0;

//void* ThreadEntry1(void* arg){
//    (void)arg;
//    while(1){
//        ++g_count;
//        sleep(1);
//    }
//    return NULL;
//}
//
//void* ThreadEntry2(void* arg){
//    (void)arg;
//    while(1){
//        printf("g_count = %d\n", g_count);
//        sleep(1);
//    }
//    return NULL;
//}

void* ThreadEntry1(void* arg){
    int* p = (int*)arg;
    while(1){
        ++*p;
        sleep(1);
    }
    return NULL;
}

void* ThreadEntry2(void* arg){
    int* p = (int*)arg;
    while(1){
        printf("count = %d\n", *p);
        sleep(1);
    }
    return NULL;
}

int main(){
    pthread_t tid1, tid2;
    int* p = (int*)malloc(sizeof(int));
    pthread_create(&tid1, NULL, ThreadEntry1, p);
    pthread_create(&tid2, NULL, ThreadEntry2, p);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
