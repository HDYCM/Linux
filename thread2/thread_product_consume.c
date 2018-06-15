#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#if 0
pthread_mutex_t g_lock;
pthread_cond_t g_cond;

//先实现交易场所，使用一个链表来实现
//带头结点不带环的单向链表
//此时我们的链表就是一个栈
typedef struct Node{
    int data;
    struct Node* next;
}Node;

Node g_head;

Node*CreateNode(int value){
    Node* ptr = (Node*)malloc(sizeof(Node));
    ptr->data = value;
    ptr->next = NULL;
    return ptr;
}

void DestoryNode(Node* ptr){
    free(ptr);
}

void Init(Node* head){
    head->data = 0;
    head->next = NULL;
}

void Push(Node* head, int value){
    if(head == NULL){
        return;
    }
    Node* new_node = CreateNode(value);
    new_node->next = head->next;
    head->next = new_node;
    return;
}

void Pop(Node* head, int* value){
    if(head == NULL){
        return;
    }
    if(head->next == NULL){
        //链表为空
        return;
    }
    Node* to_delete = head->next;
    head->next = to_delete->next;
    *value = to_delete->data;
    DestoryNode(to_delete);
}

//再实现来那个两种角色
void* Product(void* arg){
    (void)arg;
    int count = 0;
    while(1){
        pthread_mutex_lock(&g_lock);
        Push(&g_head, ++count);
        printf("Product %d\n", count);
        pthread_mutex_unlock(&g_lock);
        usleep(123456);
        pthread_cond_signal(&g_cond);
    }
    return NULL;
}

void* Consume(void* arg){
    (void)arg;
    int count = -1;
    while(1){
        pthread_mutex_lock(&g_lock);
        //pthread_cond_wait 有可能被信号打断
        //当我们的pthread_cond_wait返回的时候，再次判断条件是否就绪
        while(g_head.next == NULL){
            pthread_cond_wait(&g_cond, &g_lock);
        }
        Pop(&g_head, &count);
        printf("Consume %d\n", count);
        usleep(789123);
        pthread_mutex_unlock(&g_lock);
    }
    return NULL;
}

int main(){
    pthread_cond_init(&g_cond, NULL);
    pthread_mutex_init(&g_lock, NULL);
    Init(&g_head);
    pthread_t product, consume;
    pthread_create(&product, NULL, Product, NULL);
    pthread_create(&consume, NULL, Consume, NULL);
    pthread_join(product, NULL);
    pthread_join(consume, NULL);
    pthread_mutex_destroy(&g_lock);
    pthread_cond_destroy(&g_cond);
    return 0;
}
#endif

#if 1
////////////////////////////////////////////////////////////
//基于信号量实现生产者消费者模型
//POSIX版本的信号量
////////////////////////////////////////////////////////////
#include <semaphore.h>

sem_t g_lock;
sem_t g_data;
sem_t g_blank;

//先实现一个交易场所
//使用数组实现一个队列
#define SIZE 1024
typedef struct Queue{
    int array[SIZE];
    int head;
    int tail;
    int size;
}Queue;

Queue g_queue;

void Init(Queue* q){
    q->head = q->tail = q->size = 0;
}

void Push(Queue* q, int value){
    if(q->size >= SIZE){
        return;
    }
    q->array[q->tail++] = value;
    if(q->tail >= SIZE){
        q->tail = 0;
    }
    ++q->size;
}

void Pop(Queue* q, int * value){
    if(q->size == 0){
        return;
    }
    *value = q->array[q->head++];
    if(q->head >= SIZE){
        q->head = SIZE;
    }
    --q->size;
}

//再实现来那个两种角色
void* Product(void* arg){
    (void)arg;
    int count = 0;
    while(1){
        sem_wait(&g_blank);

        sem_wait(&g_lock);
        Push(&g_queue, ++count);
        printf("Product %d\n", count);
        sem_post(&g_lock);

        usleep(789123);
        sem_post(&g_data);
    }
    return NULL;
}

void* Consume(void* arg){
    (void)arg;
    int count = -1;
    while(1){
        sem_wait(&g_data);

        sem_wait(&g_lock);
        Pop(&g_queue, &count);
        printf("Consume %d\n", count);
        sem_post(&g_lock);

        usleep(123456);
        sem_post(&g_blank);
    }
    return NULL;
}

//再实现三种关系
int main(){
    Init(&g_queue);
    sem_init(&g_lock, 0, 1);
    sem_init(&g_data, 0, 0);
    sem_init(&g_blank, 0, SIZE);
    pthread_t product, consume;
    pthread_create(&product, NULL, Product, NULL);
    pthread_create(&consume, NULL, Consume, NULL);
    pthread_join(product, NULL);
    pthread_join(consume, NULL);
    sem_destroy(&g_lock);
    sem_destroy(&g_data);
    sem_destroy(&g_blank);
    return 0;
}

#endif
