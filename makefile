CC=gcc
CFLAGS=-g -Wall -fsanitize=address -std=c99

spchk: spchk.c test.txt
	$(CC) $(CFLAGS) $^ -o $@
