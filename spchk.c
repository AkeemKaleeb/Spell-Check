#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_PATH_LENGTH 1000
#define DICTFILE "words.txt"


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

int main(int argc, char *argv[]) {
    checkFile("C:\\Users\\kaile\\Desktop\\Programming\\Programs\\CS214\\Assignment 2\\test.txt");
    return 0;
}