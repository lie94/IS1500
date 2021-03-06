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

int startPrime;

void print_sieves(int n){
	unsigned char notPrime [n-1];
	int i;
	notPrime[0] = 1;
	for(i = 1; i < n - 1; i++){
		notPrime[i] = 0;
	}
	for(i = 1; i < n - 1; i++){
		if(!notPrime[i]){
			print_number(i + 1);
			int number = i + 1;
			int j = i + number;
			while(j < n - 1){
				notPrime[j] = 1;
				j += number;
			}
		}
		
	}
		
	double total = 0;
	int primes = -1;	
	int lastindex = startPrime;	
	for(i = startPrime; i < sizeof(notPrime)/sizeof(notPrime[0]); i++){
		if(!notPrime[i]){
			primes++;
			total += i - lastindex;
			lastindex = i;		
		}
	}
	printf("\n");
	printf("StartPrimes: %d\n",startPrime + 1);
	printf("Primes: %d\n",primes);
	printf("%f\n",total / primes);
} 

int main(int argc, char *argv[]){
  	if(argc == 2)
    	print_sieves(atoi(argv[1]));
  	else if(argc == 3){
		startPrime = (atoi(argv[2]) - 1);
		print_sieves(atoi(argv[1]));	
	}else
    	printf("Please state an interger number.\n");
  	return 0;
}

