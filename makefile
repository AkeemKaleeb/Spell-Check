CC=gcc
CFLAGS=-g -Wall -fsanitize=address -std=gnu99

spchk: spchk.c
	$(CC) $(CFLAGS) $^ -o $@
