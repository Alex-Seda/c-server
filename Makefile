# Makefile for compiling and running the server

# Declare phony targets
.PHONY: run server clean

# Compiler and flags
CC := gcc
CFLAGS := -Wall

SERVICES := $(wildcard services/*.c)

SOURCES = server.c $(SERVICES)

OBJECTS = $(SOURCES:.c=.o)

TARGET = server

# Command definitions
run: $(TARGET)
	./$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJECTS)
	find . -name "*.o" -delete

