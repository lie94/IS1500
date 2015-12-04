
// Fungerar bara för siffor mindre än 10	
void concatenate(char * string, const unsigned int string_length, const unsigned int number){
	int i = 0;
	while(string[i] != 0 && i < string_length)
		i++;
	string[i] = (char) (number + 48); // 48 is the character code for 0, 49 is the character code for 1 and so on. 
}

/**
 * Copies all elements from the array copyFrom to the array copyTo.
 */
void copyArray(int * copyFrom, int * copyTo, int length){
	int i;
	for(i = 0; i < length; i ++)
		copyTo[i] = copyFrom[i];
}

/**
 * Initiates the array with all zeros
 */
void zeroInit(int * array, int length){
	int i;
	for(i = 0; i < length; i++){
		array[i] = 0;
	}
}

/**
 * Generates a random array.
 * min and max dictates the minimal and maximum value each element in the array can have
 */
//TODO
void generateSequence(int * array, unsigned const int length, const int min,unsigned const int max){
	int i;
	for(i = 0; i < length; i++)
		array[i] = i;
}