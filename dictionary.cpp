#include "dictionary.h"
#include <stdlib.h>

char* dictionary[3][4] = {
    
//    {
//    {"speed", {1,1,2,1,0}, {'s','p', 'e', 'd', '!'}},
//    {"crane", {1,1,1,1,1}, {'c', 'r', 'a', 'n', 'e'}
//    },
//    
//    {
//    {"abroad", {1,1,2,1,0}, {'s','p', 'e', 'd', '!'}},
//    {"crane", {1,1,1,1,1}, {'c', 'r', 'a', 'n', 'e'}
//    },
//    
//    {
//    {"speed", {1,1,2,1,0}, {'s','p', 'e', 'd', '!'}},
//    {"crane", {1,1,1,1,1}, {'c', 'r', 'a', 'n', 'e'}
//    }
//    
//    }

    {"speed", "crane"},
    {"abroad", "manage", "garden", "random"},
    {"exhibit", "uniform", "genuine", "purpose"}
};

char* secretWord(int difficulty) {
    int wordIndex = rand() % (sizeof(dictionary[difficulty])/sizeof(dictionary[difficulty][0]));
    return dictionary[difficulty][wordIndex];
}