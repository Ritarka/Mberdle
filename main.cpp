//=================================================================
// The main program file.
//
// Copyright 2022 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

// External libs
#include <stdlib.h>

// Project includes
#include "globals.h"
#include "hardware.h"
#include "graphics.h"
#include "keyboard.h"


//some LED's to use for debugging.
DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);

extern char* correct_letters;

int leaderboard[5];
int difficulty = 0;

void set_random_seed();
void print(char*, int, int);

/*
* This function handles the main logic of the game. You should
* initialize the hardware in this function, make calls to 
* functions that update the keyboard and your guess of 
* the word. 
*/
int main()
{
    
    GameInputs inputs; 
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    pc.printf("Program Starting\n");
    //led1 hardware initialization
    
    //myled1=1;  
    Timer t;
    Timer playTime;
    int dt; // delta time
    
    //Initialize leaderboard
    for (int i = 0; i < sizeof(leaderboard)/sizeof(leaderboard[0]); i++) {
        leaderboard[i] = NULL;    
    }
    
    leaderboard[0] = 1300;
    leaderboard[1] = 950;
    leaderboard[2] = 800;
    
/* Put code here to initialize the game state:
   1) you will need to use a psuedo random number generator such as
     rand() to randomly select a goal word from the dictionary.
     The random function needs to be seeded before you call rand(),
     otherwise, rand() will always generate the same series of numbers.
     Call the function set_random_seed -- you need to complete its
     definition below.*/
    restart: set_random_seed();
/*
   2) call init_keyboard() and show start screen
   
   3) initialize any other game state that you add.
*/

    play: init_keyboard();
    
    int win = 1;
    int col = 0;
    int row = 2;
    int count = 0;
    roll_word(difficulty);
    playTime.start();
    print("Correct: ", 0, 0);
    while(1){
        //break;
        t.start(); //start a timer
        draw_lower_status(); //draw the lower status bar
        inputs = read_inputs(); //read the inputs of the game

        print(current_letter(), 15, 8);
        
        if (count == 5) {
            if (inputs.ax > .3) {
                moveleft();
            } else if (inputs.ax < -.3) {
                moveright();
            }
            count = 0;
        }
        
        if (!inputs.b1) {
            print(current_letter(), row, col);
            select_letter();
            if (col == 4 + difficulty) {
                wait_ms(500);
                if (check_word(row)) {
                    break;
                }
                row++;
                if (row == 8) {
                    win = 0;
                    break;
                }
                col = 0;
                
                print("Correct: ", 0, 0);
                for (int i = 0; i < difficulty + 5; i++) {
                    if (correct_letters[i] == NULL) {
                        break;
                    }
                    print(&correct_letters[i], 0, 8 + i);
                }
                
            } else {
                col++;
            }
        } else if (!inputs.b2 && !inputs.b3) {
            uLCD.cls();
            goto restart;
        } else if (!inputs.b2) {
            moveleft();
        } else if (!inputs.b3) {
            moveright();
        }
        
        while (!(inputs.b1 && inputs.b2 && inputs.b3)) { inputs = read_inputs(); }
        
        count++;
        
        t.stop();
        dt = t.read_ms();
        t.reset();
        if (dt < 100) wait_ms(100 - dt);
    }
    
    while (!(inputs.b1 && inputs.b2 && inputs.b3)) { inputs = read_inputs(); }
        
    uLCD.cls();
    playTime.stop();
    int time = playTime.read_ms() / 1000;
    playTime.reset();
    
    int score = 10000 - 50 * time;
    
    //Add current score to leaderboard
    for (int i = 0; i < sizeof(leaderboard)/sizeof(leaderboard[0]); i++) {
        if (leaderboard[i] == NULL) {
            leaderboard[i] = score;
            break;
        } else if (score > leaderboard[i]) {
            int temp = leaderboard[i];
            leaderboard[i] = score;
            score = temp;
        }
    }
    
//    FILE *fp = fopen("/sd/fail.wav", "r");
//    waver.play(fp);
//    fclose(fp);
    
    //Winscreen
    if (win) {
        uLCD.printf("Hooray! You won!\n");
        uLCD.printf("It took %d seconds for you to win\n\n", time);
        uLCD.printf("Leaderboard\n");
        for (int i = 0; i < sizeof(leaderboard)/sizeof(leaderboard[0]); i++) {
            if (leaderboard[i] != NULL) {
                uLCD.printf("%d: %5d\n", i + 1, leaderboard[i]);
            }
        }
    } else {
        //Deathscreen
        uLCD.printf("Oh no! You took   too many tries!\n");   
    }
    
    uLCD.printf("\nPress button 1 to restart\n\n");
    uLCD.printf("Press button 2 to quit");
    while(inputs.b1 && inputs.b2) {
        inputs = read_inputs();
    }
    uLCD.cls();
    if (!inputs.b1) {
        while (!inputs.b1) {inputs = read_inputs();}
        if (win && difficulty < 2) difficulty++;
        goto play;
    }
    print("Thanks for Playing", 7, 0);
    exit(0);
}



/* This should be called in the main function above.
*
* This function sets the random seed for the game using srand(seed).
* One incomplete way to do this is given below: start a Timer and sample it
* to get a seed. The problem is that the same exact time will be read
* every time you reset the mbed.  You need to add code to introduce
* variability into when the timer is stopped.  An easy way to do this
* is to get the user to press a push button to stop the timer at a random
* time.  You can print a prompt to the screen to ask the user to push
* any button to start, then run an infinite while loop that waits for a push 
* button press to break. The Timer is then stopped and read and elapsed time
* used as the seed. (This requires using read_inputs() into a GameInputs variable.)
*/
void set_random_seed() {
    Timer t;
    Timer t2;
    t.start(); // start the timer
    
    uLCD.printf("Hold button 1 to  start\n");
    
    GameInputs input = read_inputs();
    
    int animation = 0;
    int change = 0;
    while (input.b1) {
        t2.start();
        
        clear(58, 58, 16, 16);
        if (animation == 0) {
            animation = 1;
            small_star();
        } else if (animation == 1) {
            animation = 2;
            dim_star();
        } else {
            animation = 0;
            bright_star();
        }
        
        input = read_inputs();
        
        if (!input.b2 && difficulty) {
            difficulty--;
            change = 1;
        } else if (!input.b3 && difficulty < 2) {
            difficulty++;
            change = 1;
        } else {
            change = 0;
        }
        
        uLCD.current_row = 14;
        uLCD.printf("\r");
        
        if (change) {
            clear(25, 112, 16, 16);
        }
        
        if (difficulty == 0) {
            uLCD.printf("Easy");
        } else if (difficulty == 1) {
            uLCD.printf("Medium");
        } else if (difficulty == 2) {
            uLCD.printf("Hard");
        }
        
        t2.stop();
        int dt = t2.read_ms();
        t2.reset();
        if (dt < 750) wait_ms(750 - dt);
    }
    while (!(input.b1)) {
        input = read_inputs();
    }
    t.stop();               //end the timer
    int seed = t.read_ms(); //read the number of milliseconds elapsed between the start and stop
    srand(seed);            //use elapsed time as the seed
    
    uLCD.cls();
}

void print(char *text, int row, int col) {
    uLCD.text_string(text, col, row, FONT_7X8, GREEN);
}
// ===User implementations end===
