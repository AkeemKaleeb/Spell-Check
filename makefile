CC=gcc
CFLAGS=-g -Wall -fsanitize=address -std=c99

all: spchk myspchk spchk_test

spchk_test: spchk_test.c spchk.c
	$(CC) $(CFLAGS) $^ -o $@
	
spchk: spchk.c
	$(CC) $(CFLAGS) $^ -o $@

myspchk: myspchk.c
	$(CC) $(CFLAGS) $^ -o $@
