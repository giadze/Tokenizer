#ifndef SORTED_LIST_H
#define SORTED_LIST_H
#include <stdbool.h>


typedef int functionCompare(void *, void *);  
typedef void functionDestroy(void *); 


typedef struct listElement 
{ 

    struct listElement *next;
    void *value;

} listElement;  


listElement *listElementCreation(void *, listElement *); 


typedef struct sortedList 
{

    functionCompare *function_compare;  
    functionDestroy *function_destroy; 
    listElement *head;

} sorted_list;  


sorted_list *listCreation(functionCompare *, functionDestroy *);  



typedef struct sortedListTraversal 
{
    listElement *currentCount; 

} listTraversal;  


listTraversal *createTraversal(sorted_list *);



void listDestroyer(sorted_list *); 

void listElementDestroyer(sorted_list *, listElement *); 

bool objectInsert(sorted_list *, void *); 

bool objectRemoval(sorted_list *, void *); 

bool contains(sorted_list *, void *); 

int getSize(sorted_list *); 

void traverserDestroyer(listTraversal *); 

void *getItem(listTraversal *); 

void *nextItem(listTraversal *); 

#endif
