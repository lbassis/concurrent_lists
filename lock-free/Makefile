CC=gcc
CFLAGS=-Wall -std=c11
LDFLAGS=-lpthread


onelock: main.c set.c set.h
	$(CC) $(CFLAGS) $(LDFLAGS) set.c main.c -o main

clean:
	rm *.o *~
