CC=gcc
CFLAGS= -Wall -O -g

indexer: main.c sorted-list.o tokenizer.o indexer.o
	$(CC) $(CFLAGS) main.c sorted-list.o tokenizer.o indexer.o -o indexer

indexer.o: indexer.c indexer.h
	$(CC) $(CFLAGS) -c indexer.c

sorted-list.o: sorted-list.c sorted-list.h
	$(CC) $(CFLAGS) -c sorted-list.c

tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.c

clean:
	rm -rf *.o indexer
