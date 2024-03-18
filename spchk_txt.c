#include "spchk.h"

//testing the capitilzation of our Spell Check Program
int main (int argc, char * argv[]){
    /*********TEST 1:*********/
    //basic text file with common English words, all spelled correctly
    //expected output: the program should run smoothly without any errors
    test (1);

    /*********TEST 2:*********/
    //check what happens when there are no text files in the specified directory
    //expected output: nothing should be printed out
    test (2); 

    /*********TEST 3:*********/
    //we should test a file with a lot of punctuation (!@#$%^&*(){})
    //expected output: the words should not be reported as incorrect, even with the punctuation
    test (3);

    /*********TEST 4:*********/
    //we should use one of the text files (with incorrectly spelled words) as it's own dictionary file
    //expected output: the program should run smoothly without any errors
    test (4);

    /*********Test 5:*********/
    //we should use a file containing words with incorrect capitilzation (ie, MacDonalds or MACDONALDS should work, but not macdonalds)
    //they should all be reported

}

//helper function to run each test based on the test number (test(1) uses file test1.txt, test(2) uses file test2.txt, etc)
void test(int i){
    node * root = fillDictionary("/Users/shreyashukla/Desktop/CS_214_Systems/Assignment-2-SpellCheck/words.txt");
    char filename [PATH_MAX];                                                                                               //allocate memory for the filename
    snprintf (filename, PATH_MAX, "/Users/shreyashukla/Desktop/CS_214_Systems/Assignment-2-SpellCheck/test%d.txt", i);     //construct the filename
    filesInDir(root,filename);  //checks all the '.txt' files in the directory
    freeTree(root);             //free the Tree
}
