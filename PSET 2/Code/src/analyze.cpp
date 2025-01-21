/*
 * analyze.cpp
 *
 *  Created on: Jan 23, 2010
 *      Author: mike
 *  Modified to use OpenSSL
 */

#include <iostream>
#include <cfloat>
#include <cmath>
#include <fstream>
#include "aes.hpp"
#include "analyze.hpp"
#include "exception.hpp"
#include "dist.hpp"

using namespace std;

class FreqDist : public Distribution {
public:
    FreqDist() {}
    
    void calcFreq(const ByteArray& data) {
        int freq[ALPHABETSIZE] = {0};
        int sum = 0;
        
        // Count occurrences of each byte
        for (size_t i = 0; i < data.size(); i++) {
            freq[data[i]]++;
            sum++;
        }
        
        // Convert to probabilities and store using readFreq
        if (sum > 0) {
            string tempFile = "temp.freq";
            ofstream fout(tempFile);
            for (int i = 0; i < ALPHABETSIZE; i++) {
                fout << freq[i] << "\n";
            }
            fout.close();
            
            readFreq(tempFile.c_str());
            remove(tempFile.c_str());
        }
    }
};

//-------------------------------------------------------------------
Analyze::Analyze() :
    key(AES_128::keyLength), iv(AES_128::blockSize) {
    for (unsigned k = 0; k < NUM_KEY_SHARES; k++) {
        keyShare[k].resize(AES_128::keyLength);
    }
    cipher.setIV(iv); // Initialize the cipher with IV
}

//-------------------------------------------------------------------
// Calculate divergence between decrypted text frequency and expected frequency
double Analyze::divergence(const ByteArray& decrypted) const {
    FreqDist decryptedDist;
    decryptedDist.calcFreq(decrypted);
    
    double div = 0.0;
    for (int i = 0; i < ALPHABETSIZE; i++) {
        double diff = decryptedDist[i] - dist[i];
        div += diff * diff;
    }
    return div;
}

//-------------------------------------------------------------------
// Try all possible key share combinations to find the master key
void Analyze::guessKey() {
    double minDivergence = DBL_MAX;
    ByteArray testKey(AES_128::keyLength);
    ByteArray tempPlaintext;

    for (unsigned i = 0; i < NUM_KEY_SHARES - 1; i++) {
        for (unsigned j = i + 1; j < NUM_KEY_SHARES; j++) {
            // XOR the two key shares to get a potential master key
            testKey = keyShare[i];
            testKey = testKey ^ keyShare[j];
            
            cipher.setKey(testKey);
            cipher.setIV(iv);
            
            try {
                cipher.decrypt(ciphertext, tempPlaintext);
                double currentDivergence = divergence(tempPlaintext);
                
                // Update if this is the best key found so far
                if (currentDivergence < minDivergence) {
                    minDivergence = currentDivergence;
                    key = testKey;
                    keyIndex1 = i;
                    keyIndex2 = j;
                }
            } catch (const CryptoException& e) {
                // Skip invalid keys that cause decryption errors
                continue;
            }
        }
    }

    if (minDivergence == DBL_MAX) {
        throw CryptoException("Failed to find a valid key combination");
    }
}

//-------------------------------------------------------------------
void Analyze::run(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "usage: " << argv[0] << " freq key in out" << endl;
        return;
    }
    char* freqFile = argv[1];
    char* keyFile = argv[2];
    char* inFile = argv[3];
    char* outFile = argv[4];

    dist.readFreq(freqFile);

    readKeySharesFile(keyFile);

    readIVCiphertext(inFile);

    cout << "IV:\n";
    iv.writeHex(cout) << endl;

    guessKey();

    cipher.setKey(key);
    cipher.setIV(iv);
    cipher.decrypt(ciphertext, plaintext);

    cout << "Ciphertext size(bytes): " << ciphertext.size() << " " << endl << endl;

    cout << "Guessed key:\n";
    cout << "Indices " << keyIndex1 << " and " << keyIndex2 << endl;
    key.writeHex(cout) << endl;

    ofstream out(outFile);
    if (!out) {
        throw CryptoException("bruteforce: can't open plaintext file for writing");
    }
    out << plaintext;
    out.close();

    cout << "Plaintext: " << endl;
    cout << plaintext << endl;
}

//-------------------------------------------------------------------
// Read key shares from file
void Analyze::readKeySharesFile(const char* keyFile) {
    ifstream in(keyFile);
    if (!in) {
        throw CryptoException("analyze: can't open key file");
    }
    for (unsigned k = 0; k < NUM_KEY_SHARES; k++) {
        keyShare[k].readHex(in);
    }
    in.close();
}

//-------------------------------------------------------------------
// Read IV and ciphertext from file
void Analyze::readIVCiphertext(const char* inFile) {
    ifstream in(inFile);
    if (!in) {
        throw CryptoException("analyze: can't open ciphertext file for reading");
    }
    iv.readBytes(in);
    ciphertext.readAllBytes(in);
    in.close();
}