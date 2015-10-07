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

int mytime = 0x5957;
int count;
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
	
    *mTRISE = 0x00; 	// Set to 0 for output.
  	*mTRISD = *mTRISD | 0x0fe0; // Set bits 5-11 to input

    //Clock init
    count = 0;
    T2CONSET = 0x70;
    T2CONSET = 0x8000;
    PR2 = (80000000 / 256 )/ 100;
  	return;
}

int timer(void){
    if(IFS(0) & 0x100){
        IFS(0) = 0;
        count++;
    }
    if(count >= 10){
        T2CONSET = 0x8000;
        count = 0;
        return 1;
    }
    return 0;
}
/* This function is called repetitively from the main program */
void labwork( void )
{
  	//delay( 1000 );
  	int btns = getbtns();
  	if(btns){
  		int sw = getsw();
  		switch(btns){
  			case 4:
  				mytime = ((sw & 0xf) << 3*4) | (mytime & 0x0fff);	
  				break;
  			case 2:
  				mytime = ((sw & 0xf) << 2*4) | (mytime & 0xf0ff);
  				break;
  			case 1:
  				mytime = ((sw & 0xf) << 4) | (mytime & 0xff0f);
  				break;
  		}

  	} 
    if(timer()){
      	time2string( textstring, mytime );
      	display_string( 3, textstring );
      	display_update();
        tick( &mytime );
        bintick(binarytime++);
        display_image(96, icon);
    }

}
