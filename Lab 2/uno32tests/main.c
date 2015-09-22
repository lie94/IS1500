/* main.c

   This file written 2015 by F Lundevall and David Broman

   Latest update 2015-09-15 by David Broman

   For copyright and licensing, see file COPYING */

#include <stddef.h>   /* Declarations of integer sizes and the like, part 1 */
#include <stdint.h>   /* Declarations of integer sizes and the like, part 2 */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

void saveinfo(char s[], void * a, size_t ns );
void showinfo(void);
void u32init(void);

int gv; /* Global variable. */
int in = 3; /* Global variable, initialized to 4711. */

void fun(int param)
{
  param++;
  saveword( "AF1: param", &param );
  gv = param; /* Change the value of a global variable. */
}

/* This is the main function */
int main()
{
  /* Local variables. */
  int m;
  int * p; /* Declare p as pointer, so that p can hold an address. */
  char c[ 4 ] = "Hej"; 

  /* Do some calculation. */
  gv = 4;
  m = gv + in;

  /* Check the addresses and values of some variables and stuff */
  saveword( "AM1: gv", &gv );
  saveword( "AM2: in", &in );
  saveword( "AM3: fun", &fun );
  saveword( "AM4: main", &main );

  p = &m;

  /* Check p and m */
  saveword( "AM5: p", &p );
  saveword( "AM6: m", &m );

  /* Change *p */

  *p = *p + 1;

  /* Check p and m */
  saveword( "AM7: p", &p );
  saveword( "AM8: m", &m );

  p = (int*)c;   /* Casting a char pointer to a integer pointer */

  saveword( "AM9: p", &p );

  savebyte( "AM10: c[0]", &c[0] );
  savebyte( "AM11: c[1]", &c[1] );
  savebyte( "AM12: c[2]", &c[2] );
  savebyte( "AM13: c[3]", &c[3] );

  *p = 0x1234abcd; /* It starts to get interesting... */

  savebyte( "AM14: c[0]", &c[0] );
  savebyte( "AM15: c[1]", &c[1] );
  savebyte( "AM16: c[2]", &c[2] );
  savebyte( "AM17: c[3]", &c[3] );

  fun(m);

  /* Re-check the addresses and values of m and gv */
  saveword( "AM18: m", &m );
  saveword( "AM19: gv", &gv );

  showinfo();
}
