/* 
 * aes.hpp
 * 
 * Created on: Jan 19, 2012
 * Author: Michael Fischer
 * Modified to use OpenSSL instead of Botan
 */
#pragma once

#include <openssl/evp.h>
#include <openssl/aes.h>
#include "bytearray.hpp"

using namespace std;

/* Wrapper class for OpenSSL's implementation of AES-128/CBC.
 * It encrypts and decrypts raw ByteArray data.
 * Must set the key and initialization vector before calling encrypt
 * or decrypt.
 */
class AES_128 {
public:
    static const unsigned int keyLength;
    static const unsigned int blockSize;

private:
    EVP_CIPHER_CTX *ctx;
    unsigned char key[AES_BLOCK_SIZE];
    unsigned char iv[AES_BLOCK_SIZE];
    
public:
    AES_128();
    ~AES_128();
    
    void encrypt(const ByteArray& plaintext, ByteArray& ciphertext);
    void decrypt(const ByteArray& ciphertext, ByteArray& plaintext);
    void zeroPad(ByteArray& plaintext);
    void zeroUnPad(ByteArray& plaintext);
    
    void setKey(const ByteArray& key);
    void setIV(const ByteArray& iv);
};