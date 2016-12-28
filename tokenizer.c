#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <tic.h>
#include "tokenizer.h"


/*Check whether character is valid*/
static bool valid(char c) 
{ 
    return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}


/*Function comparison fro tokens order doesn't matter*/
static int function_Compare(void *first, void *second) 
{ 
    return 1; 
}


/*Funtion destroyer for tokens*/
static void function_Destroyer(void *string) 
{ 
    free(string);
}


/*Memory is allocated and tokens copied when pointer to start is given and end as well*/
static char *tokCreat(char *indexStarter, char *indexCurr) 
{ 

    size_t sizeOfToken = indexCurr - indexStarter; 

    if (sizeOfToken == 0) 
	 {
        return NULL;
    }

    char *token = malloc(sizeOfToken + sizeof(char));
	    memcpy(token, indexStarter, sizeOfToken);
  		  token[sizeOfToken] = '\0';

    return token;
}


/*Tokenizeee and return token in list in order*/
sorted_list *tokenizer(char *string) 
{
    sorted_list *list = listCreation(&function_Compare, &function_Destroyer);

    if (list == NULL) 
	 {
        fprintf(stderr, "Error creating list! Not enough memory?\n");

        return NULL;
    }

    char *indexStarter = string; 
    char *indexCurr = string; 
    char *indexStop = string + strlen(string); 

    while (indexCurr != indexStop) 
	 {

        if (valid(*indexCurr)) 
		 {
            indexCurr = indexCurr + sizeof(char);

            if (indexCurr == indexStop) 
				{

                char *token = tokCreat(indexStarter, indexCurr);

                if (token != NULL) 
					 {
                    objectInsert(list, token);
                }
            }

        } else {

            char *token = tokCreat(indexStarter, indexCurr);

            if (token != NULL) 
				{
                objectInsert(list, token);
            }

            indexCurr = indexCurr + sizeof(char);
	            indexStarter = indexCurr;

        }
    }

    return list;

}
