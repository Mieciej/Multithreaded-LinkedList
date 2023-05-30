#include "linkedlist.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void print_array(int* array, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%d ",array[i]);
    }
    printf("\n");
}

void * thread(void * arg)
{
    sleep(5);
    printf("Waking up...\n");
    insert(1,1,(LinkedList*)arg);
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
    pthread_t t;

    pthread_create(&t,NULL,thread,(void*)&list);
    pthread_detach(t);

    printf("%d\n",poll(1,&list));
    print_array(iterate(&list),count(&list));
    return 0;
}

