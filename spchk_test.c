#include "spchk.h"
#include <stdio.h>

//helper function to run each test based on the test number (test(1) uses file test1.txt, test(2) uses file test2.txt, etc)
void test(int i){
    dictNode *root = NULL;
    fillDictionary("./words.txt", &root);
    
    char filename [PATH_MAX];                                                                                               //allocate memory for the filename
    snprintf (filename, PATH_MAX, "./test%d.txt", i);     //construct the filename
    filesInDir(filename, root);  //checks all the '.txt' files in the directory
    freeTrie(root);             //free the Tree
}

//testing the capitilzation of our Spell Check Program
int main (int argc, char * argv[]){
    /*********TEST 1:*********/
    //basic text file with common English words, all spelled correctly
    //expected output: the program should run smoothly without any errors
    test (1);       

    /*********TEST 2:*********/
    //we should test a file with a lot of punctuation (!@#$%^&*(){})
    //expected output: the words should not be reported as incorrect, even with the punctuation
    test (2);       //normal punctuation    //jane eyre file

    /*********TEST 3:*********/
    //we should test a file with a lot of punctuation (!@#$%^&*(){})
    //expected output: the words should not be reported as incorrect, even with the punctuation
    test (3);       //abnormal punctuation

    /*********TEST 4:*********/
    //we should use one of the text files with incorrect punctuation
    test (4);       //incorrectly spelled words

    /*********Test 5:*********/
    //we should test all the issues relating to capitilzation, punctuation, spelling, etc.
    //they should all be reported

    test (5);

    /*********Test 6:*********/
    //we should use a file containing words with incorrect capitilzation (ie, MacDonalds or MACDONALDS should work, but not macdonalds)
    //they should all be reported
    test (6);

    return 0;
}
