#ifndef INDEXER_H
#define INDEXER_H
#include "sorted-list.h"

typedef struct indexer {

    sorted_list *entries;

} indexerStruct; 


typedef struct indexerRecord{

	char *filePath;
	int count;

}entryRecord;


typedef struct indexEntry {

		char *token;
		sorted_list *records;

} indexerInsert;



indexerStruct *indexerCreator();  
void indexerDestroyer(indexerStruct *);
bool runIndexerCreator(indexerStruct *, char *);

entryRecord *entryRecordCreator(char *);
void entryRecordDestroyer(entryRecord *);

indexerInsert *indexerEntryCreator(char *);
void indexerEntryDestroyer(indexerInsert *);

#endif
