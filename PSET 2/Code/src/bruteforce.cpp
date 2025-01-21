/*
 * main.cpp
 *
 *  Created on: Jan 23, 2010
 *      Author: mike
 */

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "analyze.hpp"

int main(int argc, char* argv[]) {
    // Initialize OpenSSL
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    try {
        Analyze anal;
        anal.run(argc, argv);
    }
    catch (std::exception& e) {
        std::cerr << "exception caught\n" << e.what() << std::endl;
    }

    // Clean up OpenSSL
    EVP_cleanup();
    ERR_free_strings();
    CRYPTO_cleanup_all_ex_data();

    return 0;
}
