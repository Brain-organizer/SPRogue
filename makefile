CC = gcc
CFLAGS = -lncursesw
SOURCES = ./src/*.c

.PHONY: clean

prerequisite:
		sudo apt install libncurses5-dev libncursesw5-dev
		sudo apt install xterm

build:
		$(CC) $(SOURCES) $(CFLAGS) -o rogue

run: 
		resize -s 40 115
		./rogue

clean:
		rm rogue