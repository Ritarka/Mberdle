#include "dictionary.h"
#include "globals.h"
#include "hardware.h"
#include "graphics.h"
#include "doubly_linked_list.h"

extern int difficulty;

#define WORD_SIZE difficulty + 5

static DLinkedList* keyboard;
static LLNode* current;
static char* secret;
static char* guess;
static int pos;

struct ListItem {
    char letter;
};

void make_items(ListItem*, unsigned int);

/*
* Function to implement comparisons of data in the DLL.
* This function allows the comparisons of chars if you input
* chars into the doubly_linked_list. If you want you can make 
* additional functions to compare different types of data.
*
* This function should be passed into the create_dLinkedList()
* call.
*
*/
int compareChar(void* input1, void* input2);

/*
* This function should intialize the keyboard and 
* select the goal word. To pick goal word, select a word 
* randomly from the dictionary you create in dictionary.h 
* (hint: the function rand() can be used to generate a random
* number >=0).
* To create the keyboard, initialize a DLL with all the nodes
* being the letters of the alphabet. Display the visible part
* of the keyboard at the start of the game (e.g., for baseline: 
* display the first letter of the alphabet).
*/
void init_keyboard();

/*
* Scroll left on the keyboard.
*/
void moveleft();

/*
* Scroll right on the keyboard.
*/
void moveright();

/*
* Pick the current letter in the keyboard to use 
* in your guess.
*/
void select_letter();

/*
* Removes letters from your guess that you no
* longer want in your guess (Removes right to left).
*/
void remove_letter(char);

/*
* This function compares your guess to the secret word. If
* you guess the correct letter in the correct spot, keep the 
* letter displayed. If you guess the correct letter in the
* incorrect spot, display that letter on the top of the screen.
* Delete letters that are not in the secret word from the keyboard
* DLL. Also should handle game winning and game losing.
*
*/
int check_word(int);

char* current_letter();

void roll_word(int);

void add(char);