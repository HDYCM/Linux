#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

pthread_mutex_t g_lock;
int g_count = 0;

//Func函数线程安全，不可重入
void Func(){
    pthread_mutex_lock(&g_lock);
    printf("lock!\n");
    ++g_count;
    sleep(3);
    printf("unlock!\n");
    pthread_mutex_unlock(&g_lock);
}

void* ThreadEntry(void* arg){
    (void)arg;
    while(1){
        Func();
    }
    return NULL;
}

void MyHandler(int sig){
    (void)sig;
    Func();
}

int main(){
    signal(SIGINT, MyHandler);
    pthread_mutex_init(&g_lock, NULL);
    ThreadEntry(&g_count);
    pthread_mutex_destroy(&g_lock);
    return 0;
}

