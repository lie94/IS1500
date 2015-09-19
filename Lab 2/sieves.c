#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6

int column = 0;
void print_number(int n){
    if(column % COLUMNS == 0){
        column = 0;
        printf("\n");
    }
    printf("%10d ", n);
    column++;
}

void print_sieves(int n){
	unsigned char notPrime [n-1];
	int i;
	notPrime[0] = 1;
	for(i = 1; i < n - 1; i++){
		notPrime[i] = 0;
	}
	for(i = 1; i < n - 1; i++){
		if(!notPrime[i]){
			print_number(i+1);
			int number = i + 1;
			int j = i + number;
			while(j < n - 1){
				notPrime[j] = 1;
				j += number;
			}
		}
		
	}
	printf("\n");
} 

int main(int argc, char *argv[]){
  	if(argc == 2)
    	print_sieves(atoi(argv[1]));
  	else
    	printf("Please state an interger number.\n");
  	return 0;
}

