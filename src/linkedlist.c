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
    pthread_cond_signal(&list->new_item);
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

    fprintf(stderr,"Failed to get key: %d\n", key);
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


int* iterate(LinkedList* list)
{
    size_t n = 0;
    int * keys = malloc(0);
    pthread_mutex_lock(&list->mutex);
    Node * curr = list->head;
    while (curr->next!=NULL)
    {
        n++;
        keys = realloc(keys,sizeof(int)*n);
        keys[n-1] = curr->next->key; 
        curr = curr->next;
    }
    pthread_mutex_unlock(&list->mutex);
    return keys;

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