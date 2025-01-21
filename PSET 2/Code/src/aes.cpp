/* 
 * aes.cpp
 * 
 * Created on: Jan 19, 2012
 * Author: Michael Fischer
 * Modified to use OpenSSL instead of Botan
 */
#include <fstream>
#include "aes.hpp"
#include "exception.hpp"
#include <cstring>

//-----------------------------------------------------------------------
// Define static constants
const unsigned int AES_128::keyLength = 16;
const unsigned int AES_128::blockSize = 16;

//-----------------------------------------------------------------------
AES_128::AES_128() {
    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw CryptoException("Failed to create cipher context");
    }
}

AES_128::~AES_128() {
    EVP_CIPHER_CTX_free(ctx);
}

//-----------------------------------------------------------------------
void AES_128::setKey(const ByteArray& newKey) {
    if (newKey.size() != keyLength) {
        throw CryptoException("Invalid key length");
    }
    memcpy(key, newKey.data(), keyLength);
}

void AES_128::setIV(const ByteArray& newIV) {
    if (newIV.size() != blockSize) {
        throw CryptoException("Invalid IV length");
    }
    memcpy(iv, newIV.data(), blockSize);
}

//-----------------------------------------------------------------------
// Encrypts an arbitrary size plaintext using AES-128/CBC/NoPadding.
// Precondition: the key and iv must have been previously set.
void AES_128::encrypt(const ByteArray& plaintext, ByteArray& ciphertext) {
    ByteArray padded(plaintext);
    zeroPad(padded);
    
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) {
        throw CryptoException("Failed to initialize encryption");
    }
    
    EVP_CIPHER_CTX_set_padding(ctx, 0); // Disable OpenSSL internal padding
    
    int outlen1, outlen2;
    ciphertext.resize(padded.size() + EVP_MAX_BLOCK_LENGTH);
    
    if (!EVP_EncryptUpdate(ctx, &ciphertext.front(), &outlen1,
                          padded.data(), padded.size())) {
        throw CryptoException("Failed during encryption");
    }
    
    if (!EVP_EncryptFinal_ex(ctx, &ciphertext.front() + outlen1, &outlen2)) {
        throw CryptoException("Failed to finalize encryption");
    }
    
    ciphertext.resize(outlen1 + outlen2);
}

//-----------------------------------------------------------------------
// Decrypts an arbitrary size ciphertext using AES-128/CBC/NoPadding.
// Precondition: the key and iv must have been previously set.
void AES_128::decrypt(const ByteArray& ciphertext, ByteArray& plaintext) {
    if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) {
        throw CryptoException("Failed to initialize decryption");
    }
    
    EVP_CIPHER_CTX_set_padding(ctx, 0); // Disable OpenSSL internal padding
    
    int outlen1, outlen2;
    plaintext.resize(ciphertext.size() + EVP_MAX_BLOCK_LENGTH);
    
    if (!EVP_DecryptUpdate(ctx, &plaintext.front(), &outlen1,
                          ciphertext.data(), ciphertext.size())) {
        throw CryptoException("Failed during decryption");
    }
    
    if (!EVP_DecryptFinal_ex(ctx, &plaintext.front() + outlen1, &outlen2)) {
        throw CryptoException("Failed to finalize decryption");
    }
    
    plaintext.resize(outlen1 + outlen2);
    zeroUnPad(plaintext);
}

//-----------------------------------------------------------------------
void AES_128::zeroPad(ByteArray& plaintext) {
    while (plaintext.size() % blockSize != 0) {
        plaintext.push_back(0);
    }
}

//-----------------------------------------------------------------------
void AES_128::zeroUnPad(ByteArray& plaintext) {
    while (plaintext.size() > 0 && plaintext.back() == 0) {
        plaintext.pop_back();
    }
}