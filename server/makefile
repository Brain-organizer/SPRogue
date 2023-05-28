CC = gcc
CFLAGS = -lpthread
SOURCES = ./src/*.c

.PHONY: run clean

build:
		$(CC) $(SOURCES) $(CFLAGS) -o server

run:
		./server

clean:
		rm server