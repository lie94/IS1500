#include <stdint.h>   
#include <pic32mx.h>  
#include "mipslab.h" 

volatile int * mPORTD = (volatile int *) 0xbf8860D0;  
int getbtns(void){
	return (((*mPORTD) >> 4) & 0xe)	|((PORTF	>>	1)	&0x1);
 	//return (((*mPORTD) >> 4) & 0xe) | (PORTF 	>> 1 	& 0x1);
}
/*int getbtns(void){
	return ((*mPORTD) >> 5) & 0x7;
}*/
int getsw(void){
	return ((*mPORTD) >> 8) & 0xf;
}