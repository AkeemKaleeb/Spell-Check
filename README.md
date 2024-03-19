Kaileb Cole (kjc265) & Shreya Shukla (ss4515)
CS 214: Systems Programming
Professor Menendez
Programming Assignment #2
02/26/2024

/**SpellCheckProject**/
This project will create a spell checker to detect which words are spelt incorrectly given a dictionary of words and a file or multiple to read from and traverse

Usage: type "make spchk_test" to compile spchk_test.c. then type "./spchk_test" to run our test cases and see the functionality of our spellcheck program.

Test Cases:
Test(1) makes sure that the spellcheck program runs smoothly without any error messages. It inputs a text file with common English words that have no spelling, punctuation or capitilization mistakes.

Test(2) runs a text file with normal punctuation. There should be no punctuation errors reported in Test(2) (except for Georgina)

Test(3) runs a text file with abnormal punctuation. There should be a bunch of errors reported.

Test(4) runs a text file with incorrect spelling, which should also be reported.

Test(5) runs a text file with multiple types of mistakes, relating to punctuation, capitilization, and spelling. These should all be reported.

Test(6) runs a file containing words with incorrect capitilzation (example: MacDonalds or MACDONALDS should work, but not macdonalds). These mistakes should all be reported
