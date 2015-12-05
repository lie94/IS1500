#ifndef SIDEFUNCS
#define SIDEFUNCS

// Fungerar bara för siffor mindre än 10	
void concatenate(char * string, const unsigned int string_length, const unsigned int number);

/**
 * Copies all elements from the array copyFrom to the array copyTo.
 */
void copyArray(int * copyFrom, int * copyTo, int length);

/**
 * Initiates the array with all zeros
 */
void zeroInit(int * array, int length);

/**
 * returns base ^ tothepower
 */
int powerTo(int base, int tothepower);

/**
 * Generates a random array.
 * min and max dictates the minimal and maximum value each element in the array can have
 */
//TODO
void generateSequence(int * array, unsigned const int length, const int min,unsigned const int max);
#endif