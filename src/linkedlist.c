#include "linkedlist.h"
#include <stdio.h>




void insert(int key, int value, LinkedList *list)
{
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
    pthread_mutex_unlock(&list->mutex);
}

int get(int key, LinkedList *list)
{
    Node * curr = list->head;
    pthread_mutex_lock(&list->mutex);
    while (curr->next!=NULL)
    {
        if(curr->next->key == key)
        {
            pthread_mutex_unlock(&list->mutex);
            return curr->next->value;
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