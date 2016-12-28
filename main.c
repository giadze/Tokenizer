#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "indexer.h"


int main(int argc, char * argv[]) 
{

    if (argc != 3) 
	 {
		  printf("\n");
		  fprintf(stderr, "Wrong number of arguments, please use the following format\n"
				"./indexer then <inverted-index file name> and then <directory or file name>\n");
		  printf("\n");

        return EXIT_FAILURE;

    }
     else if (strcmp(argv[1], argv[2]) == 0) 

	 {
		  printf("\n");
        fprintf(stderr, "The file name you are trying to index to and test file have the same name\n");
		  printf("\n");

        return EXIT_FAILURE;

    }

    char *newFilePath = argv[1];
    char *inputPath = argv[2];

    FILE *newFile;
		
	
     if (access(newFilePath, F_OK) != -1) 
	 {

        printf("File already exists.\nEnter 1 to overwrite it, 2 to add to it anyway or 3 to cancel the operation.\n");

        fflush(stdout);

        int choice; 

        scanf("%d", &choice);

        if (choice == 3) return EXIT_SUCCESS;

        	newFile = fopen(newFilePath, choice == 1 ? "w" : "a");
		
    }
		
		else {

        newFile = fopen(newFilePath, "a+");
    }
    


    indexerStruct *indexer = indexerCreator();

    bool weAreGood = runIndexerCreator(indexer, inputPath); 

    if (weAreGood) {

        listTraversal *entries = createTraversal(indexer -> entries);

        indexerInsert *entry = getItem(entries);
        // JSON format 
        fprintf(newFile,"{ \"List\" : [ \n");
        int x = 1;
        while (entry != NULL) 
			{
				//JSON format
				
			if(x != 1 )
			{	
				
			fprintf(newFile, "           ]},\n");
			
			}
			else{
				x = 0;
			}
            fprintf(newFile, "           {\"%s\" :  [  \n\n", entry -> token);
            	listTraversal *records = createTraversal(entry -> records);
            		entryRecord *record = getItem(records);

            int counter = 0; 

            while (record != NULL) 	
			   {

                if (counter > 0) 
					 {

                    counter++;
                 

                }
					// JSON format
                fprintf(newFile, "                    {\"%s\" : %i}           \n\n", record -> filePath, record -> count);
                	record = nextItem(records);


                if (record != NULL) fprintf(newFile, " ");

	                counter++;
            }

            traverserDestroyer(records);
	            entry = nextItem(entries);
  		          
        }
        //JSON format
        fprintf(newFile, "           ]}\n");
        fprintf(newFile, "]}");
						
        traverserDestroyer(entries);

    } else {

        fprintf(stderr, "Parsing Error\n");

    }

    fclose(newFile);
	    indexerDestroyer(indexer);

	if(weAreGood == true)
	{
		printf("File has been successfully written.\n");
		return EXIT_SUCCESS;			

	} else {
		
		printf("Error writing the file\n");
		return EXIT_FAILURE;	
		
	}
}
