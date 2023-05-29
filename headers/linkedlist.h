#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include<inttypes.h>
#include<stdlib.h>
#include<pthread.h>


typedef struct Node
{
    int key;
    int value;
    struct Node * next;
}Node;
typedef struct LinkedList
{
    pthread_mutex_t mutex;
    Node *  head;
}LinkedList;


// inserts new (key, value) pair to a list given by head.
void insert(int key, int value, LinkedList * list);

int get(int key, LinkedList *list);
size_t count(LinkedList *list);
#endif

