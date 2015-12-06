#include "sidefuncs.c"



const int ARRAY_LENGTH = 4;

void getGuess(int * res, int ARRAY_LENGTH){
	//int res [4];
	char repeat;
	do{
		repeat = 0;
		//printf("The guess should be made in the pattern: \\d \\d \\d \\d\n");
		//printf("And only contain numbers 0-6\n");
		//printf("Make a guess: ");
		int i;
		for(i = 0; i < 4; i++){
			if(scanf("\n%d", &res[i])){	
				if(res[i] > 5 || res[i] < 0){
					repeat = 1;
					//printf("Non vailid numbers, try again.\n");
				}
			}
		}
		
	}while(repeat);
	//printf("\n");
}
/*
 * Sets the type of right answer in guess_info.
 * Number 1 means that there is a correctly positioned symbol
 * Number 2 means that there is a symbol that belongs in the sequence
 */
void setData(int * guess_info, int data){
	int i = 0;
	while(guess_info[i] != 0)
		i++;
	guess_info[i] = data;

}	




void checkTotalMatches(int * guesses, int * valid_sequence, int * guess_info){
	int i;
	for(i = 0; i < ARRAY_LENGTH; i++){
		if(guesses[i] == valid_sequence[i]){
			guesses[i] 			= -1;
			valid_sequence[i] 	= -2;
			setData(guess_info,1);
		}
	}
}

void checkPartialMatch(int * guesses, int * valid_sequence, int * guess_info){
	int i,j;
	for(i = 0; i < ARRAY_LENGTH; i++){
		for(j = 0; j < ARRAY_LENGTH; j++){
			if(guesses[i] == valid_sequence[j]){
				valid_sequence[j] 	= -2;
				guesses[i] 			= -1;
				setData(guess_info,2);
			}
		}
	}
}
/*void //printArray(int * array, int length){
	int i;
	for(i = 0; i < length; i++)
		//printf("%d ", array[i]);
	//printf("\n");
}*/

int main(void) {
	int i;
	unsigned int valid_sequence [ARRAY_LENGTH];
	generateSequence(valid_sequence,ARRAY_LENGTH,0,6);
	//printArray(valid_sequence, ARRAY_LENGTH);
	int ** guesshistory;
	const int MAX_GUESSES = 6;
	int guess_nr = 0;
	guesshistory = (int **) malloc(6); 
	while(1){
		//printf("Detta är gissning nr: %d \n", ++guess_nr);
		int guess [ARRAY_LENGTH];
		//guess = (int *) malloc(4);
		getGuess(guess, ARRAY_LENGTH); 										//GOTO MAKEGUESS-SCREEN		 
		int * guess_info;
		guess_info = (int *) malloc(4);
		zeroInit(guess_info, ARRAY_LENGTH); 	
		char rightGuess = 1;
		////printArray(valid_sequence,	ARRAY_LENGTH);
		////printArray(guess,			ARRAY_LENGTH);
		for(i = 0; i< 4; i++){
			if (valid_sequence[i] != guess[i])
				rightGuess = 0;
		}	
																		
		if (rightGuess){												//GOTO WIN-SCREEN
			//printf("You guessed right!\n");
			return 0;
		}
		int * valid_sequence_copy 	= (int *) malloc(ARRAY_LENGTH);
		copyArray(valid_sequence, valid_sequence_copy, ARRAY_LENGTH);
		checkTotalMatches(guess, valid_sequence_copy, guess_info);
		checkPartialMatch(guess, valid_sequence_copy, guess_info);
		//for(i = 0; i < ARRAY_LENGTH; i++)
		//	//printf("%d\n", guess_info[i]);
		i = 0;
		while(guess_info[i] != 0 && i < 4)
			//printf("%d", guess_info[i++]);
		//printf("\n");

		if(guess_nr == MAX_GUESSES){ 								//GOTO 
			//printf("Du han fick inte rätt gissning inom tisgränsen.\n");
			return 0;
		}
		free(valid_sequence_copy);
		free(guess_info);
	}	
   	return 0;
}
