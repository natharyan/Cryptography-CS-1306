//============================================================================
// Name        : byteArray.hpp
// Author      : Michael Fischer, derived from code by Ewa Syta, es633, slightly modified by Debayan Gupta, dg447
// Description : A container for raw bytes
//============================================================================

#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include "prng.hpp"

/* ByteArray is an adaptor to the standard container vector<uint8_t>.
 * It augments vector with various I/O and random byte functions.
 * The element type is a raw 8-bit byte, defined to be an unsigned char.
 */

class ByteArray : public std::vector<std::uint8_t> {
public:
    // Constructor with default size 0
    explicit ByteArray(std::size_t n = 0) : std::vector<std::uint8_t>(n) {}

    // Fill array with random bytes
    void fillRandom(Prng& rng);

    // I/O operations
    std::ostream& writeHex(std::ostream& out) const;   // writes size() bytes in hex
    std::istream& readHex(std::istream& in);           // reads size() bytes in hex
    std::ostream& writeBytes(std::ostream& out) const; // writes size() raw bytes
    std::istream& readBytes(std::istream& in);         // reads size() raw bytes
    std::istream& readAllBytes(std::istream& in);      // reads raw bytes to EOF
    
    // Bitwise operations
    ByteArray& operator^(const ByteArray& op2);  // take XOR of ByteArray
};

// Stream operator for ByteArray
inline std::ostream& operator<<(std::ostream& out, const ByteArray& ba) {
    return ba.writeBytes(out);
}