// MUST USE POSIX read() write() open() close() etc.
// TODO:
// ./spchk ../dict ../testfile

// Read Dictionary File
// Fill Trie - Based on term "Retrieval"
// Locate Directory
// Read Directory Text Files
// Remove trailing punctuation
// Compare word against dictionary words
// Check hypenated words, one on each side
// Compare with capitalization if it does not match
// Report errors based on incorrect spelling

// Find and open all specified files including directory traversal
// Reading the file and generating a sequence of position annotated words
// Checking whether a word is contained in the dictionary

// Program Start
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NUM_CHAR 256        // Capital, Lowercase, space, new line

typedef struct trienode {
    struct trienode *children[NUM_CHAR];
    bool terminal;          // node is the end of a word
}trieNode;

// function to create a new node in the trie
// allocates space for the new node and the children to NULL values
trieNode * createNode() {
    trieNode *newNode = malloc(sizeof(*newNode));       // allocate space for a new node

    for(int i = 0; i < NUM_CHAR; i++) {             
        newNode->children[i] = NULL;                // initialize the new node to null characters
    }

    newNode->terminal = false;                      // new node will not be terminal by default
    return newNode;                                 // return the new node
}

// function to insert a new node into the trie
// false implies the node already exists
// true implies the node was inserted
bool insertTrie(trieNode **root, char *signedText) {
    if(*root == NULL) {
        *root = createNode();                           // Create a new root if one doesn't exist
    }

    unsigned char *text = (unsigned char*)signedText;   // Prevents negative index in table
    trieNode *tmp = *root;
    int length = strlen(signedText);

    for(int i = 0; i < length; i++) {
        if(tmp->children[text[i]] == NULL) {            // Create a new node if it does not exist
            tmp->children[text[i]] = createNode();
        }
        tmp = tmp->children[text[i]];                   // Moves the temp pointer to look at the appropriate child node
    }

    if(tmp->terminal) {
        return false;           // checks if the word already exists
    }
    else {
        tmp->terminal = true;   // there was a node for this word, make it terminal
        return true;
    }
}

// Recursive print function to print out the word in a trie
void printTrie_Recursive(trieNode *node, unsigned char *prefix, int length) {
    unsigned char newPrefix[length + 2];        // One more symbol and null character
    memcpy(newPrefix, prefix, length);          // Copies the previous word to the new prefix with two extra spaces
    newPrefix[length+1] = 0;                    // null terminate the string

    if(node->terminal) {                        // Base Case, word complete
        printf("WORD: %s\n", prefix);
    }

    for(int i = 0; i < NUM_CHAR; i++) {                                         // Traverse all possible nodes
        if(node->children[i] != NULL) {                                         // Check if a node has something worth checking
            newPrefix[length] = i;                                              // Sets the value to be checked
            printTrie_Recursive(node->children[i], newPrefix, length + 1);      // Recursively loop through the trie
        }
    }
}

// Function to print out the trie in ASCII-betical order
void printTrie(trieNode *root) {
    if(root == NULL) {                  // Special case for when the TRIE is empty
        printf("TRIE EMPTY\n");
        return;
    }

    printTrie_Recursive(root, NULL, 0); // Call to recursive print function
}

// Function to search if the word exists within the trie
// True implies the word exists
// False implies the word does not exist
bool searchTrie(trieNode *root, char *signedText) {
    unsigned char *text = (unsigned char*)signedText;       // Prevents negative indexes in the table
    int length = strlen(signedText);                        // How long is the word we are searching for
    trieNode *tmp = root;                                   // Temporary node, initialized to the root

    for(int i = 0; i < length; i++) {                       // Search trie character by character
        if(tmp->children[text[i]] == NULL) {                // Check if the ith character in the word exists
            return false;                                   // Return false if the word does not exist
        }
        tmp = tmp->children[text[i]];                       // Search the next node based on the next character
    }

    return tmp->terminal;                                   // Returns true only if the final node is terminal: the word exists and is not a substring of another word
}

/***************************************************************************************************/
/********************************THE ABOVE CODE WORKS AS INTENDED***********************************/
/***************************************************************************************************/



int main() {
    trieNode *root = NULL;
    insertTrie(&root, "KIT");
    insertTrie(&root, "CATTLE");
    insertTrie(&root, "KIN");
    insertTrie(&root, "CAT");
    insertTrie(&root, "HAPPY");

    printTrie(root);

    printf("search for CATTLE: %d\n", searchTrie(root, "CATTLE"));
    printf("search for LINK: %d\n", searchTrie(root, "LINK"));
    printf("search for HAPPY: %d\n", searchTrie(root, "HAPPY"));
    printf("search for CAT: %d\n", searchTrie(root, "CAT"));
    printf("search for KITTEN: %d\n", searchTrie(root, "KITTEN"));
    
    return EXIT_SUCCESS;
}