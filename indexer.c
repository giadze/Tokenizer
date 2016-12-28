#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include "tokenizer.h"
#include "indexer.h"


/*Given the path of the file, creates indexer record*/
entryRecord *entryRecordCreator(char *filePath) 
{

    entryRecord *record = malloc(sizeof(entryRecord));

    	record -> filePath = malloc(strlen(filePath) + sizeof(char));

    		strcpy(record -> filePath, filePath);

    			record -> count = 0;

    				return record;
}


/*Destroys an indexer entry record*/
void entryRecordDestroyer(entryRecord *record) 
{
    free(record -> filePath);

    	free(record);
}


/*Given the path of the file, it gets the index entry record, else null*/
static entryRecord *retrieveEntryRecord(indexerInsert *entry, char *filePath)  
{

    listTraversal *iterator = createTraversal(entry -> records);

    	entryRecord *record = getItem(iterator);

    		while (record != NULL) {

        		if (strcmp(filePath, record -> filePath) == 0) 
		  {
	            return record;
        }
	
        record = nextItem(iterator);
    }

    return NULL;
}


/*Entry record comparison function*/
static int storeCompFunction(void *first, void *second) 
{ 

    entryRecord *firstStore = first; 

    	entryRecord *secondStore = second; 
	
   // int value1 = firstStore -> count; 
    //int value2 = secondStore -> count; 

    //printf("%i -> %i\n", value1, value2);

    	return firstStore -> count > secondStore -> count ? 1 : -1;
}


/*Function destroyer for entry records which are inside the list */
static void storeFunctionDestroyer(void *object) 
{
    entryRecordDestroyer(object);
}


/*Indexer entry creator, memory is freed by the caller*/
indexerInsert *indexerEntryCreator(char *token) 
{

    indexerInsert *entry = malloc(sizeof(indexerInsert));

    	entry -> records = listCreation(&storeCompFunction,
            &storeFunctionDestroyer);


      entry -> token = malloc(strlen(token) + sizeof(char));
    			strcpy(entry -> token, token);

  		return entry;
}

/*Indexer entry destroyer*/
void indexerEntryDestroyer(indexerInsert *entry) 
{

    listDestroyer(entry -> records);

    	free(entry -> token);

    		free(entry);

}


/*When given the token, this gets the index entry, else null*/
static indexerInsert *getEntry(indexerStruct *indexer, char *token) 
{  

    listTraversal *iterator = createTraversal(indexer -> entries);
    	indexerInsert *entry = getItem(iterator);

  		  while (entry != NULL) {
	
       	 if (strcmp(token, entry -> token) == 0) {

            return entry;
        }
	
        entry = nextItem(iterator);
    }

    return NULL;
}


/*Indexer entry comparison function*/
static int insertCompFunction(void *first, void *second)  
{
	
    indexerInsert *firstInsert = first; 

    	indexerInsert *secondInsert = second; 

    		return strcmp(firstInsert -> token, secondInsert -> token) > 0 ? -1 : 1;

}


/*Function destroyer for indexer entries*/
static void insertFunctionDestroyer(void *object) 
{
    indexerEntryDestroyer((indexerInsert *) object);
}


/*Indexer creator, memory freed by caller*/
indexerStruct *indexerCreator() 
{
    indexerStruct *indexer = malloc(sizeof(indexerStruct));

   	 indexer -> entries = listCreation(&insertCompFunction,

            &insertFunctionDestroyer);

    				return indexer;
}


/*Indexer destroyer*/
void indexerDestroyer(indexerStruct *indexer) 
{
    listDestroyer(indexer -> entries);

   	 free(indexer);
}


/*Self explanetory*/
static int toLower(int c) 
{

    if (c >= 'A' && c <= 'Z') 
	 {
        c = c - 'A';
        c = c + 'a';
    }

    return c;

}


/*Given the path to the file, it reads the contents of it and returns as string, error if null*/
static char *read(char *filePath)  
{

    FILE *file = fopen(filePath, "r");

   	 if (file != NULL) 
	{
        struct stat fileStatus; 


        if (fstat(fileno(file), &fileStatus) == 0) 
		{

            off_t sizeOfFile = fileStatus.st_size; 


            if (sizeOfFile > 0) 
				{

                char *data = malloc((size_t) sizeOfFile + sizeof(char));
                char *currIndex = data; 
                int current;


	                while ((current = fgetc(file)) != EOF) 
					 {
  		                  sprintf(currIndex, "%c", toLower(current));
     			               currIndex = currIndex + sizeof(char);
                }

                sprintf(data + sizeOfFile, "%c", '\0');

                	fclose(file);

                		return data;

            } else {

                fclose(file);

                char *t = malloc(sizeof(char)); 
                *t = '\0';

                return t;
            }

        } else {

            fprintf(stderr, "Problem getting the status for file.\n");
            fclose(file);

        }

    } else {

	        fprintf(stderr, "There was a problem with opening a file.\n");

    }

    return NULL;
}


/*Creates an entry if there is one alread in the indexer, checks for an instance 
 of a record already in indexer and if there is not one, creates a new one */
static void tokenHandler(indexerStruct *indexer, char *filePath, char *token)  
{ 

    indexerInsert *entry = getEntry(indexer, token);

    if (entry == NULL) 
	 {

        entry = indexerEntryCreator(token);
   	     objectInsert(indexer -> entries, entry);

    }

    entryRecord *record = retrieveEntryRecord(entry, filePath);


	    if (record == NULL) 
	 {

        record = entryRecordCreator(filePath);
	        objectInsert(entry -> records, record);

    }

    record -> count++;
}


/*File parser when indexer is given along with path of the file. It also updates and adds entries for that indexer*/
static bool fileParser(indexerStruct *indexer, char *filePath) 
{

    char *filData = read(filePath); 
	
  	  if (filData == NULL) 
	 {
        return false;
    }

    sorted_list *listOfTokens = tokenizer(filData); 
	    listTraversal *iterator = createTraversal(listOfTokens);

  			  if (iterator != NULL) 
	 {

        char *token = getItem(iterator);
	
  		      while (token != NULL) 
	     {
     		       tokenHandler(indexer, filePath, token);
        			    token = nextItem(iterator);
        }

        traverserDestroyer(iterator);
    }

    listDestroyer(listOfTokens);
	    free(filData);

    return true;
}


/*When path is given, files are iterated in a directory, entries are updates and addes for indexer given*/
static bool directTraver(indexerStruct *indexer, char *path)  
{ 

    DIR *directory = opendir(path);

    if (directory != NULL) 
	 {

        struct dirent *currFile; 

	        while ((currFile = readdir(directory)) != NULL) 
		  {

            char *fileName = currFile -> d_name; 

	            if (strcmp(fileName, ".") == 0 || strcmp(fileName, "..") == 0) 
				{
                continue;
            }

            size_t pathSize = strlen(path) + strlen(fileName) + (2 * sizeof(char)); 
            char *filePath = malloc(pathSize);

            strcpy(filePath, path);
            	strcat(filePath, "/");
            		strcat(filePath, fileName);


            if (currFile -> d_type == DT_DIR) 
				{

                directTraver(indexer, filePath);


            } else if (currFile -> d_type == DT_REG) 

				{

                fileParser(indexer, filePath);
            }

            free(filePath);

        }

        closedir(directory);
        return true;

    } else {

        return false;

    }

}


/*Indexer is ran when the path to directory is given to traverse or to parse a file*/
bool runIndexerCreator(indexerStruct *indexer, char *path) 
{ 

    if (!directTraver(indexer, path) && !fileParser(indexer, path)) 
	 {
        return false;

    }

    return true;
}
