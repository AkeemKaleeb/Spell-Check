CC=gcc
CFLAGS=-g -Wall -fsanitize=address -std=c99

#spchk: spchk.c
#	$(CC) $(CFLAGS) $^ -o $@

SPC: spellCheckTries.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) -r $(BINS) *.dSYM