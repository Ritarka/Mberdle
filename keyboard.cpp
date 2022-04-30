#include "keyboard.h"
#include "globals.h"
#include "dictionary.h"
#include "doubly_linked_list.h"
#include <string>

char* correct_letters;

void make_items(ListItem* result[], unsigned n)
{
    while(n--)
    {
        result[n] = (ListItem*) malloc(sizeof(ListItem));
        result[n]->letter = n + 97;
    }
}

int compareChar(void* input1, void* input2){
    if(*(char*)input1 == *(char*)input2){
        return 1;
    }
    else{
        return 0;
    }
}

void init_keyboard()
{
    keyboard = create_dlinkedlist(compareChar);
    
    ListItem* m[26];
    make_items(m, 26);
    
    for(int i = 0; i < 26; i++) {
        insertTail(keyboard, m[i]);
    }
    
    //Create double circularly linked list
    keyboard->head->prev = keyboard->tail;
    keyboard->tail->next = keyboard->head;
    
    current = keyboard->head;
}

char* current_letter() {
    ListItem* item = (ListItem *) current->data;
    return &item->letter;
}

void moveleft()
{
    current = current->prev;
}


void moveright()
{
    current = current->next;
}


void select_letter()
{
    guess[pos] = ((ListItem *)(current->data))->letter;
    pos++;
}

void remove_letter(char letter)
{
    LLNode* check = keyboard->head;
    for (int i = 0; i < keyboard->size; i++) {
        char test = ((ListItem *) (check->data))->letter;
        if (test == letter) {
            if (keyboard->size == 1) {
                destroyList(keyboard);
                break;
            }
            
            if (current == check) {
                current = current->next;
            }
            
            LLNode* prev = check->prev;
            LLNode* next = check->next;
            prev->next = next;
            next->prev = prev;
            free(check->data);
            free(check);
        }
        check = check->next;
    }
}

int check_word(int row)
{
    pos = 0;
    int match = 1;
    for (int i = 0; i < WORD_SIZE; i++) {
        if (guess[i] == secret[i]) {
            //add(guess[i]);
        } else {
            match = 0;
            int exist = 0;
            for (int j = 0; j < WORD_SIZE; j++) {
                if (guess[i] == secret[j]) {
                    add(guess[i]);
                    exist = 1;
                    break;
                }
            }
            if (!exist) {
                remove_letter(guess[i]);
            }
            clear(7 * i, row * 8, 7, 8);
            if (i < WORD_SIZE) {
                if (exist) {
                    uLCD.text_char(guess[i], i, row, 0xFFFF00);
                    if (guess[i] == 'Y') {
                        uLCD.printf("\n%d\n", i);
                    }
                } else {
                    uLCD.text_char(guess[i], i, row, LGREY);
                }
            }
        }
    }
    return match;
}

void roll_word(int difficulty) {
    secret = secretWord(difficulty);
    //uLCD.text_string(secret, 0, 13, FONT_7X8, GREEN); 
    pos = 0;
    guess = (char *)malloc(sizeof(char) * WORD_SIZE);
    correct_letters = (char *)malloc(sizeof(char) * WORD_SIZE);
    for (int i = 0; i < WORD_SIZE; i++) {
        guess[i] = NULL;
        correct_letters[i] = NULL;
    }
}

void add(char letter) {
    for (int i = 0; i < WORD_SIZE; i++) {
        if (correct_letters[i] == NULL) {
            correct_letters[i] = letter;
            break;
        } else if (letter == correct_letters[i]) {
            break;
        }
    }
}