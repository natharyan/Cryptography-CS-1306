/*****Please include following header files*****/
// stdlib.h
// string.h
// math.h
/***********************************************/

#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
	int Key;
	char Value;
} KeyValuePair;

int compare(const void* first, const void* second) {
	return ((KeyValuePair*)first)->Value - ((KeyValuePair*)second)->Value;
}

char** Create2DArray(int rowCount, int colCount) {
	char** rArray = (char**)malloc(sizeof(char*) * rowCount);

	for (int i = 0; i < rowCount; i++) {
		rArray[i] = (char*)malloc(sizeof(char) * colCount);
	}

	return rArray;
}

char* PadRight(char* str, int max, char padChar) {
	int strLen = strlen(str);
	char* output = (char*)malloc(sizeof(char*) * max);

	if (strLen < max) {
		int padLen = max - strLen;
		for (int i = 0; i < max; ++i)
			output[i] = i < strLen ? str[i] : padChar;
	}

	output[max] = '\0';
	return output;
}

int* GetShiftIndexes(char* key)
{
	int keyLength = strlen(key);
	int* indexes = (int*)malloc(sizeof(int) * keyLength);
	KeyValuePair* sortedKey = (KeyValuePair*)malloc(sizeof(KeyValuePair) * keyLength);
	int i;

	for (i = 0; i < keyLength; ++i)
		sortedKey[i].Key = i;
		sortedKey[i].Value = key[i];

	qsort(sortedKey, keyLength, sizeof(KeyValuePair), compare);
	i = 0;

	for (int i = 0; i < keyLength; ++i)
		indexes[sortedKey[i].Key] = i;

	return indexes;
}

char* Encipher(char* input, char* key, char padChar)
{
	int totalChars = strlen(input);
	int keyLength = strlen(key);
	input = (totalChars % keyLength == 0) ? input : PadRight(input, totalChars - (totalChars % keyLength) + keyLength, padChar);
	totalChars = strlen(input);
	char* output = (char*)malloc(sizeof(char) * totalChars);
	int totalColumns = keyLength;
	int totalRows = (int)ceil((double)totalChars / totalColumns);
	char** rowChars = Create2DArray(totalRows, totalColumns);
	char** colChars = Create2DArray(totalColumns, totalRows);
	char** sortedColChars = Create2DArray(totalColumns, totalRows);
	int currentRow, currentColumn, i, j;
	int* shiftIndexes = GetShiftIndexes(key);

	for (i = 0; i < totalChars; ++i)
	{
		currentRow = i / totalColumns;
		currentColumn = i % totalColumns;
		rowChars[currentRow][currentColumn] = input[i];
	}

	for (i = 0; i < totalRows; ++i)
		for (j = 0; j < totalColumns; ++j)
			colChars[j][i] = rowChars[i][j];

	for (i = 0; i < totalColumns; ++i)
		for (j = 0; j < totalRows; ++j)
			sortedColChars[shiftIndexes[i]][j] = colChars[i][j];

	for (i = 0; i < totalChars; ++i)
	{
		currentRow = i / totalRows;
		currentColumn = i % totalRows;
		output[i] = sortedColChars[currentRow][currentColumn];
	}

	output[totalChars] = '\0';
	return output;
}

char* Decipher(char* input, char* key)
{
	int keyLength = strlen(key);
	int totalChars = strlen(input);
	char* output = (char*)malloc(sizeof(char*) * totalChars);
	int totalColumns = (int)ceil((double)totalChars / keyLength);
	int totalRows = keyLength;
	char** rowChars = Create2DArray(totalRows, totalColumns);
	char** colChars = Create2DArray(totalColumns, totalRows);
	char** unsortedColChars = Create2DArray(totalColumns, totalRows);
	int currentRow, currentColumn, i, j;
	int* shiftIndexes = GetShiftIndexes(key);

	for (i = 0; i < totalChars; ++i)
	{
		currentRow = i / totalColumns;
		currentColumn = i % totalColumns;
		rowChars[currentRow][currentColumn] = input[i];
	}

	for (i = 0; i < totalRows; ++i)
		for (j = 0; j < totalColumns; ++j)
			colChars[j][i] = rowChars[i][j];

	for (i = 0; i < totalColumns; ++i)
		for (j = 0; j < totalRows; ++j)
			unsortedColChars[i][j] = colChars[i][shiftIndexes[j]];

	for (i = 0; i < totalChars; ++i)
	{
		currentRow = i / totalRows;
		currentColumn = i % totalRows;
		output[i] = unsortedColChars[currentRow][currentColumn];
	}

	output[totalChars] = '\0';
	return output;
}

char* text = "The quick brown fox jumps over the lazy dog";
char* key = "pangram";
char* cipherText;
cipherText = Encipher(text, key, '-');
char* plainText = Decipher(cipherText, key);