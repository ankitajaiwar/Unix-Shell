

CFLAGS=-Wall -std=c99 -pedantic 
CC = gcc






myshell: myshell.o myls.o

	gcc -o myshell myshell.o  $(CFLAGS) 
	gcc -o myls myls.o $(CFLAGS) 


	
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@







clean:

	rm -f myshell

	rm -f myls