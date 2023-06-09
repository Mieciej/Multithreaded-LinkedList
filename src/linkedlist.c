#include "linkedlist.h"
#include <stdio.h>



void insert(int key, int value, LinkedList *list)
{
    if(value == -1) return;
    pthread_mutex_lock(&list->mutex);
    Node *curr = list->head;
    while (curr->next != NULL)
    {
        if (curr->next->key == key)
        {
            curr->next->value = value;
            pthread_mutex_unlock(&list->mutex);

            return;
        }
        curr = curr->next;
    }
    Node *new_node = malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    curr->next = new_node;
    pthread_cond_broadcast(&list->new_item);
    pthread_mutex_unlock(&list->mutex);
}

int get(int key, LinkedList *list)
{
    pthread_mutex_lock(&list->mutex);
    Node * curr = list->head;
    while (curr->next!=NULL)
    {
        if(curr->next->key == key)
        {
            int val = curr->next->value;
            pthread_mutex_unlock(&list->mutex);
            return val;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&list->mutex);

    return -1;
    
}

size_t count(LinkedList *list)
{
    size_t n = 0;
    pthread_mutex_lock(&list->mutex);
    Node *  curr  = list->head;
    while (curr->next!=NULL)
    {
        n++;
        curr = curr->next;
    }
    pthread_mutex_unlock(&list->mutex);
    return n;
}

void del(int key, LinkedList * list)
{
    pthread_mutex_lock(&list->mutex);
    Node * curr = list->head;
    while (curr->next!=NULL)
    {
        if(curr->next->key == key)
        {
            Node *to_remove  = curr->next;
            curr->next = to_remove->next;
            free(to_remove);
            pthread_mutex_unlock(&list->mutex);
            return;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&list->mutex);

}





int poll(int key, LinkedList * list)
{
    int value = get(key, list);
    pthread_mutex_lock(&list->mutex);
    while (value == -1)
    {
        pthread_cond_wait(&list->new_item,&list->mutex);
        value = get(key, list);
    }
    pthread_mutex_unlock(&list->mutex);
    return value;
}



void start_iteration(LinkedList *list)
{
    pthread_mutex_lock(&list->mutex);
    pthread_mutex_lock(&list->iterator_mutex);
    list->iterator = list->head;
}
void end_iteration(LinkedList *list)
{
    list->iterator = NULL;
    int e = pthread_mutex_unlock(&list->iterator_mutex);
    if(e!=0)
    {
        fprintf(stderr,"Invalid end_iteration usage.\n");

        exit(0);
    }
    e = pthread_mutex_unlock(&list->mutex);
    if(e!=0)
    {
        fprintf(stderr,"Invalid end_iteration usage.\n");
        exit(0);
    }
}
Node* next(LinkedList *list)
{
    // fail safe measures
    int invalid = 0;
    pthread_mutex_lock(&list->mutex);
    pthread_mutex_lock(&list->iterator_mutex);

    if(list->iterator == NULL)
    {
        fprintf(stderr,"Invalid usage of next() function. Did not use start_iteration()\n");
        invalid = 1;
        start_iteration(list);
    }
    Node *next = list->iterator->next;
    list->iterator = next;
    if(invalid)
    {
        end_iteration(list);
    }
    pthread_mutex_unlock(&list->iterator_mutex);
    pthread_mutex_unlock(&list->mutex);

    return next;
}


void free_list(LinkedList *list)
{
    start_iteration(list);
    Node * curr;
    while ((curr = next(list))!=NULL)
    {
        free(curr);
    }
    end_iteration(list);   
    free(list->head);
    free(list);
}

LinkedList * initialise_list()
{
    LinkedList * list = malloc(sizeof(LinkedList));
    Node * head = malloc(sizeof(Node));
    head->key = 0 ;
    head->next=0;
    head->next =NULL;
    list->head =head;
    list->new_item = (pthread_cond_t)PTHREAD_COND_INITIALIZER; 
    pthread_mutexattr_t recursiveAttrs;
    pthread_mutexattr_init(&recursiveAttrs);
    pthread_mutexattr_settype(&recursiveAttrs, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&list->mutex, &recursiveAttrs);
    pthread_mutex_init(&list->iterator_mutex, &recursiveAttrs);
    return list;
}