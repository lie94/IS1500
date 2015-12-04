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

int guess_nr = 1;
char iter = 0;

const unsigned int START_SCREEN = 0, GUESS_SCREEN = 1, LOSE_SCREEN = 2;
const unsigned int MAX_GUESSES = 6;


int mytime = 0x5957;
int timeoutcount;
unsigned char binarytime = 0;
volatile char * mPORTE = (volatile char * )0xbf886110; 
char textstring[] = "text, more text, and even more text!";


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
    //TRISF |= 0x2;     // Make input 


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

void updateScreen(char * hasUpdated, unsigned int currentScreen){
    if(*hasUpdated == 0){
        if(currentScreen == START_SCREEN){
            display_string(1,"  Welcome to");
            display_string(2,"  MASTERMIND");
        }else if(currentScreen == GUESS_SCREEN){   
            char guess [16]= "Guess nr: ";
            concatenate(guess, 16, guess_nr); 
            display_string(0, guess);
            display_string(1,"  ");
            display_string(2,"  ");
            display_string(3,"  ");
        }else if(currentScreen == LOSE_SCREEN){
            display_string(0,"GAME OVER");
            display_string(1,"Right answer:");
            display_string(2,"//TODO");
            display_string(3,"");
        }
        display_update();
        *hasUpdated = 1;
    }
}
void usebtns(char * currentScreen){
    int btns = getbtns();
    if(*currentScreen == 0){
        if(btns){
            if(btns & 0x8){
                *currentScreen = 1;
            }
        }
    }else if(*currentScreen == 1){
        if(btns){
            if(btns & 0x1){
                iter = 1;
            }
        }
    }
}


/* This function is called repetitively from the main program */
/*
 * There will be several types of pictures that will be displayed.
 * 
 */
void mastermind( char * currentScreen, char * hasUpdated)
{
    
    char lastScreen = *currentScreen;
    
    if(timer() && 5 > timeoutcount){
        if(iter){
            guess_nr++;
            iter = 0;
            *hasUpdated = 0;
            if(guess_nr > MAX_GUESSES){
                *currentScreen = LOSE_SCREEN;
            }
        }
        updateScreen(hasUpdated, *currentScreen);
        timeoutcount = 0;
    }
    usebtns(currentScreen);
    if(lastScreen != *currentScreen){
        *hasUpdated = 0;
    }



}
