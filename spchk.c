// MUST USE POSIX read() write() open() close() etc.
// TODO:
// ./spchk ../dict ../testfile

// DONE: Read Dictionary File
// DONE: Fill Trie - Based on term "Retrieval"
// Locate Directory
// DONE: Read Directory Text Files
// DONE: Remove trailing punctuation
// DONE: Compare word against dictionary words
// DONE: Check hypenated words, one on each side
// Compare with capitalization if it does not match
// DONE: Report errors based on incorrect spelling
// Include line and column number

// Find and open all specified files including directory traversal
// Reading the file and generating a sequence of position annotated words
// Checking whether a word is contained in the dictionary

// Program Start
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define NUM_CHAR 256            // Capital, Lowercase, space, new line
#define BUFFER_SIZE 1024        // Read buffer to reduce SYS Calls

#define DICTFILE "words.txt"
#define TEXTFILE "test.txt"

// #region TRIE

typedef struct trienode
{
    struct trienode *children[NUM_CHAR];
    bool terminal; // node is the end of a word
} dictNode;

// function to create a new node in the trie
// allocates space for the new node and the children to NULL values
dictNode *createNode()
{
    dictNode *newNode = malloc(sizeof(*newNode)); // allocate space for a new node

    for (int i = 0; i < NUM_CHAR; i++)
    {
        newNode->children[i] = NULL; // initialize the new node to null characters
    }

    newNode->terminal = false; // new node will not be terminal by default
    return newNode;            // return the new node
}

// function to insert a new node into the trie
// false implies the node already exists
// true implies the node was inserted
bool insertTrie(dictNode **root, char *signedText)
{
    if (*root == NULL)
    {
        *root = createNode(); // Create a new root if one doesn't exist
    }

    unsigned char *text = (unsigned char *)signedText; // Prevents negative index in table
    dictNode *tmp = *root;
    int length = strlen(signedText);

    for (int i = 0; i < length; i++)
    {
        if (tmp->children[text[i]] == NULL)
        { // Create a new node if it does not exist
            tmp->children[text[i]] = createNode();
        }
        tmp = tmp->children[text[i]]; // Moves the temp pointer to look at the appropriate child node
    }

    if (tmp->terminal)
    {
        return false; // checks if the word already exists
    }
    else
    {
        tmp->terminal = true; // there was a node for this word, make it terminal
        return true;
    }
}

// Recursive print function to print out the word in a trie
void printTrie_Recursive(dictNode *node, unsigned char *prefix, int length, int *wordNumber)
{
    unsigned char newPrefix[length + 2]; // One more symbol and null character
    memcpy(newPrefix, prefix, length);   // Copies the previous word to the new prefix with two extra spaces
    newPrefix[length + 1] = 0;           // null terminate the string

    if (node->terminal)
    { // Base Case, word complete
        printf("WORD [%d]: %s\n", *wordNumber, prefix);
        (*wordNumber)++;
    }

    for (int i = 0; i < NUM_CHAR; i++)
    { // Traverse all possible nodes
        if (node->children[i] != NULL)
        {                                                                               // Check if a node has something worth checking
            newPrefix[length] = i;                                                      // Sets the value to be checked
            printTrie_Recursive(node->children[i], newPrefix, length + 1, wordNumber);  // Recursively loop through the trie
        }
    }
}

// Function to print out the trie in ASCII-betical order
void printTrie(dictNode *root)
{
    if (root == NULL)
    { // Special case for when the TRIE is empty
        printf("TRIE EMPTY\n");
        return;
    }

    int wordNumber = 1;     // Start with the first word
    printTrie_Recursive(root, NULL, 0, &wordNumber); // Call to recursive print function
}

// Function to search if the word exists within the trie
// True implies the word exists
// False implies the word does not exist
bool searchTrie(dictNode *root, char *signedText)
{
    unsigned char *text = (unsigned char *)signedText; // Prevents negative indexes in the table
    int length = strlen(signedText);                   // How long is the word we are searching for
    dictNode *tmp = root;                              // Temporary node, initialized to the root

    for (int i = 0; i < length; i++)
    { // Search trie character by character
        if (tmp->children[text[i]] == NULL)
        {                 // Check if the ith character in the word exists
            return false; // Return false if the word does not exist
        }
        tmp = tmp->children[text[i]]; // Search the next node based on the next character
    }

    return tmp->terminal; // Returns true only if the final node is terminal: the word exists and is not a substring of another word
}

// Function to recursively free the Trie
void freeTrie(dictNode *node) {
    if(node == NULL) {
        return;
    }

    for(int i = 0; i < NUM_CHAR; i++) {
        freeTrie(node->children[i]);
    }

    free(node);
} 

// #endregion

// #region FILES

// Function to fill the dictionary Trie
// True implies dictionary was filled successfully
// False implies dictionary was not filled successfully
bool fillDictionary(const char* dictPath, dictNode **root) {
    int dictionary_FD = open(dictPath, O_RDONLY);               // Open the dictionary in read only mode

    if(dictionary_FD == -1) {                                   // Prints an error if the dictionary cannot be opened
        perror("Dictionary could not be opened!\n");
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];           // Buffer to reduce sys calls
    int bytesRead;                      // number of bytes read so far
    char word[BUFFER_SIZE];             // Buffer to store a single word
    int wordLength = 0;                 // Length of the word stored

    while((bytesRead = read(dictionary_FD, buffer, BUFFER_SIZE)) > 0) {     // Loop through the file
        for(int i = 0; i < bytesRead; i++) {    // Loop through the word
            if(buffer[i] == '\n') {             // End of word, insert to Trie
                word[wordLength] = '\0';        // Terminate the word
                if(wordLength > 0) {
                    insertTrie(root, word);     // Insert complete word to Trie
                    wordLength = 0;             // Reset word length for next word
                }
            }
            else {
                word[wordLength++] = buffer[i];     // Append the next character to the word
            }
        }
    }

    close(dictionary_FD);

    return true;
}

// Function that checks an individual text file's words against a provided dictionary
bool checkFile(const char* textPath, dictNode *root) {
    int text_FD = open(textPath, O_RDONLY);                 // Open the dictionary in read only mode

    if(text_FD == -1) {                                     // Prints an error if the text file cannot be opened
        perror("Dictionary could not be opened!\n");
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];           // Buffer to reduce sys calls
    int bytesRead;                      // number of bytes read so far
    char word[BUFFER_SIZE];             // Buffer to store a single word
    int wordLength = 0;                 // Length of the word stored

    int row = 1;    // Row number of error
    int col = 0;    // Column number of error

    while((bytesRead = read(text_FD, buffer, BUFFER_SIZE)) > 0) {       // Loop through the file
        for(int i = 0; i < bytesRead; i++) {    // Loop through the word
            if(buffer[i] == '\n') {             // Increment row counter every new line
                row++;
                col = 1;
            }
            else {
                col++;
            }
            
            // End of word, insert to Trie
            if(!(('a' <= buffer[i] && 'z' >= buffer[i]) ||
                ('A' <= buffer[i] && 'Z' >= buffer[i]))) 
            {             
                word[wordLength] = '\0';        // Terminate the word
                if(wordLength > 0) {
                    bool wordExists = searchTrie(root, word);       // Search the Trie for the existence of the word

                    if(!wordExists) {                               // If the word does not exist
                        printf("%s (%d, %d): %s\n", textPath, row, col - wordLength, word);
                    }

                    wordLength = 0;                                 // Reset word length for next word
                }
            }
            else {
                word[wordLength++] = buffer[i];     // Append the next character to the word
            }
        }
    }

    close(text_FD);
    return true;
}

// #endregion

/***************************************************************************************************/
/********************************THE ABOVE CODE WORKS AS INTENDED***********************************/
/***************************************************************************************************/





void testCases(dictNode *root) {
    printf("search for Cattle: %d\n", searchTrie(root, "Cattle"));
    printf("search for Link: %d\n", searchTrie(root, "Link"));
    printf("search for Apple: %d\n", searchTrie(root, "Apple"));
    printf("search for apple: %d\n", searchTrie(root, "apple"));
    printf("search for efpa: %d\n", searchTrie(root, "efpa"));
}

int main()
{
    dictNode *root = NULL;

    fillDictionary(DICTFILE, &root);
    //printTrie(root);
    //testCases(root);
    checkFile(TEXTFILE, root);

    freeTrie(root);

    return EXIT_SUCCESS;
}