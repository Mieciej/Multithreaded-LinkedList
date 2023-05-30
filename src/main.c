#include "linkedlist.h"
#include <pthread.h>
#include <stdio.h>


void print_array(int* array, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%d ",array[i]);
    }
    printf("\n");
}



int main(int argc, char const *argv[])
{
    LinkedList list = {0};
    Node head = {.key = 0, .value=0, .next = NULL};
    list.head =&head;
    list.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER; 
    insert(1,1,&list);
    insert(2,32,&list);
    insert(3,31,&list);
    print_array(iterate(&list),count(&list));
    
    del(1,&list);
    print_array(iterate(&list),count(&list));

    return 0;
}

