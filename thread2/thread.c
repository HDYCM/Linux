#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

pthread_mutex_t g_lock;
int g_count = 0;

int main(){
    signal(SIGINT, MyHandler);
    pthread_mutex_init(&g_lock, NULL);
    ThreadEntry();
    pthread_mutex_destroy(&g_lock);
    return 0;
}

