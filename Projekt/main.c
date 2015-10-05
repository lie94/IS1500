#include "stdio.h"
#include <unistd.h>

void initArray(char inpArr []){
	int i;
	for(i = 0; i < sizeof(inpArr)/sizeof(char); i++){
   		inpArr[i] = 0;
   	}
}
void printGuessed(char rightAnswer [], char rightGuesses []){
	int i;
	if(rightGuesses[0])
   					printf("%c", rightAnswer[0]);
   				else
   					printf("_");
	for(i = 1; i < sizeof(rightAnswer)/sizeof(char); i++){
   				if(rightGuesses[i])
   					printf(" %c", rightAnswer[i]);
   				else if(rightAnswer[i] != 0)
   					printf(" _");
   	}
   	printf("\n");
}
char getGuess(){
	printf("Make a guess: ");
	char res;
	scanf("\n%c", &res);
	
	printf("\n");
	return res;
}
void closerToDeath(int * deathStage){
	*deathStage += 1;
}
char hasWon(char rightGuesses[], char rightAnswer []){
	int i;
	for(i = 0; i < sizeof(rightGuesses)/sizeof(char); i++){
		if(!rightGuesses[i] && rightAnswer[i] != 0)
			return 0;
	}
	return 1;
}
void clearPage(){
	int i;
	for(i = 0; i < 100; i++){
		printf("\n");
	}
}
char hasGuessed(char guesses [], char  guess){
	int i;
	for(i = 0; i < sizeof(guesses) / sizeof(char); i++){
		if(guesses[i] == guess)
			return 1;
	}	
	return 0;
}
/**
  * A B | Y
  * 0 0 | 0
  * 0 1 | 1
  * 1 0 | 1
  * 1 1 | 1
  */
void printPreviouslyGuessed(char guessed []){
	if(guessed[0] != 0){
		printf("Your previous guesses:");
		int i = 0;
		while(guessed[i] != 0)
			printf(" %c", guessed[i++]);
		printf("\n");
	}
}
int main(void) {
	printf("Please input a string value: ");
	char rightAnswer [32];
	initArray(rightAnswer);
	scanf("%s", rightAnswer);
	clearPage();
	int i;

	char alive = 1;
	int deathStage = 0;
	char rightGuesses[sizeof(rightAnswer)/sizeof(char)];
	char previousGuesses[10];
	initArray(rightGuesses);
	initArray(previousGuesses);
	while(1){
		printGuessed(rightAnswer,rightGuesses);
		printPreviouslyGuessed(previousGuesses);
		char guess, guessedRight, repeat;
		do{
			guessedRight = 0;
			guess = getGuess();	
			for(i = 0; i < sizeof(rightAnswer)/sizeof(char); i++){
				if(rightAnswer[i] == guess){
					rightGuesses[i] = 1;
					guessedRight = 1;
				}
			}
			repeat = hasGuessed(previousGuesses,guess);
			if(repeat)
				printf("You have already guessed %c, make another guess. \n", guess);
				printPreviouslyGuessed(previousGuesses);
		}while(repeat);
		

		if(!guessedRight){
			previousGuesses[deathStage] = guess;
			closerToDeath(&deathStage);
		}
		if(hasWon(rightGuesses, rightAnswer)){
			printf("Congratulations, you have won!\n");
			printf("The word was: %s\n",rightAnswer);
			break;
		}else if(!(deathStage < 10)){
			printf("You have lost :(\n");
			printf("The word was: %s\n",rightAnswer);
			break;
		}else{
			printf("You have %d attempts left \n", (10 - deathStage));
			
		}

	}	
   	return 0;
}
