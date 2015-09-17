/*
 print-prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6

void print_number(int n);
int is_prime(int n);


int column = 0; 
int is_prime(int n){
    int i;
    for(i = 2; i*i < n + 1; i++){
        if(n % i == 0)
            return 0;
    }
    return 1;
}

void print_primes(int n){
  // Should print out all prime numbers less than 'n'
  // with the following formatting. Note that
  // the number of columns is stated in the define
  // COLUMNS


    int i;
    for(i = 2; i < n; i++){
        if(is_prime(i)){
            print_number(i);
        }
    }
  /*printf("%10d ", 2);
  printf("%10d ", 3);
  printf("%10d ", 5);
  printf("%10d ", 7);
  printf("%10d ", 11);
  printf("%10d ", 13);
  printf("\n");
  printf("%10d ", 17);
  printf("%10d ", 19);*/

  printf("\n");
}

void print_number(int n){
    if(column % COLUMNS == 0){
        column = 0;
        printf("\n");
    }
    printf("%10d ", n);
    column++;
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
  if(argc == 2)
    print_primes(atoi(argv[1]));
  else
    printf("Please state an interger number.\n");
  return 0;
}

 
