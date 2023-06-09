#include "linkedlist.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#define MIN_KEY  0
#define MAX_KEY  10
#define MAX_VAL 100


volatile sig_atomic_t stopFlag = 0;
void ctrlC(int num) { stopFlag = 1; }




void * test_add(void * arg)
{
    while(!stopFlag)
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
    LinkedList *list = (LinkedList*)arg;
    while(!stopFlag)
    {
        sleep(5);
        start_iteration(list);
        Node * curr;
        while ((curr = next(list))!=NULL)
        {
            printf("(%d, %d)  ",curr->key,curr->value);
        }
        printf("\n");

        end_iteration(list);
        
    }
    
    return NULL;
}
void * test_remove(void * arg)
{
    while(!stopFlag)
    {
        srand(clock());
        int key = (rand() % (MIN_KEY+MAX_KEY)) + MIN_KEY;
        del(key,(LinkedList*)arg);
    }
    
    return NULL;
}



void * test_poll(void * arg)
{
    while(!stopFlag)
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
    LinkedList * list = initialise_list();
    pthread_t t[5];
    pthread_create(&t[0],NULL,test_add,(void*)list);
    pthread_create(&t[1],NULL,test_iterate,(void*)list);
    pthread_create(&t[2],NULL,test_remove,(void*)list);
    pthread_create(&t[3],NULL,test_poll,(void*)list);
    pthread_create(&t[4],NULL,test_poll,(void*)list);

    signal(SIGINT, ctrlC); 
    while (!stopFlag);

    // to unlock any stuck poll()
    for (size_t i = MIN_KEY; i < MAX_KEY; i++)
    {
        insert(i,0,list);
    }
    
    for (size_t i = 0; i < 5; i++)
    {
        pthread_join(t[i], NULL);
    }
    return 0;
}

