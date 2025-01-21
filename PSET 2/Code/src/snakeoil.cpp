/*
 * main.cpp
 *
 *  Created on: Jan 21, 2012
 *      Author: Michael Fischer
 *      Derived from code by Ewa Syta
 *      Solutions and edits by Debayan Gupta
 *      Modified to use OpenSSL
 */
#include <ctime>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "crypto.hpp"

int main(int argc, char* argv[]) {
    // Initialize OpenSSL
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ERR_load_crypto_strings();

    try {
        unsigned int seed = time(NULL);
        Crypto toplevel(seed);
        toplevel.run(argc, argv);
    }
    catch (std::exception& e) {
        std::cerr << "exception caught\n" << e.what() << std::endl;
    }

    // Cleanup OpenSSL
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    return 0;
}