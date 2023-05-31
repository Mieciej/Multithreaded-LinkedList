#include "linkedlist.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define MIN_KEY  -300
#define MAX_KEY  1000
#define MAX_VAL 100

void print_array(int* array, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%d ",array[i]);
    }
    printf("\n");
}

void * test_add(void * arg)
{
    while(1)
    {
        srand(clock());
        int key = (rand() % (MIN_KEY+MAX_KEY)) + MIN_KEY;
        int val = rand()%MAX_VAL;
        insert(key,val,(LinkedList*)arg);
    }
    
    return NULL;
}
void * test_iterate(void * arg)
{
    while(1)
    {
        sleep(5);
        int * key_value = iterate((LinkedList*)arg);
        for (size_t i = 0; i < key_value[0]; i++)
        {
            printf("(%d, %d)  ",key_value[i+1],key_value[i+1+key_value[0]]);
        }
        printf("\n");
        free(key_value);
    }
    
    return NULL;
}
void * test_remove(void * arg)
{
    while(1)
    {
        srand(clock());
        int key = (rand() % (MIN_KEY+MAX_KEY)) + MIN_KEY;
        del(key,(LinkedList*)arg);
    }
    
    return NULL;
}

void * test_poll(void * arg)
{
    while(1)
    {
        sleep(5);
        srand(clock());
        int key = (rand() % (MIN_KEY+MAX_KEY)) + MIN_KEY;
        printf("Polling: %d\n",key);
        int val = poll(key,(LinkedList*)arg);
        printf("(%d, %d)\n",key,val);
    }
    
    return NULL;
}
int main(int argc, char const *argv[])
{
    LinkedList list = {0};
    Node head = {.key = 0, .value=0, .next = NULL};
    list.head =&head;
    list.new_item = (pthread_cond_t)PTHREAD_COND_INITIALIZER; 
    pthread_mutexattr_t attr;
    pthread_mutexattr_t recursiveAttrs;
    pthread_mutexattr_init(&recursiveAttrs);
    pthread_mutexattr_settype(&recursiveAttrs, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&list.mutex, &recursiveAttrs);
    pthread_t t[4];

    pthread_create(&t[0],NULL,test_add,(void*)&list);
    pthread_create(&t[1],NULL,test_iterate,(void*)&list);
    pthread_create(&t[2],NULL,test_remove,(void*)&list);
    pthread_create(&t[3],NULL,test_poll,(void*)&list);


    for (size_t i = 0; i < 4; i++)
    {
        pthread_join(t[i], NULL);
    }
    
    return 0;
}

