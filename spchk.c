#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define MAX_WORD_SIZE 100

typedef struct trie_node{
    struct trie_node *children[ALPHABET_SIZE * 2];  // account for capital and lowercase letters
}trieNode;

typedef struct trie{
    trieNode root;
} dictTrie;

struct trie dictionary;                 // dictionary trie being used

// Initialise the dictionary trie
dictTrie initialiseDictionary() {
    dictionary = (struct trie) {};     // zeroes out the tree 
}

// Function to convert letters to integer values based upon its alphabetical position
// Returns -1 if the character is not in the alphabet
int letterToInt(char letter){
    if(letter >= 'A' && letter <= 'Z') {
        return letter - 'A' + 26;
    }
    else if(letter >= 'a' && letter <= 'z') {
        return letter - 'a';
    }
    return -1;
}

// Function to remove invalid characters at the end of a word
void removeInvalidCharacters(const char* word) {

}

// Function to insert word into the trie
int put(trieNode *node, const char *word) {
    for(int i = 0; i < strlen(word); i++) {
        int letter = letterToInt(word[i]);
        if(letter == -1) {
            printf("Invalid character in the dictionary");
            return EXIT_FAILURE;
        }

        trieNode *parent = node;
        node = node->children[letter];

        if(!node) {
            node = malloc(sizeof(struct trie_node));
            parent->children[letter] = node;
        }
    }

    return EXIT_SUCCESS;
}

// Travers the trie from the root node to search for a word
char *get(trieNode *node, const char *word) {
    for(int i = 0; i < strlen(word); i++) {
        int letter = letterToInt(word[i]);
        if(letter == -1) {
            return EXIT_FAILURE;
        }

        node = node->children[letter];
        if(!node) {
            printf("Word not found in dictionary");
            return EXIT_FAILURE;   // word not found
        }
    }

    return ("Word found: %s\n", word);
}

// Function to fill the dictionary trie
int fillDictionary(const char *dictPath)  {
    int file = open(dictPath, O_RDONLY);
    if(file == -1) {
        printf("Could not find/open dictionary \"%S\"\n", dictPath);
    }

    char buffer[MAX_WORD_SIZE + 2];     // +2 for space and newline characters
    int count = 0;

    while(1) {
        int bytesRead = read(file, buffer, sizeof(buffer) - 1);     // read a line from the file

        if(bytesRead == -1) {
            perror("Error reading file");
            close(file);
            return EXIT_FAILURE;
        }
        else if(bytesRead == 0) {           // EOF
            break;
        }

        buffer[bytesRead] = '\0';           // Null-terminate the buffer

        char *word = strtok(buffer, " ");

        if(!word) {
            printf("Error parsing line: %s\n", buffer);
            close(file);
            return EXIT_FAILURE;
        }

        // Insert the word into the trie
        if(!put(&dictionary.root, word)) {
            close(file);
            return EXIT_FAILURE;
        }
        else{
            count++;
        }     
    }

    close(file);
    printf("Parsed Dictionary \"%s\" with %i entries\n", dictPath, count);
    return EXIT_SUCCESS;
}

// Helper method to free a node
void freeNode(trieNode *node) {
    if(!node) {
        return;
    }

    for(int i = 0; i < ALPHABET_SIZE * 2; i++) {
        freeNode(node->children[i]);
    }

    free(node);
}

// Function to free the trie
void freeTrie() {
    freeNode(&dictionary.root);
}

int main() {


    return 0;
}


/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX_PATH_LENGTH 1000
#define HASH_MAP_SIZE 105000
#define DICTFILE "words.txt"
#define TESTFILE "test.txt"

typedef struct hashMapEntry {
    char *key;
    char *value;
}Entry;

typedef struct HashMap {
    Entry * entries[HASH_MAP_SIZE];
}HashMap;

// hash function
unsigned int hash(const char *key) {
    unsigned int hash = 0;                          // sets up return hash value
    for(int i = 0; key[i] != '\0'; i++) {           // works through the keys until null character
        hash = 31 * hash + key[i];                  // assigns a hash value
    }

    return hash % HASH_MAP_SIZE;                    // returns the hash value based on the array size
}

// function to initializae a hashmap
HashMap *createHashMap(){
    HashMap *map = (HashMap*)malloc(sizeof(HashMap));
    memset(map->entries, 0, sizeof(map->entries));
    return map;
}

// function to enter a new entry into the hash map
void put(HashMap *map, char *key, char *value) {
    unsigned int index = hash(key);                             // establish an index given a key
    Entry *entry = (Entry*)malloc(sizeof(Entry));               // initialize and allocate size for a new entry
    if(!entry) {
        perror("Error allocating memory for entry");
    }
    entry->key = strdup(key);                                           // set the entry key to the parameter key
    entry->value = strdup(value);                                       // set the value to the parameter value
    map->entries[index] = entry;                                // put the entry into the hashmap
}

// function to allow a specific entry to be retrieved from the hashmap
char *get(HashMap *map, const char *key) {
    unsigned int index = hash(key);                             // establish an index given a key for the hashmap
    if(map->entries[index] == NULL) {                           // return NULL if there is no entry at the given position
        return NULL;
    }
    return map->entries[index]->value;                          // if there is an entry, return the value
}

// function to free a hashmap from memory correctly
void freeHashMap(HashMap *map) {
    for(int i = 0; i < HASH_MAP_SIZE; i++) {
        if(map->entries[i] != NULL) {
            free(map->entries[i]->key);                         // free the key
            free(map->entries[i]->value);                       // free the value
            free(map->entries[i]);                              // free the entry
        }
    }

    free(map);                                                  // free the map
}

// function to initialze a hash map for the dictionary words
HashMap* initializeHashMap(const char *dictPath) {
    printf("%s", dictPath);
    int dictFile = open(dictPath, O_RDONLY);            // open the dictiionary in read only mode
    if(dictFile == -1) {                                // checks if the dictionary file is in the folder
        perror("Error opening dictionary file");        // prints error if it is not
        exit(EXIT_FAILURE);                             // exit the program
    }

    HashMap *map = createHashMap();                                             // establish the hashmap

    char word[MAX_PATH_LENGTH];                                                 // make an array of words
    ssize_t bytesRead;
    while((bytesRead = read(dictFile, word, MAX_PATH_LENGTH - 1) > 0)) {        // scan the full text file dictionary
        word[bytesRead] = '\0';
        put(map, word, word);                                                   // put every word into the hashmap
    }

    if(bytesRead == -1) {
        perror("Error reading dictionary file");
        exit(EXIT_FAILURE);
    }

    close(dictFile);                                                           // close the dictionary to keep things clean
    
    return map;
}

// Checking if each word in a given filepath is contained in the dictionary
void checkFile(const char *dictPath, const char *filePath, HashMap *map) {
    // open the dictionary file
    printf("%s", dictPath);
    int dictFile = open(dictPath, O_RDONLY);            // set a pointer to the dictionary file in read only mode
    if(dictFile == -1) {                                // check if the dictionary file exists in the project
        perror("Error opening dictionary file");        // print error if it does not
        exit(EXIT_FAILURE);                             // exit program
    }

    // open the text file
    int textFile = open(filePath, O_RDONLY);            // set a pointer to the provided text file in read only mode
    if(textFile == -1) {                                // check if the text file exists
        perror("Error opening text file");              // print error if it does not
        exit(EXIT_FAILURE);                             // exit program
    }

    // Read words from the text file and compare with the dictionary
    // TODO: Implement word comparison
    char word[MAX_PATH_LENGTH];
    ssize_t bytesRead;
    while((bytesRead = read(textFile, word, MAX_PATH_LENGTH - 1)) > 0) {
        // Compare the given word with the dictionary
        // Ignore punctuation
        // Allowed lowercase, Firstcase, ALLCAPS, hypenated-words
        // Just printing the word right now to show it is functional
        word[bytesRead] = '\0';
        printf("%s\n", word);
    }

    // Close all files
    close(textFile);
    close(dictFile);
}

// Find and Open Files -> Directory Traversal
// Reading the file and generating a sequence of position-annotated words
void searchdirectory(const char *dictPath, const char *dirPath, HashMap *map) {
    printf("%s", dictPath);
    int dir = open(dirPath, O_RDONLY);                 // set a pointer to the directory path
    if(dir == -1) {                                     // check if the directory exists
        perror("Error opening directory");              // print error if it does not
        exit(EXIT_FAILURE);                             // exit program
    }

    char buffer[MAX_PATH_LENGTH];
    ssize_t bytesRead;
    while((bytesRead = read(dir, buffer, MAX_PATH_LENGTH)) > 0) {
        for(int i = 0; i < bytesRead; i++) {
            if(buffer[i] == '\n') {
                buffer[i] = '\0';
                checkFile(*dictPath, buffer, map);
            }
        }
    }

    if(bytesRead == -1) {
        perror("Error reading directory");
        exit(EXIT_FAILURE);
    }

    close(dir);
}

int main(int argc, char *argv[]) {
    //argv[0]: spchk
    //argv[1]: dictionary path
    //argv[2-inf]: files to check

    HashMap *map = initializeHashMap("words.txt");
    
    /*if(argc < 3) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
*/

    //searchdirectory("words.txt", "test.txt", map);
    //freeHashMap(map);                           // free the hashmap

    
    /*int dictFile = open("words.txt", O_RDONLY);
    char word[MAX_PATH_LENGTH];                                                 // make an array of words
    ssize_t bytesRead;
    while((bytesRead = read(dictFile, word, MAX_PATH_LENGTH - 1) > 0)) {        // scan the full text file dictionary
        printf("%s\n", word);
    }*/
/*
    return 0;
}
*/