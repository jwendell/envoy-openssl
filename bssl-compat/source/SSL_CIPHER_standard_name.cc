#include <openssl/ssl.h>
#include <ossl.h>


/*
 * https://github.com/google/boringssl/blob/955ef7991e41ac6c0ea5114b4b9abb98cc5fd614/include/openssl/ssl.h#L1475
 * https://www.openssl.org/docs/man3.0/man3/SSL_CIPHER_standard_name.html
 */
extern "C" const char *SSL_CIPHER_standard_name(const SSL_CIPHER *cipher) {
  return ossl.ossl_SSL_CIPHER_standard_name(cipher);
}
