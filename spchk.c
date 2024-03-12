#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#define NUM_NODES 26
//trie data structure
/************************************************************************************************************/
struct TrieNode {
    //each node has 26 children, corresponding to each letter of the alphabet
    struct TrieNode * children[NUM_NODES];
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
    newNode->isLeafNode = 0;
    newNode->value = c;
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

//function to insert an entire word into the tree
node * insertTree (node * root, char * word){
    node* current = root;
    int size = sizeof(word) - 1; //since the word also contains '\0'

    for (int i = 0; i < size; i++){
        int index = tolower(word[i]) - 'a'; //this should return the position of the letter in the children array
        if (current->children[index] == NULL){
            //create node
            current->children[index] = makeNode (word[i]);
        }
        if (i == size - 1){
            current->isLeafNode = 1;   //set the boolean equal to true if this is the last letter/node
        }
        current = current->children[index];
    }
    return root;
}
int searchTree (node *root, char * word){
    node* current = root;
    int size = sizeof(word) - 1; //since the word also contains '\0'

    for (int i = 0; i < size; i++){
        int index = tolower(word[i]) - 'a'; //this should return the position of the letter in the children array
        if (current->children[index] == NULL){
            return EXIT_FAILURE;    //the node does not exist //this is not a word
        }
        current = current->children[index];
    }
    return EXIT_SUCCESS;
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
/************************************************************************************************************/
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
        struct stat statBuff;
        char entryPath[1024];
        snprintf(entryPath, sizeof(entryPath), "%s%s", path, entry->d_name);
        if (stat(entryPath, &statBuff == -1)){
            perror("Issue with getting file stats.");
        }

        if (S_ISDIR(entry) == TRUE){
            filesInDir(entry.d_name);
        }
        else if ((strstr(entry->d_name, ".txt")!= NULL) && entry->d_name[0] != '.'){
            printf ("\n%s\n", entry->d_name);
        }
    }
    closedir(dir);
    return EXIT_SUCCESS;
}



int main(int argc, char *argv[]) {
    node* root = makeNode('\0');
    root = insertTree(root, "hello");
    root = insertTree(root, "hi");
    root = insertTree(root, "teabag");
    root = insertTree(root, "teacan");
    printTree(root);
    //argv[0]: spchk
    //argv[1]: dictionary path
    //argv[2-inf]: directory path
    /*
    if(argc < 3) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    filesInDir(argv[2]);
    */
    return 0;
}
