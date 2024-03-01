CC=gcc
CFLAGS=-g -Wall -fsanitize=address -std=c99

spchk: spchk.c
	$(CC) $(CFLAGS) -o $@ $^

#.PHONY: run
#run: spchk
#	./spchk "words.txt" "test.txt"

#.PHONY: clean
#clean:
#	rm -f spchk
