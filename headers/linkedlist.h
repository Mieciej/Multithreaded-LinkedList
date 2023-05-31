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
    pthread_cond_t new_item;
    Node *  head;
}LinkedList;


// Inserts new (key, value) pair to a list given by head. Value -1 is reserved and not possible to insert.
void insert(int key, int value, LinkedList * list);

// Get value for a given key, if not found return -1.
int get(int key, LinkedList *list);

// Return count of items in the list.
size_t count(LinkedList *list);

// Remove item from the list.
void del(int key, LinkedList * list);

// Iterate through all elements of the list and return all keys.
int* iterate(LinkedList* list);


// Request value of a given key, and wait until it apears in the list. 
int poll(int key, LinkedList * list);

#endif

