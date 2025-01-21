//============================================================================
// Name        : byteArray.cpp
// Author      : Michael Fischer, derived from code by Ewa Syta, es633, slightly modified by Debayan Gupta, dg447
// Description : A container for raw bytes
//============================================================================

#include <iostream>
#include <iomanip>
#include "bytearray.hpp"
#include "exception.hpp"

//-----------------------------------------------------------------------
void ByteArray::fillRandom(Prng& rng) {
    if (!empty()) {
        rng.nextBytes(&front(), size());
    }
}

//-----------------------------------------------------------------------
// Writes the array in hex
std::ostream& ByteArray::writeHex(std::ostream& out) const {
    for (std::size_t i = 0; i < size(); ++i) {
        if (i > 0 && i % 8 == 0) {
            out << '\n';
        }
        out << std::setw(2) << std::hex << std::setfill('0') 
            << static_cast<unsigned>(at(i)) << ' ' << std::dec;
        
        if (!out.good()) {
            throw CryptoException("writeHex: I/O error");
        }
    }
    return out << '\n';
}

//-----------------------------------------------------------------------
// Reads the array in hex
// Tries to fill the array to its current size.
std::istream& ByteArray::readHex(std::istream& in) {
    for (auto& byte : *this) {
        unsigned value;
        in >> std::hex >> value;
        byte = static_cast<std::uint8_t>(value);
        
        if (!in.good()) {
            throw CryptoException("readHex: unexpected eof or bad data");
        }
    }
    return in;
}

//-----------------------------------------------------------------------
// Writes raw bytes
std::ostream& ByteArray::writeBytes(std::ostream& out) const {
    for (const auto& byte : *this) {
        out.put(static_cast<char>(byte));
        if (!out.good()) {
            throw CryptoException("writeBytes: I/O error");
        }
    }
    return out;
}

//-----------------------------------------------------------------------
// Reads raw bytes
// Tries to fill the array to its current size
std::istream& ByteArray::readBytes(std::istream& in) {
    for (auto& byte : *this) {
        char ch;
        in.get(ch);
        if (!in.good()) {
            throw CryptoException("readBytes: unexpected eof or bad data");
        }
        byte = static_cast<std::uint8_t>(ch);
    }
    return in;
}

//-----------------------------------------------------------------------
// Reads raw bytes
// Read all bytes into array.
std::istream& ByteArray::readAllBytes(std::istream& in) {
    clear();
    char ch;
    while (in.get(ch)) {
        push_back(static_cast<std::uint8_t>(ch));
    }
    
    if (!in.eof()) {
        throw CryptoException("readBytes: I/O error");
    }
    return in;
}

//-----------------------------------------------------------------------
// take XOR of ByteArray
ByteArray& ByteArray::operator^(const ByteArray& op2) {
    if (size() != op2.size()) {
        throw CryptoException("XOR of ByteArray of unequal lengths not supported");
    }
    
    for (std::size_t i = 0; i < size(); ++i) {
        at(i) ^= op2[i];
    }
    return *this;
}