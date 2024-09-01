#ifndef TRANSPOSITIONCIPHER_H
#define TRANSPOSITIONCIPHER_H

#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

class TranspositionCipher {
public:
    TranspositionCipher();  // Constructor
    string encrypt(string key, string text, int shift);  // Method to perform encryption
    string decrypt(string key, string text, int shift);  // Method to perform decryption
};

#endif // TRANSPOSITIONCIPHER_H
