#include <stdio.h>
#include <stdlib.h>
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

int* GetShiftIndexes(char* key) {
    int keyLength = strlen(key);
    int* indexes = (int*)malloc(sizeof(int) * keyLength);
    KeyValuePair* sortedKey = (KeyValuePair*)malloc(sizeof(KeyValuePair) * keyLength);
    
    for (int i = 0; i < keyLength; ++i) {
        sortedKey[i].Key = i;
        sortedKey[i].Value = key[i];
    }
    
    qsort(sortedKey, keyLength, sizeof(KeyValuePair), compare);
    
    for (int i = 0; i < keyLength; ++i)
        indexes[i] = sortedKey[i].Key;
    
    free(sortedKey);  // Clean up memory
    return indexes;
}

char* Encipher(char* input, char* key) {
    int totalChars = strlen(input);
    int keyLength = strlen(key);
    int totalColumns = keyLength;
    int totalRows = totalChars / keyLength;
    int extraChars = totalChars % keyLength;  // Determine how many columns have an extra character

    // Create a 2D array to hold characters
    char** rowChars = Create2DArray(totalRows + 1, totalColumns);  // 1 extra row to account for extra chars

    // Fill the matrix row-wise with the input characters
    int currentIndex = 0;
    for (int i = 0; i < totalRows + 1; ++i) {
        for (int j = 0; j < totalColumns; ++j) {
            if ((i == totalRows && j >= extraChars) || currentIndex >= totalChars) {
                rowChars[i][j] = '\0';  // Fill the last row and beyond extraChars with null
            } else {
                rowChars[i][j] = input[currentIndex++];
            }
        }
    }

    // Get the shift indexes based on the sorted key
    int* shiftIndexes = GetShiftIndexes(key);

    // Encipher the text by reading column-wise according to the sorted key order
    char* output = (char*)malloc(sizeof(char) * (totalChars + 1));
    currentIndex = 0;
    
    for (int i = 0; i < totalColumns; ++i) {
        int sortedColIndex = shiftIndexes[i];
        for (int j = 0; j < totalRows + 1; ++j) {
            if (rowChars[j][sortedColIndex] != '\0') {
                output[currentIndex++] = rowChars[j][sortedColIndex];
            }
        }
    }

    output[currentIndex] = '\0';  // Null-terminate the output

    // Clean up memory
    for (int i = 0; i < totalRows + 1; ++i) {
        free(rowChars[i]);
    }
    free(rowChars);
    free(shiftIndexes);

    return output;
}

char* Decipher(char* input, char* key) {
    int totalChars = strlen(input);
    int keyLength = strlen(key);
    int totalColumns = keyLength;
    int totalRows = totalChars / keyLength;
    int extraChars = totalChars % keyLength;  // Determine how many columns have an extra character

    // Create a 2D array to hold characters column by column
    char** colChars = Create2DArray(totalRows + 1, totalColumns);  // 1 extra row to account for extra chars

    // Get the shift indexes based on the sorted key
    int* shiftIndexes = GetShiftIndexes(key);

    // Fill the columns in the correct order based on the sorted key
    int currentIndex = 0;
    for (int i = 0; i < totalColumns; ++i) {
        int sortedColIndex = shiftIndexes[i];
        for (int j = 0; j < totalRows + 1; ++j) {
            if (j == totalRows && sortedColIndex >= extraChars) {
                colChars[j][sortedColIndex] = '\0';  // Fill last row of non-extra columns with null
            } else if (currentIndex < totalChars) {
                colChars[j][sortedColIndex] = input[currentIndex++];
            }
        }
    }

    // Read row-wise to reconstruct the original plaintext
    char* output = (char*)malloc(sizeof(char) * (totalChars + 1));
    currentIndex = 0;
    
    for (int j = 0; j < totalRows + 1; ++j) {
        for (int i = 0; i < totalColumns; ++i) {
            if (colChars[j][i] != '\0' && currentIndex < totalChars) {
                output[currentIndex++] = colChars[j][i];
            }
        }
    }

    output[currentIndex] = '\0';  // Null-terminate the output

    // Clean up memory
    for (int i = 0; i < totalRows + 1; ++i) {
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

    char* cipherText = Encipher(text, key);
    char* plainText = Decipher(cipherText, key);

    printf("Original text: %s\n", text);
    printf("Encrypted text: %s\n", cipherText);
    printf("Decrypted text: %s\n", plainText);

    free(cipherText);
    free(plainText);

    return 0;
}
