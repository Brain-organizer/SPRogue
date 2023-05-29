CC = gcc
CFLAGS = -lpthread -g
SOURCES = ./src/*.c

.PHONY: run clean clear_rank

build:
		$(CC) $(SOURCES) $(CFLAGS) -o server

run:
		./server

clean:
		rm server

clear_rank:
		./rank/ranking_clear
