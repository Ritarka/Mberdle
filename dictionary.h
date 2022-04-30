#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

/*
* This file contains the dictionary array which is 
* an array of Word structs that hold words that could be
* potential secret words. You may add or remove from the Word
* struct as you see fit for your own implementation.
* You should add more potential secret words to the dictionary.
*/

#define MAX_WORDS 5
#define LENGTH 2

typedef struct myWord {
   char* letters;
//   int counts[MAX_WORDS];
//   char unique[MAX_WORDS];
} Word;  

char* secretWord(int);

#endif
