/* mipslabwork.c

   This file written 2015 by F Lundevall

   This file should be changed by YOU! So add something here:

   This file modified 2015-12-24 by Ture Teknolog 

   Latest update 2015-08-28 by F Lundevall

    Modified by Felix Hedenström
   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "sidefuncs.h"

volatile char * mPORTE = (volatile char * )0xbf886110; 

const unsigned int  START_SCREEN = 0, 
                    GUESS_SCREEN = 1, 
                    LOSE_SCREEN = 2,
                    PAST_GUESS = 3,
                    WRONG_SCREEN = 4,
                    INVALID_SCREEN = 5,
                    WIN_SCREEN = 6;

const unsigned int MAX_GUESSES = 6;

char symbols [] = {35,36, 43,45, 33,63,0}; // #$+-!?

char arrow = 62;

struct Guess {
    int * sequence;
    int * answer;
};







unsigned int timeoutcount;



void createString(char * res, const int symbol_index, int * guesses){
    int i = 0;
    while(res[i] != 0){
        i++;
    }
    int j;
    for(j = 0; j < 4; j++){
        if(j == symbol_index){
            res[i] = arrow;
            i++;
        }else{
            res[i] = 32;
            i++;
        }
        res[i] =  symbols[guesses[i]];
        res[++i] = 32;
        i++;
    }
}

void bintick(char bin_time){
    (*mPORTE) = bin_time;
}

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void ){
    volatile char * mTRISE = (volatile char *) 0xbf886100;
    volatile short * mTRISD = (volatile short *) 0xbf8860C0;

    *mTRISE = 0x00;     // Set to 0 for output.
    *mTRISD = *mTRISD | 0x0fe0; // Set bits 5-11 to input 

    //Clock init
    timeoutcount = 0;
    T2CON = 0x70;
    T2CONSET = 0x8000;
    PR2 = (80000000 / 256 ) / 10;
    return;
}

int timer(void){
    if(IFS(0) & 0x100){
        IFS(0) = 0;
        timeoutcount++;
        return 1;
    }
    return 0;
}

void updateScreen(unsigned int currentScreen, int * guess_nr, int * current_guess, const int symbol_index){
    if(currentScreen == START_SCREEN){
        display_string(0,"  ");
        display_string(1,"  Welcome to");
        display_string(2,"  MASTERMIND");
        display_string(3,"  ");
    }else if(currentScreen == GUESS_SCREEN){   
        char guess [16] = "Guess nr: ";
        concatenate(guess, 16, *guess_nr); 
        display_string(0, guess); 
        char input [16] = "   ";
        createString(input, symbol_index, current_guess); 
        display_string(1, input);
        display_string(2,"  ");
        display_string(3,"  ");
    }else if(currentScreen == LOSE_SCREEN){
        display_string(0,"  GAME OVER");
        display_string(1,"Right answer:");
        display_string(2,"  //TODO");
        display_string(3,"");
    }
    display_update();
}

/**
 * Returns 1 if the button was pressed last cycle but is not pressed
 * this cycle. Ergo the button was released
 */
char buttonPressed(int buttonNr, int buttons, int * lastBtns){
    int temp = powerTo(2,buttonNr - 1);

    if( (temp & *lastBtns) && ((buttons & temp) == 0) )                                                                                                                                                                                                                //if(    (((temp & ~(buttons)) & (lastBtns & temp)) == temp)                    && lastBtns)
        return 1; 
    return 0;
}

void usebtns(const char currentScreen, char * changeScreen, char * iter, int * lastBtns, int * guess_nr){
    int btns = getbtns();
    if(currentScreen == START_SCREEN){
        if(buttonPressed(1,btns, lastBtns)){ //(btns & 0x8){
            *changeScreen = 1;
        }
    }else if(currentScreen == GUESS_SCREEN){
        if(buttonPressed(1,btns, lastBtns)){
            *iter = 1;
        }
    }else if(currentScreen == LOSE_SCREEN){
        if(buttonPressed(1,btns, lastBtns)){
            *changeScreen = 0;
            *guess_nr = 1; // To adjust for screen update
            *iter = 0;
        }
    }
    *lastBtns = btns;
}



/* This function is called repetitively from the main program */
/*
 * There will be several types of pictures that will be displayed.
 * 
 */
void mastermind(void)
{   
    char currentScreen = 0;
    char changeScreen = 0;
    char iter = 0;
    int lastBtns = 0;
    int guess_nr = 1;
    int current_guess [4]  = {0,0,1,0};

    int symbol_index = 1;
    while(1){
        


        if(timer() && 5 > timeoutcount){
            if(iter){
                guess_nr++;
                iter = 0;
                if(guess_nr > MAX_GUESSES){
                    changeScreen = LOSE_SCREEN;
                }   
                updateScreen(currentScreen, &guess_nr, current_guess, symbol_index);
            }
            if(changeScreen != -1){
                currentScreen = changeScreen;
                changeScreen = -1;
                updateScreen(currentScreen, &guess_nr, current_guess, symbol_index);
            }
            timeoutcount = 0;
        }
        usebtns(currentScreen,&changeScreen,&iter, &lastBtns, &guess_nr);
    }
}
