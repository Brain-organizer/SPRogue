CC = gcc
CFLAGS = -lncurses
SOURCES = ./src/*.c

all: rogue run clean rebuild

rogue: 
		$(CC) $(SOURCES) $(CFLAGS) -o rogue

run:
		./rogue 2> logs/err

clean:
		rm rogue

rebuild:
		$(CC) $(SOURCES) $(CFLAGS) -o rogue