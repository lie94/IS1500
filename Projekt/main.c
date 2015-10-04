#include "stdio.h"
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
   				printf("%d",i);
   				if(rightGuesses[i])
   					printf("%c", rightAnswer[i]);
   				else
   					printf("_");
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
	*deathStage++;
}
char hasWon(char rightGuesses[]){
	int i;
	for(i = 0; i < sizeof(rightGuesses)/sizeof(char); i++){
		if(!rightGuesses[i])
			return 0;
	}
	return 1;
}
int main(void) {
	printf("Please input a string value: ");
	char rightAnswer [0];
	initArray(rightAnswer);
	scanf("%s", rightAnswer);
	int i;
	for(i = 0; i < sizeof(rightAnswer)/sizeof(char); i++){
		printf("%c\n", rightAnswer[i]);
	}
	char alive = 1;
	int deathStage = 0;
	char rightGuesses[sizeof(rightAnswer)/sizeof(char)];
	char previousGuesses[29];
	initArray(rightGuesses);
	initArray(previousGuesses);
	while(1){
		printGuessed(rightAnswer,rightGuesses);
		char guess;
		guess = getGuess();	
		for(i = 0; i < sizeof(rightAnswer)/sizeof(char); i++){
			if(rightAnswer[i] == guess){
				rightGuesses[i] = 1;
			}/*else{
				printf("TEst\n");
				previousGuesses[0] = 1; //Change to all letters
				closerToDeath(&deathStage);
				break;
			}*/
		}
		printf("You have %d attempts left \n", (10 - deathStage));
		if(hasWon(rightGuesses)){
			printf("Congratulations, you have won!\n");
			break;
		}	
		if(!(deathStage < 10)){
			printf("You have lost :(\n");
			break;
		}
	}	
   	return 0;
}
