#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
	char* output = (char*)malloc(sizeof(char) * (max + 1));  // Fix: Add +1 for null terminator

	if (strLen < max) {
		int padLen = max - strLen;
		for (int i = 0; i < max; ++i)
			output[i] = (i < strLen) ? str[i] : padChar;
	}

	output[max] = '\0';  // Ensure null termination
	return output;
}

int* GetShiftIndexes(char* key) {
	int keyLength = strlen(key);
	int* indexes = (int*)malloc(sizeof(int) * keyLength);
	KeyValuePair* sortedKey = (KeyValuePair*)malloc(sizeof(KeyValuePair) * keyLength);
	int i;

	for (i = 0; i < keyLength; ++i) {
		sortedKey[i].Key = i;
		sortedKey[i].Value = key[i];
	}

	qsort(sortedKey, keyLength, sizeof(KeyValuePair), compare);

	for (i = 0; i < keyLength; ++i)
		indexes[sortedKey[i].Key] = i;

	free(sortedKey);  // Clean up memory
	return indexes;
}

char* Encipher(char* input, char* key, char padChar) {
	int totalChars = strlen(input);
	int keyLength = strlen(key);

	input = (totalChars % keyLength == 0) ? input : PadRight(input, totalChars + (keyLength - (totalChars % keyLength)), padChar);
	totalChars = strlen(input);  // Update totalChars after padding

	char* output = (char*)malloc(sizeof(char) * (totalChars + 1));
	int totalColumns = keyLength;
	int totalRows = totalChars / totalColumns;
	char** rowChars = Create2DArray(totalRows, totalColumns);
	int* shiftIndexes = GetShiftIndexes(key);

	// Fill row-wise
	for (int i = 0; i < totalChars; ++i) {
		rowChars[i / totalColumns][i % totalColumns] = input[i];
	}

	// Read column-wise in sorted order
	int currentIndex = 0;
	for (int i = 0; i < totalColumns; ++i) {
		int sortedColIndex = shiftIndexes[i];
		for (int j = 0; j < totalRows; ++j) {
			output[currentIndex++] = rowChars[j][sortedColIndex];
		}
	}

	output[totalChars] = '\0';  // Null-terminate the output

	// Clean up memory
	for (int i = 0; i < totalRows; ++i) {
		free(rowChars[i]);
	}
	free(rowChars);
	free(shiftIndexes);

	return output;
}

char* Decipher(char* input, char* key) {
	int keyLength = strlen(key);
	int totalChars = strlen(input);
	int totalColumns = keyLength;
	int totalRows = totalChars / totalColumns;
	char* output = (char*)malloc(sizeof(char) * (totalChars + 1));
	char** colChars = Create2DArray(totalRows, totalColumns);
	int* shiftIndexes = GetShiftIndexes(key);

	// Fill column-wise in sorted order
	int currentIndex = 0;
	for (int i = 0; i < totalColumns; ++i) {
		int sortedColIndex = shiftIndexes[i];
		for (int j = 0; j < totalRows; ++j) {
			colChars[j][sortedColIndex] = input[currentIndex++];
		}
	}

	// Read row-wise to get original plaintext
	currentIndex = 0;
	for (int i = 0; i < totalRows; ++i) {
		for (int j = 0; j < totalColumns; ++j) {
			output[currentIndex++] = colChars[i][j];
		}
	}

	output[totalChars] = '\0';  // Null-terminate the output

	// Clean up memory
	for (int i = 0; i < totalRows; ++i) {
		free(colChars[i]);
	}
	free(colChars);
	free(shiftIndexes);

	return output;
}

int main() {
	char text[256];
	char key[256];

	printf("Enter the key: ");
	fgets(key, sizeof(key), stdin);
	key[strcspn(key, "\n")] = '\0';  // Remove the newline character

	printf("Enter the text: ");
	fgets(text, sizeof(text), stdin);
	text[strcspn(text, "\n")] = '\0';  // Remove the newline character

	char* cipherText = Encipher(text, key, '-');
	char* plainText = Decipher(cipherText, key);

	printf("Original text: %s\n", text);
	printf("Encrypted text: %s\n", cipherText);
	printf("Decrypted text: %s\n", plainText);

	free(cipherText);
	free(plainText);

	return 0;
}