#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_PATH_LENGTH 1000
#define HASH_MAP_SIZE 100000
#define DICTFILE "words.txt"

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
void put(HashMap *map, const char *key, const char *value) {
    unsigned int index = hash(key);                             // establish an index given a key
    Entry *entry = (Entry*)malloc(sizeof(Entry));               // initialize and allocate size for a new entry
    entry->key = strdup(key);                                   // set the entry key to the parameter key
    entry->value = strdup(value);                               // set the value to the parameter value
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


// Find and Open Files -> Directory Traversal
// Reading the file and generating a sequence of position-annotated words
void searchdirectory(const char *dirPath) {
    DIR *dir = opendir(dirPath);                        // set a pointer to the directory path
    if(dir == NULL) {                                   // check if the directory exists
        perror("Error opening directory");              // print error if it does not
        exit(EXIT_FAILURE);                             // exit program
    }

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL) {
        char fullPath[MAX_PATH_LENGTH];                 // construct the full path of an entry
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);

        struct stat stat_buf;
        if(stat(fullPath, &stat_buf) == -1) {
            perror("Error getting file status");
            continue;
        }

        if(S_ISDIR(stat_buf.st_mode)) {
            // Ignore "." and ".." directories
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            searchdirectory(fullPath);              // recursively search for subdirectories
        }
        else {
            // check if the file ends with ".txt"
            if(strlen(entry->d_name) > 4 && strcmp(entry->d_name + strlen(entry->d_name) - 4, ".txt") == 0) {
                checkFile(fullPath);                // process the file
            }
        }
    }

    closedir(dir);
}

// Checking if each word in a given filepath is contained in the dictionary
void checkFile(const char *filePath) {
    // open the dictionary file
    FILE *dictFile = fopen(DICTFILE, "r");              // set a pointer to the dictionary file in read only mode
    if(dictFile == NULL) {                              // check if the dictionary file exists in the project
        perror("Error opening dictionary file");        // print error if it does not
        exit(EXIT_FAILURE);                             // exit program
    }

    // open the text file
    FILE *textFile = fopen(filePath, "r");              // set a pointer to the provided text file in read only mode
    if(textFile == NULL) {                              // check if the text file exists
        perror("Error opening text file");              // print error if it does not
        exit(EXIT_FAILURE);                             // exit program
    }

    // Read words from the text file and compare with the dictionary
    // TODO: Make dictionary into hashmap for faster traversal
    // TODO: Implement word comparison
    char word[MAX_PATH_LENGTH];
    while(fscanf(textFile, "%s", word) != EOF) {
        // Compare the given word with the dictionary
        // Ignore punctuation
        // Allowed lowercase, Firstcase, ALLCAPS, hypenated-words
        // Just printing the word right now to show it is functional
        printf("%s\n", word);
    }

    // Close all files
    fclose(textFile);
    fclose(dictFile);
}

// function to initialze a hash map for the dictionary words
HashMap* initializeHashMap() {
    FILE *dictFile = fopen("words.txt", "r");           // open the dictiionary in read only mode
    if(dictFile == NULL) {                              // checks if the dictionary file is in the folder
        perror("Error opening dictionary file");        // prints error if it is not
        exit(EXIT_FAILURE);                             // exit the program
    }

    HashMap *map = createHashMap();                     // establish the hashmap

    char word[MAX_PATH_LENGTH];                         // make an array of words
    while(fscanf(dictFile, "%s", word) != EOF) {        // scan the full text file dictionary
        put(map, word, word);                           // put every word into the hashmap
    }

    fclose(dictFile);                                   // close the dictionary to keep things clean
    
    return map;
}


int main(int argc, char *argv[]) {
    const char *searchWord = "example";         // example word to check for
    HashMap *map = initializeHashMap();         // declare a hashmap
    char *result = get(map, searchWord);        // checks if the word exists
    if(result != NULL) {
        printf("Word found: %s\n", result);     // prints the output if the word exists
    }
    else {
        printf("Word not found\n");             // prints an error if the word is not found
    }

    freeHashMap(map);                           // free the hashmap

    return 0;
}