# Makefile for compiling and running the server

# Declare phony targets
.PHONY: run server clean

# Compiler and flags
CC := gcc
CFLAGS := -Wall

# Command definitions
run: server
	./server

server: server.o
	$(CC) $(CFLAGS) -o server server.o

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

clean:
	rm -f server server.o

