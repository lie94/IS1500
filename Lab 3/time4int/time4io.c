#include <stdint.h>   
#include <pic32mx.h>  
#include "mipslab.h" 

volatile int * mPORTD = (volatile int *) 0xbf8860D0;  
int getbtns(void){
	return ((*mPORTD) >> 5) & 0x7;
}
int getsw(void){
	
	return ((*mPORTD) >> 8) & 0xf;
}