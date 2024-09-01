#include "transposition_cipher.h"

using namespace std;

TranspositionCipher::TranspositionCipher() {}

string TranspositionCipher::encrypt(string key, string text, int shift) {
    int key_size = key.size();
    map<char, vector<char> > transpositionMatrix;
    int row = 0;
    int i = 0;

    // Create transposition matrix
    while(i < text.size()) {
        transpositionMatrix[key[(row * shift + i) % key_size]].push_back(text[i]);
        if (i % key_size == key_size - 1) {
            ++row;
        }
        ++i;
    }

    // Fill remaining row columns
    while(i % key_size != 0) {
        transpositionMatrix[key[(row * shift + i) % key_size]].push_back(char(65 + i % 6));
        ++i;
        if(i % key_size == 0) {
            ++row;
        }
    }

    cout << "key: " << key << endl;

    cout << endl;

    cout << "transposition matrix:" << endl;

    // Print transposition matrix
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < key_size; j++) {
            cout << transpositionMatrix[key[j]][i] << ' ';
        }
        cout << endl;
    }

    // Sorting using multimap
    multimap<char, vector<char> > transposed;
    for(auto val: transpositionMatrix) {
        transposed.insert(make_pair(val.first, val.second));
    }

    string ciphertext = "";
    cout << "transposed:" << endl;

    // Print transposed matrix
    for (int i = 0; i < row; i++) {
        for (multimap<char, vector<char> >::iterator val = transposed.begin(); val != transposed.end(); val++) {
            cout << val->second[i] << ' ';
            ciphertext += val->second[i];
        }
        cout << endl;
    }

    cout << "ciphertext: " << ciphertext << endl;

    return ciphertext;
}

string TranspositionCipher::decrypt(string key, string ciphertext, int shift) {
    int key_size = key.size();
    map<char, vector<char> > transpositionMatrix;
    int row = 0;
    int i = 0;

    string ordered_key = key;
    sort(ordered_key.begin(), ordered_key.end());

    // Create transposition matrix
    while(i < ciphertext.size()) {
        transpositionMatrix[ordered_key[(row * shift + i) % key_size]].push_back(ciphertext[i]);
        if (i % key_size == key_size - 1) {
            ++row;
        }
        ++i;
    }
 
    string plaintext = "";

    // Print transposed matrix
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < key_size; j++) {
            cout << transpositionMatrix[key[j]][i] << ' ';
            plaintext += transpositionMatrix[key[j]][i];
        }
        cout << endl;
    }

    cout << "plaintext: " << plaintext << endl;

    return plaintext;
    
}
