# Makefile for compiling and running the server

CC := gcc
CFLAGS := -Wall

run: server
	./server

server: server.o
	$(CC) $(CFLAGS) -o server server.o

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

clean:
	rm -f server server.o

