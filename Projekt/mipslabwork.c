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

#define TRUE 1
#define FALSE 0

volatile char * mPORTE = (volatile char * ) 0xbf886110; 

const unsigned int  START_SCREEN = 0, 
                    GUESS_SCREEN = 1, 
                    LOSE_SCREEN = 2,
                    PAST_GUESS = 3,
                    WRONG_SCREEN = 4,
                    INVALID_SCREEN = 5,
                    WIN_SCREEN = 6;

const unsigned int MAX_GUESSES = 6;

char SYMBOLS [] = {35,36, 43,45, 33,63,0}; // #$+-!?

const char ARROW = 62;

unsigned int timeoutcount;
unsigned int randomSeed;

/**
 * Creates a char array from the first 4 digits in the array and returns it.
 */
void createSimpleString(int * array, char * input){
    int i = 0;
    while(input[i] != 0)
        i++;
    int j;
    for(j = 0; j < 4; j++){
        input[i++] = 32;
        input[i++] = SYMBOLS[array[j]];
    }
}
/**
 * Creates a string with an arrow pointing to a symbol
 */
void createString(char * res, const int symbol_index, int * guesses){
    int i = 0;
    while(res[i] != 0 && i < 16)    
        i++;
    int j;
    for(j = 0; j < 4; j++){
        if(j == symbol_index){
            res[i++] = ARROW;
        }else{
            res[i++] = 32;
        }
        res[i++] = SYMBOLS[guesses[j]];
    }
}

void led_display(char bin_time){
    (*mPORTE) = bin_time;
}

/* Legacy-code */
void user_isr( void ){
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


/**
 * Timer that controlls the screens refreshrate, the blinking of the leds and supplies
 * a random seed for the sequence generator.
 */
int timer(void){
    if(IFS(0) & 0x100){
        IFS(0) = 0;
        timeoutcount++;
        randomSeed = (randomSeed + 2) % 100;
        return 1;
    }
    return 0;
}


/**
 * Updates the screen with the information given from the mastermind loop. Is ONLY called from the mastermind loop
 * Is not allowed to change the value from any of the inputs it is given, only allowed to display thing on the screen.
 */
void updateScreen(const unsigned int currentScreen, const int guess_nr, int * current_guess, const int symbol_index, int * rightAnswer, int * past_guess, const int past_guess_nr){
    if(currentScreen == START_SCREEN){
        display_string(0,"\t");
        display_string(1,"\tWELCOME TO");
        display_string(2,"\tMASTERMIND");
        display_string(3,"\t");
    }else if(currentScreen == GUESS_SCREEN){   
        char guess [16] = "\tGUESS NR: ";
        concatenate(guess, 16, guess_nr); 
        display_string(0, guess); 
        char input [16] = "\t";
        createString(input, symbol_index, current_guess); 
        display_string(1, input);
        display_string(2,"\t");
        display_string(3,"\t");
    }else if(currentScreen == LOSE_SCREEN){
        display_string(0,"\t\tGAME OVER");
        display_string(1,"");
        display_string(2,"\tRIGHT ANSWER:");
        char input [16] = "\t";
        createSimpleString(rightAnswer, input);
        display_string(3,input);
    }else if(currentScreen == WRONG_SCREEN){
        display_string(0,"\tWRONG ANSWER");
        char input[16] = "\tYOU HAVE ";
        concatenate(input,16,MAX_GUESSES -guess_nr);
        display_string(1,input);
        if(MAX_GUESSES - guess_nr == 1){
            display_string(2,"\tGUESS LEFT");
        }else{
            display_string(2,"\tGUESSES LEFT");
        }
        display_string(3,"");
    }else if(currentScreen == WIN_SCREEN){
        display_string(0,"\t!!!YOU WON!!!");
        display_string(1,"\tYOUR ANSWER:");
        char input [16] = "\t";
        createSimpleString(rightAnswer, input);
        display_string(2,input);
        display_string(3,"\tPLAY AGAIN?");
    }else if(currentScreen == PAST_GUESS){
        char guess [16] = "\tGUESS NR: ";
        concatenate(guess, 16, past_guess_nr); 
        display_string(0, guess); 
        char input [16] = "\t";
        createSimpleString(past_guess,input); 
        display_string(1, input);
        display_string(2,"\t");
        display_string(3,"\t");
    }else if(currentScreen == INVALID_SCREEN){
        display_string(0,"OBS! INVALID");
        display_string(1,"SWITCH VALUE");
        display_string(2,"MUST BE LOWER");
        char input [16] = "THAN ";
        concatenate(input,16, guess_nr);
        display_string(3,input);
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

const int UPDATE_SCREEN         = 0;
const int ITER_INCREASE         = 1;
const int SYMBOL_INCREASE       = 2;
const int SYMBOL_INDEX_CHANGE   = 3;
const int RESET                 = 4;
const int CHECK_ANSWER          = 5;
const int REMOVE_BLINKS         = 6;
const int GOTO_MAP              = 7;

/**
 * Returns nessesary canges with dataArray:
 * index0: updateScreen         : If non zero, update the screen to the number given
 * index1: iter                 : Increase the number associated with the guesses
 * index2: symbol_increase      : Increase the iteration of the symbols
 * index3: symbol_index_change  : Change the index of the current symbol +/- 1
 * index4: reset                : reset all varialbes
 * index5: check_answer         : Check if the current answer is correct
 * index6: remove_blinks        : Turns off the led display
 * index7: goto_map             : Changes the display to a passed guess or to an invalid screen if the index is out of reach.
 */
void usebtns(const char currentScreen, int * lastBtns, int * dataArray){
    int btns = getbtns();
    if(currentScreen == START_SCREEN){
        if(buttonPressed(1,btns, lastBtns)){ //(btns & 0x8){
            dataArray[UPDATE_SCREEN] = GUESS_SCREEN;

        }
    }else if(currentScreen == GUESS_SCREEN){
        if(buttonPressed(1,btns, lastBtns)){
            dataArray[CHECK_ANSWER] = TRUE;
        
        }else if(buttonPressed(2,btns, lastBtns)){
            dataArray[SYMBOL_INCREASE] = TRUE;
        
        }else if(buttonPressed(3,btns, lastBtns)){
            dataArray[SYMBOL_INDEX_CHANGE] = 1;

        }else if(buttonPressed(4,btns, lastBtns)){
            dataArray[SYMBOL_INDEX_CHANGE] = -1;
        
        }
    
    }else if(currentScreen == LOSE_SCREEN || currentScreen == WIN_SCREEN){
        if(buttonPressed(1,btns, lastBtns)){
            dataArray[RESET] = 1;

        }

    }else if(currentScreen == WRONG_SCREEN){
        if(buttonPressed(1,btns ,lastBtns)){
            dataArray[ITER_INCREASE] = TRUE;
            dataArray[UPDATE_SCREEN] = GUESS_SCREEN;       
            dataArray[REMOVE_BLINKS] = TRUE;

        }
    }
    // SWITCHES
    if(getsw() && (currentScreen == GUESS_SCREEN || currentScreen == INVALID_SCREEN || currentScreen == PAST_GUESS)){
        dataArray[GOTO_MAP] = getsw();

    }else if(getsw() == 0 && (currentScreen == PAST_GUESS || currentScreen == INVALID_SCREEN)){
        dataArray[UPDATE_SCREEN] = GUESS_SCREEN;        
        dataArray[REMOVE_BLINKS] = TRUE;
    }
    //if(SWITCHARNA ÄR IGÅNG OCH VI INTE ÄR VISSA SKÄRMAR)
    *lastBtns = btns;
}

/**
 * Resets all inputdata
 */
void reset(int * dataArray, int * symbol_index, int * current_guess, int * guess_nr, int * rightAnswer, const int dataArray_length){
    zeroInit(dataArray,dataArray_length);
    //dataArray[UPDATE_SCREEN] = 0;
    *symbol_index = 0;
    *guess_nr = 1;
    zeroInit(current_guess, 4);
}


/**
 * Returns how many symbols are in the sequence but not at the right place.
 */
int checkPartialMatch(int * guesses, int * valid_sequence){
    int temp = 0;
    int i,j;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            if(guesses[i] == valid_sequence[j]){
                valid_sequence[j]   = -2;
                guesses[i]          = -1;
                temp++;
                //setData(guess_info,2);
            }
        }
    }
    return temp;
}

/**
 * Returns the number of symbols that are in the right place
 */ 
int checkTotalMatches(int * guesses, int * valid_sequence){
    int temp = 0;
    int i;
    for(i = 0; i < 4; i++){
        if(guesses[i] == valid_sequence[i]){
            guesses[i]          = -1;
            valid_sequence[i]   = -2;
            temp++;
            //setData(guess_info,1);
        }
    }
    return temp;
}

/**
 * 
 */
void createBlinks(int * current_guess, int * right_answer, int * current_lit, int * current_blink ){
    int copy_guess [4];
    int copy_answer [4];
    int temp1, temp2;
    //zeroInit(game_answer,4);
    copyArray(current_guess, copy_guess, 4);
    copyArray(right_answer, copy_answer, 4);
    *current_lit    = 0;
    *current_blink  = 0;
    temp1 = checkTotalMatches(copy_guess, copy_answer);
    temp2 = checkPartialMatch(copy_guess, copy_answer);
    if (temp1 == 1)
        *current_lit = 0x80;
    else if (temp1 == 2)
        *current_lit = 0xc0;
    else if (temp1 == 3)
        *current_lit = 0xe0;
    else if (temp1 ==4)
        *current_lit = 0xf0;
    temp2 += temp1;
    if (temp2 == 1)
        *current_blink = 0x80;
    else if (temp2 == 2)
        *current_blink = 0xc0;
    else if (temp2 == 3)
        *current_blink = 0xe0;
    else if (temp2 == 4)
        *current_blink = 0xf0;
}

void mastermind(void)
{   
    char currentScreen = 0;
    
    int lastBtns = 0;
    
    int guess_nr = 1;
    
    int current_guess [4];

    int symbol_index;

    int rightAnswer [4];

    int past_guesses [MAX_GUESSES][4];
    int past_answer  [MAX_GUESSES][2]; // Given past_answer[i][0] = LIT, past_answer[i][1] = BLINKING
    int currentLit = 0;
    int currentBlink = 0;
    int dataArray[8];
    int past_guess_nr = 0;
    reset(dataArray, &symbol_index, current_guess, &guess_nr, rightAnswer,sizeof(dataArray) / sizeof(int));
    while(TRUE){
        if(1 < timeoutcount){ // LED SKA LYSA
            led_display(currentLit);
        
        }else{              // LED SKA INTE LYSA
            led_display(currentBlink);
        
        }

        if(timer() && 2 < timeoutcount){
            if(dataArray[CHECK_ANSWER] == TRUE){
                if(equal(current_guess, rightAnswer, 4)){
                    dataArray[UPDATE_SCREEN] = WIN_SCREEN;
                
                }else if(guess_nr == MAX_GUESSES){
                    dataArray[UPDATE_SCREEN] = LOSE_SCREEN;
                
                }else{
                    dataArray[UPDATE_SCREEN] = WRONG_SCREEN;
                    createBlinks(current_guess, rightAnswer, &currentLit, &currentBlink);
                    past_answer[guess_nr - 1][0] = currentLit;
                    past_answer[guess_nr - 1][1] = currentBlink;
                }
    
            }
            if(dataArray[GOTO_MAP]){
                if(dataArray[GOTO_MAP] < guess_nr){
                    dataArray[UPDATE_SCREEN] = PAST_GUESS;
                    past_guess_nr = dataArray[GOTO_MAP];
                    currentLit      = past_answer[past_guess_nr - 1][0];
                    currentBlink    = past_answer[past_guess_nr - 1][1];

                }else{
                    dataArray[UPDATE_SCREEN] = INVALID_SCREEN;
                    dataArray[REMOVE_BLINKS] = TRUE;
                }
                dataArray[GOTO_MAP] = 0;
            }


            if(dataArray[REMOVE_BLINKS] == TRUE ){
                currentLit = 0;
                currentBlink = 0;

            }
            if(dataArray[ITER_INCREASE] == TRUE){
                //past_guesses[guess_nr - 1] = 5;/current_guess; 
                copyArray(current_guess, past_guesses[guess_nr - 1], 4);
                guess_nr++;
                if(guess_nr > MAX_GUESSES){
                    dataArray[UPDATE_SCREEN] = LOSE_SCREEN;
            
                }   
            
            }
            if(dataArray[RESET]){
                reset(dataArray, &symbol_index, current_guess, &guess_nr, rightAnswer,sizeof(dataArray) / sizeof(int));
            
            }
            if(dataArray[SYMBOL_INDEX_CHANGE] != FALSE){
                symbol_index += dataArray[SYMBOL_INDEX_CHANGE];
                if(symbol_index < 0){
                    symbol_index = 3;
                
                }else if(symbol_index > 3){
                    symbol_index = 0;
                
                }
            
            }
            if(dataArray[SYMBOL_INCREASE] == TRUE){
                current_guess[symbol_index]++;
                if(current_guess[symbol_index] > 5){
                    current_guess[symbol_index] = 0;
                
                }

            }
            if(dataArray[UPDATE_SCREEN] != -1){
               if(currentScreen == START_SCREEN && dataArray[UPDATE_SCREEN] == GUESS_SCREEN)
                    generateSequence(rightAnswer, 4, 0, 6, randomSeed);
               currentScreen = dataArray[UPDATE_SCREEN];
            
            }
            updateScreen(currentScreen, guess_nr, current_guess, symbol_index, rightAnswer, past_guesses[past_guess_nr - 1], past_guess_nr);
            zeroInit(dataArray, sizeof(dataArray) / sizeof(int));;
            dataArray[UPDATE_SCREEN] = -1;
            timeoutcount = 0;
        }
        usebtns(currentScreen, &lastBtns, dataArray);
    }
}
