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
    printf("Thread2\n");
    pthread_exit((void*)2);
    return NULL;
}

void* ThreadEntry3(void* arg){
    (void)arg;
    printf("Thread3\n");
    pthread_cancel(pthread_self());
    return NULL;
}

//创建三个线程，分别使用不同的方式终止线程
//通过线程等待来观察线程返回的结果
int main(){
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, ThreadEntry1, NULL);
    pthread_create(&tid2, NULL, ThreadEntry2, NULL);
    pthread_create(&tid3, NULL, ThreadEntry3, NULL);
    void* ret = NULL;
    pthread_join(tid1, &ret);
    printf("thread1 ret = %p\n", ret);
    pthread_join(tid2, &ret);
    printf("thread2 ret = %p\n", ret);
    pthread_join(tid3, &ret);
    printf("thread3 ret = %p\n", ret);
    return 0;
}
