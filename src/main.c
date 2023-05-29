#include "linkedlist.h"
#include <pthread.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
    LinkedList list = {0};
    Node head = {.key = 0, .value=0, .next = NULL};
    list.head =&head;
    list.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER; 
    insert(1,1,&list);
    insert(2,32,&list);
    insert(3,31,&list);
    printf("%d\n",get(1,&list));
    printf("%d\n",get(3,&list));
    printf("%d\n",get(2,&list));

    return 0;
}

