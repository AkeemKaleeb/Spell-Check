#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

#define MAX_PATH_LENGTH 1000
#define HASH_MAP_SIZE 1000
#define DICTFILE "words.txt"


typedef struct hashMapEntry {
    char *key;      //the key used to identify the entry
    char *value;    //the value of the entry
}Entry;

//an array of key-value pairs
typedef struct HashMap {
    Entry* entries[HASH_MAP_SIZE];
}HashMap;

// hash function
unsigned int hash(const char *key) {
    unsigned int hash = 0;                          // sets up return hash value
    for(int i = 0; key[i] != '\0'; i++) {           // works through the keys until null character
        hash = 31 * hash + key[i];                  // assigns a hash value
    }
    return hash % HASH_MAP_SIZE;                    // returns the hash value based on the array size
}

//create a new HashMap
void createHashMap (){
    HashMap* map = (HashMap*) malloc (sizeof(HashMap));
    memset (map -> entries, 0, sizeof(Entry));  //initialize map with 000s
    return map;
}
/*
void put (HashMap * map, char * key, char * value){
    int index = hash (key);
    map->entries[index]->key = strcpy(key);
    map->entries[index]->key = strcpy (value); 
}
*/
//I'm not sure we want to use a HashMap here since it doesn't make logical sense
//some of the data structures to consider would be here: https://stackoverflow.com/questions/10017808/best-data-structure-for-implementing-a-dictionary

//When spchk is given a directory name as an argument, it will perform a recursive directory traversal
//and check spelling in all files whose names end with “.txt”, but ignoring any files or directories whose
//names begin with “.”

//a function to return all the files in a specified directory whose names end with ".txt", but don't begin with "."
int filesInDir(char* path){
    DIR *dir;//creates an object of struct DIR
    struct dirent * entry; //stores all the directory entries (ie, the files and subfolders in the directory)


    if ((dir = opendir (path)) == NULL){
        perror ("There was a problem opening the directory!\n");
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dir)) != NULL){ //while the entry we are reading from the directory is not null
        if ((strstr(entry->d_name, ".txt")!= NULL) && entry->d_name[0] != '.'){
            printf ("\n%s\n", entry->d_name);
        }
    }
    closedir(dir);
    return EXIT_SUCCESS;
}



int main(int argc, char *argv[]) {
    //argv[0]: spchk
    //argv[1]: dictionary path
    //argv[2-inf]: directory path
    
    if(argc < 3) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    filesInDir(argv[2]);
    return 0;
}
