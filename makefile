CC = gcc
CFLAGS = -lncursesw
SOURCES = ./src/*.c

.PHONY: run clean

build:
		$(CC) $(SOURCES) $(CFLAGS) -o rogue

run:
		./rogue 2> logs/err

clean:
		rm rogue