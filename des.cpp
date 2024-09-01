// fiestal networks

#include <iostream>
#include "helpers/transposition_cipher.h"

using namespace std;

int main(){
    string key, str;
    int shift;
    cout << "Key: ";
    cin >> key;
    cout << "String: ";
    cin >> str;
    cout << "Shift: ";
    cin >> shift;

    TranspositionCipher transpositionCipher;

    string ciphertext = transpositionCipher.encrypt(key, str, shift);
    string plaintext = transpositionCipher.decrypt(key, ciphertext, shift);
}