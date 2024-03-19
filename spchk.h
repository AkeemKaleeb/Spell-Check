#ifndef SPCHK_H
#define SPCHK_H

#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

#define NUM_CHAR 256        // Capital, Lowercase, space, new line
#define BUFFER_SIZE 256     // Read buffer to reduce SYS Calls

#define DICTFILE "words.txt"
#define TEXTFILE "test.txt"
#define PATH_MAX 4096

typedef struct trienode
{
    struct trienode *children[NUM_CHAR];
    bool terminal; // node is the end of a word
} dictNode;

dictNode *createNode();
bool insertTrie(dictNode **root, char *signedText);
void printTrie(dictNode *root);
bool searchTrie(dictNode *root, char *signedText);
void freeTrie(dictNode *node);

bool fillDictionary(const char *dictPath, dictNode **root);
bool spellCheckFile(const char *textPath, dictNode *root);
int filesInDir(char *path, dictNode *root);

#endif /* SPCHK_H */
