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
    pthread_mutex_t iterator_mutex;
    pthread_cond_t new_item;
    Node *  head;
    Node *  iterator;
}LinkedList;


// Inserts new (key, value) pair to a list given by head. Value -1 is reserved and not possible to insert.
void insert(int key, int value, LinkedList * list);

// Get value for a given key, if not found return -1.
int get(int key, LinkedList *list);

// Return count of items in the list.
size_t count(LinkedList *list);

// Remove item from the list.
void del(int key, LinkedList * list);

// Request value of a given key, and wait until it apears in the list. 
int poll(int key, LinkedList * list);

// Iterate through array one by one, 
Node* next(LinkedList *list);

// Indicate the start of the iteration by a thread
void start_iteration(LinkedList *list);

// Close iteration by a thread
void end_iteration(LinkedList *list);

// Deallocate list.
void free_list(LinkedList *list);

// Initialise list.
LinkedList * initialise_list();
#endif

