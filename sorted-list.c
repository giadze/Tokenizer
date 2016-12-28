#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sorted-list.h"


/*Sorted list creator*/
sorted_list *listCreation(functionCompare *func_comp, functionDestroy *func_destr) {  

    sorted_list *list = malloc(sizeof(sorted_list));

    if (list == NULL) 
	 {
        return NULL;
    }

    list -> function_compare = func_comp;
    	list -> function_destroy = func_destr;
    		list -> head = NULL;

    return list;
}


/*Sorted list destroyer*/
void listDestroyer(sorted_list *list) 
{

    listElement *index = list -> head;

   	 while (index != NULL) {

        listElement *next = index -> next;
        	 list -> function_destroy(index -> value);
        		free(index);
        			index = next;

    }

    free(list);
}


/*List element destroyer*/
void listElementDestroyer(sorted_list *list, listElement *element) 
{
    list -> function_destroy(element -> value);
	    free(element);
}


/*List element creator*/
listElement *listElementCreation(void *value, listElement *next) 
{

    listElement *element = malloc(sizeof(listElement));

    if (element == NULL) 
	 {
        return NULL;
    }


    element -> value = value;
   	 element -> next = next;

   		 return element;
}


/*Insert after given index*/
static void insAfter(listElement *index, listElement *element) 
{ 

    listElement *tmp = index -> next;
    	element -> next = tmp;
    		index -> next = element;

}


/*Inserts element before given index*/
static bool insBefore(sorted_list *list, listElement *index, listElement *element) { 

    if (index == list -> head) 
	 {

        element -> next = list -> head;
	        list -> head = element;

    } else {

        listElement *tmp = list -> head;


        while (tmp != NULL) 
		  {

            if (tmp -> next == index) 
				{

                insAfter(tmp, element);
                return true;

            }

            tmp = tmp -> next;
        }
    }

    return false;
}


/*Object inserter*/
bool objectInsert(sorted_list *list, void *obj) 
{ 

    if (list -> head == NULL) 
	 {

        list -> head = listElementCreation(obj, NULL);

    } else {

        listElement *index = list -> head;

        while (true) 
		  {

            int result = list -> function_compare(obj, index -> value);

            switch (result) {

                case 0:

                    printf("Error! Cannot insert duplicate values.\n");
                    return true;

                case 1:

                    insBefore(list, index, listElementCreation(obj, NULL));

                    return true;

                case -1:

                default:
                    break;
            }

            if (index -> next == NULL) 
				{

                insAfter(index, listElementCreation(obj, NULL));

                return 1;

            }

            index = index -> next;
        }
    }

    return false;
}


/*Object remover fromlist*/
bool objectRemoval(sorted_list *list, void *obj) 
{ 

    listElement *index = list -> head;

    if (index -> value != NULL) 
	{

        if (list -> function_compare(index -> value, obj) == 0) 
		  {

            list -> head = index -> next;
            	listElementDestroyer(list, index);

            return true;

        }
    }

    while (index != NULL) 
	 {

        if (index -> next != NULL && index -> next -> value != NULL) 
		  {
            listElement *current_element = index -> next;

            if (list -> function_compare(index -> next -> value, obj) == 0) 
				{

                index -> next = index -> next -> next;
	                listElementDestroyer(list, current_element);

                return true;

            }
        }

        index = index -> next;
    }

    return false;
}


/*Check if given object is contained in list*/
bool contains(sorted_list *list, void *obj) 
{ 

    listElement *index = list -> head;

    while (index != NULL) 
	 {

    if (index -> value != NULL && list -> function_compare(index -> value, obj) == 0) 
		  {
            return true;
        }

        index = index -> next;
    }

    return false;
}


/*Size of the list getter*/
int getSize(sorted_list *list) 
{

    listElement *element = list -> head;

    int size = 0;

    while (element != NULL) 
	 {

        element = element -> next;
	        size++;
    }

    return size;
}


/*Iterator*/
listTraversal *createTraversal(sorted_list *list) 
{
    listTraversal *iterator = malloc(sizeof(listTraversal));


    if (list == NULL) 
	 {

        printf("Error creating iterator!\n");

        return NULL;
    }

    iterator -> currentCount = list -> head;

    return iterator;
}


/*Traverser destroyer*/
void traverserDestroyer(listTraversal *iterator) 
{
    free(iterator);
}


/*Gets current item in the lis*/
void *getItem(listTraversal *iterator) 
{

    if (iterator -> currentCount != NULL) 
	 {

        return iterator -> currentCount -> value;
    }

    return NULL;
}


/*Get next item in list*/
void *nextItem(listTraversal *iterator) 
{

    listElement *next_element = iterator -> currentCount -> next;

    if (next_element != NULL) 
	 {

        iterator -> currentCount = next_element;
        return next_element -> value;

    }

    return NULL;
}
