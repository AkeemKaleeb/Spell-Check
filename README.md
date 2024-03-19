Kaileb Cole (kjc265) & Shreya Shukla (ss4515)
CS 214: Systems Programming
Professor Menendez
Programming Assignment #2
02/26/2024

# SpellCheckProject
 This project will create a spell checker to detect which words are spelt incorrectly given a dictionary of words and a file or multiple to read from and traverse. It takes in a dictionary path and directories or individual text files and searches through every text file recursively, reporting back any words that cannot be found within the dictionary.

 If a word is found to not match a word in the dictionary, an error is printed to the console including the file name, the row number, the column number, and the word that was misspelt.

 # Testing
 In order to test this project, we have included a collection of directories and sub directories that all include different name.txt files and .name.txt to ensure that only the proper files are checked. It then checks every name.txt file word by word and compares it to the dictionary to check for any possible spelling errors and reports it.
