/*
 * exception.hpp
 *
 *  Created on: Jan 21, 2012
 *      Author: Michael Fischer
 */

#pragma once

//


class CryptoException: public std::exception {
private:
    std::string msg;
public:
    CryptoException(std::string msg) :
        msg(msg) {
    }
    virtual ~CryptoException() throw() {}
    const char* what() const throw() {
        return msg.c_str();
    }
};
