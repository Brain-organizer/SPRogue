CC = gcc
CFLAGS = -lncursesw
SOURCES = ./src/*.c

.PHONY: run clean

prerequisite:
		sudo apt install xterm

build:
		$(CC) $(SOURCES) $(CFLAGS) -o rogue

run: 
		resize -s 40 120
		./rogue 2> logs/err

clean:
		rm rogue