CC = gcc
CFLAGS = -Wall -Wextra -lm

all: main

main: main.c vader_lexicon.c vader_lexicon.h
	$(CC) -o mySA main.c vader_lexicon.c $(CFLAGS)

clean: 
	rm -f mySA