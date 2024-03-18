#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#define NUM_NODES 128        //assume all characters are in ASCI 128
#define BUFFER_SIZE 1024    //limit the number of system calls
#define PATH_MAX 4096       //max number of characters in a path name
//tree data structure
/************************************************************************************************************/
struct TreeNode {
    //each node has 26 children, corresponding to each letter of the alphabet
    struct TreeNode * children[NUM_NODES];
    int isLeafNode; //0 means FALSE and 1 means TRUE - if isLeafNode is true, that means this node represents the end of a word
    char value; //stores the actual node/character value
} typedef node;

//creates a new node
node* makeNode (char c){
    node* newNode = (node*) malloc (sizeof(node)); //what's the difference between using malloc and calloc over here?
                                                    //the latter fills the allocated memory wih 0s
    for (int i = 0; i < NUM_NODES; i++){
        newNode->children[i] = NULL;
    }
    newNode->isLeafNode = 0;    //not a leaf node
    newNode->value = c;         //stores the letter
    return newNode;
}
//frees a single node
void freeNode (node * targetNode){
    for (int i = 0; i < NUM_NODES; i++){
        if (targetNode->children[i] != NULL){
            free (targetNode->children[i]);
        }
    }
    free(targetNode);
}
//function to print all the words in a tree
void printTree(node * root){
    if (root == NULL){
        return;
    }
    node* current = root;
    printf ("%c -> ", current->value);
    for (int i = 0; i < NUM_NODES; i++){
        printTree(current->children[i]);
    }
}
//function to insert an entire word into the tree
node * insertTree (node * root, char * word){
    node* current = root;
    for (int i = 0; word[i] != '\0'; i++){
        int index = (int) word[i] - '0'; //this should return the position of the letter in the children array
        if (current->children[index] == NULL){
            //create node
            current->children[index] = makeNode (word[i]);
        }
        current = current->children[index];
    }
    current->isLeafNode = 1;   //set the boolean equal to true if this is the last letter/node
    return root;
}
//searches the dictionary for a single word
int searchTree (node *root, char * word){
    node* current = root;
    int size = sizeof(word) - 1; //since the word also contains '\0'

    for (int i = 0; i < size; i++){
        int index = tolower(word[i]) - '0'; //this should return the position of the letter in the children array
        if (current->children[index] == NULL){
            return EXIT_FAILURE;    //the node does not exist //this is not a word  //returns 1
        }
        current = current->children[index];
    }
    return EXIT_SUCCESS;    //which is 0
}
//we need to free the entire Tree Data Structure
void freeTree (node * root){
    if (root == NULL){
        return;
    }
    for (int i = 0; i < NUM_NODES; i++){
        freeTree(root->children[i]);
    }
    free(root);
}
/************************************************************************************************************/

void * fillDictionary(char* dictFile){
    int fd = open(dictFile, O_RDONLY); //file descriptor 

    if (fd == -1){
        perror ("Could not open dictionary file!\n");
        return NULL;
    }
    
    char buffer [BUFFER_SIZE]; //assuming the length of the words will not exceed 1024 characters (the longest english word is only 45 chars)
    ssize_t bytesRead;
    char word [BUFFER_SIZE];
    int pos = 0;

    node* root = (node *) malloc(sizeof (node));    //create a root node
    
    while ((bytesRead = read (fd, buffer, BUFFER_SIZE)) > 0){  //read fxn returns 0 when there is an error in reading bytes from the open files
        for (int i = 0; i < bytesRead; i++){
            if (buffer[i] == '\n'){
                word[pos] = '\0';
                //printf("%s\n", word);
                insertTree(root, word);
                pos = 0;
            }
            else {
                word[pos] = buffer[i];
                pos++;
            }
        }
    }
    close(fd);
    return root;
}

//spellchecks all the words in a text file
int checkFile (node* dict_root, char* textFile){
    int fd = open (textFile, O_RDONLY);

    if (fd == -1){
        perror ("Could not open text file!\n");
        return EXIT_FAILURE;    //AKA 1
    }

    char buffer [BUFFER_SIZE]; //assuming the length of the words will not exceed 1024 characters (the longest english word is only 45 chars)
    ssize_t bytesRead;
    char word [BUFFER_SIZE];
    int pos = 0;

    int col = 1;
    int line = 1;

    while ((bytesRead = read (fd, buffer, BUFFER_SIZE)) > 0){
        for (int i = 0; i < bytesRead; i++){
            if (buffer[i] == '\n'){
                line++;
                col = 1;
            }
            else{
                col++;
            }
            if (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n'){
                word[pos] = '\0';
                //printf("%s\n", word);
                pos = 0;
                if (searchTree (dict_root, word) == 1){  //if the word DOES NOT exist in the dictionary
                    printf ("%s (%d,%d): %s\n", textFile, line, col, word);
                }
            }
            else {
                word[pos] = buffer[i];
                pos++;
            }
        }
}
    close(fd);
    return EXIT_SUCCESS;    //AKA 0
};

//When spchk is given a directory name as an argument, it will perform a recursive directory traversal
//and check spelling in all files whose names end with “.txt”, but ignoring any files or directories whose
//names begin with “.”

//a function to return all the files in a specified directory whose names end with ".txt"
int filesInDir(node* dict_root, char* path){
    DIR *dir;//creates an object of struct DIR
    struct dirent * entry; //stores all the directory entries (ie, the files and subfolders in the directory)
    //printf("Callling function filesInDir!\n");
    //printf("Path: %s\n", path);

    //checks if we're able to open the directory
    if ((dir = opendir (path)) == NULL){
        perror ("There was a problem opening the directory!\n");
        closedir(dir);
        return EXIT_FAILURE;    //AKA 1
    }

    while ((entry = readdir(dir)) != NULL){ //while the entry we are reading from the directory is not null
        //skip any directory entries that begin with '.', including the links to self (".") and parent ("..")
        if (entry->d_name[0] !='.'){
            //this is the base case
            if (entry->d_type == DT_REG && (strstr(entry->d_name, ".txt")!= NULL)){
            printf ("%s!\n", entry->d_name);
            checkFile (dict_root, entry->d_name);
            }
            //check if the entry is a sub-directory //recursive case
            else if (entry->d_type == DT_DIR){
                //recursive case to print all the directory entries in the sub-directory
                char subpath[PATH_MAX];
                snprintf(subpath, sizeof(subpath), "%s/%s", path, entry->d_name);
                filesInDir(dict_root, subpath);
            }
         }
    }
    closedir(dir);
    return EXIT_SUCCESS;    //AKA 0
}
/* for some reason, this did not work in my filesInDir function
        //check if the entry is a sub-directory //recursive case
        else if (entry->d_type == DT_DIR){
            //recursive case to print all the directory entries in the sub-directory
            char* subpath; //we need to construct the directory path
            subpath = malloc(strlen(path) + strlen(entry->d_name) + 2); //length of the current path, directory name, plus 2 for '/' and '\0'
            
            //construct the subpath
            strcat(subpath, path);
            strcat(subpath, "/");
            strcat(subpath, entry->d_name); //we don't need to append '\0' because strcat appends it automatically

            filesInDir(subpath);    //recursive function call
            free(subpath);          //free the subpath
        }
*/
int main(int argc, char *argv[]) {    
    //filesInDir(argv[2]);
    //argv[0]: spchk
    //argv[1]: dictionary path
    //argv[2-inf]: directory path
    if(argc < 3) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE); //AKA 1
    }
    node * root = fillDictionary("./words.txt");
    //checkFile (root, "./test.txt");
    filesInDir(root, argv[2]);
    freeTree(root);
    return 0;
}
