
int distorter = 42;

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
 * returns base ^ tothepower
 */
int powerTo(int base, int tothepower){
    int res =	 1;
    int i;
    for(i = 0; i < tothepower; i++){
        res *= base;
    }
    return res;
}

/**
 * Checks if two arrays are equal
 * returns 1 if they are equal, 0 otherwise
 */
char equal(int * array1, int * array2,const int length){
	int i;
	for(i = 0; i < length; i++){
		if(array1[i] != array2[i])
			return 0;
	}
	return 1;
}

/**
 * Generates a random array.
 * min and max dictates the minimal and maximum value each element in the array can have
 */
//TODO
void generateSequence(int * array, unsigned const int length, const int min,unsigned const int max, unsigned const int random_seed){
	int i;
	for(i = 0; i < length; i++){
		int a = ((random_seed ^ 3) ^ 5) / 7;
		int b = ((distorter ^ 11) - 13) ^ 17;
		distorter = (a ^ b); 
		array[i] = min + ((a + b) % (max - min));
	}
}