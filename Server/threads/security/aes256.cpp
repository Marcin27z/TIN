
#include <stdio.h>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <initializer_list>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/engine.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include "aes256.h"
#include "base64.h"
#include "md5.h"

#define PRINT_OPENSSL_ERRORS

using namespace std;

void handleOpenSSLErrors(void)
{
#ifdef PRINT_OPENSSL_ERRORS
    ERR_print_errors_fp(stderr);
#endif
    //abort();
}

/**
 * Encrypt string using passphrase
 *
 * @param text Encrypted base64 encoded string
 * @param passphrase Passphrase
 * @return Decrypted string
 */
string AES256::encrypt(string text, string passphrase)
{
    unsigned char salt_c[8];
    memset(salt_c, 0, 8);
    string key;
    string iv;
    for (int i = 0; !RAND_bytes(salt_c, 8) && i < 50; i++);
    string salt = string((const char *) salt_c);
    text = text.substr(0, text.length()-1);



    int j = 50;
    do {
        string dx = "";
        string salted = "";

        while (salted.length() < 48) {
            dx = MD5(dx + passphrase + salt).binary();
            salted += dx;
        }

        key = salted.substr(0, 32);
        iv = salted.substr(32, 16);
    }
    while ((key.length() != 32 || iv.length() != 16) && j--);



    key = {87, -128, 56, -19, 27, -87, -108, 41, -38, -45, 81, -44, -64, -41, -109, 84, -84, -96, 89, 109, 100, -44, 0, 65, -16, -44, -11, 114, -64, 79, -64, -34};
    iv = {0, 36, 14, 99, 118, -45, -79, 113, 117, -17, -66, 47, 99, -51, -11, -36};
    std::cout << "Do szyfrowania wchodzi: " + text << std::endl;
    string outText = encryptFinal((unsigned char *)text.c_str(), text.length(),
                                  (unsigned char *)key.c_str(), (unsigned char *)iv.c_str());

    string encrypted = "Salted__" + salt + outText;
    std::cout << "Po zakodowaniu: " << encrypted.length() << std::endl;

    std::cout << std::endl << "Text: " << std::endl;
    for(int i=0; i < outText.length(); ++i)
        std::cout <<  (int)outText[i] << " ";
    return base64_encode((const unsigned char*)encrypted.c_str(), encrypted.length());
}

/**
 * Encrypt string using key and IV
 *
 * @param text Text
 * @param text_len Text length
 * @param key Cipher key
 * @param iv Cipher IV
 * @return Encrypted string
 */
string AES256::encryptFinal(unsigned char *text, int text_len, unsigned char *key, unsigned char *iv)
{

    EVP_CIPHER_CTX *ctx;
    unsigned char *plaintexts;
    int len;
    int plaintext_len;
    unsigned char *plaintext = new unsigned char[text_len];
    bzero(plaintext, text_len);

    if (!(ctx = EVP_CIPHER_CTX_new())) {
        EVP_CIPHER_CTX_cleanup(ctx);
        handleOpenSSLErrors();
    }
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_cleanup(ctx);
        handleOpenSSLErrors();
    }

    EVP_CIPHER_CTX_set_key_length(ctx, EVP_MAX_KEY_LENGTH);
    if (1 != EVP_EncryptUpdate(ctx, plaintext, &len, text, text_len)) {
        EVP_CIPHER_CTX_cleanup(ctx);
        handleOpenSSLErrors();
    }

    plaintext_len = len;
    int pad_len;

    if (1 != EVP_EncryptFinal_ex(ctx, plaintext + len, &len)) {
        EVP_CIPHER_CTX_cleanup(ctx);
        handleOpenSSLErrors();
    }

    plaintext_len += len;
    plaintext[plaintext_len] = 0;

    EVP_CIPHER_CTX_cleanup(ctx);

    string ret = (char *) plaintext;
    delete[] plaintext;
    return ret;
}



/**
 * Decrypt encrypted string using passphrase
 *
 * @param text Encrypted base64 encoded string
 * @param passphrase Passphrase
 * @return Decrypted string
 */
string AES256::decrypt(string text, string key)
{
    string in = base64_decode(text);
    string salted = in.substr(0, 8);

    if (salted != "Salted__") {
        return "";
    }

    string salt = in.substr(8, 8);
    string iv;


    string ct = in.substr(16);

    iv = {0, 36, 19, 99, 118, -45, -79, 113, 117, -17, -66, 47, 99, -51, -11, -36};

    return decryptFinal((unsigned char *)ct.c_str(), ct.length(),
                        (unsigned char *)key.c_str(), (unsigned char *)iv.c_str());
}

/**
 * Decrypt encrypted string using key and IV
 *
 * @param ciphertext Encrypted text
 * @param ciphertext_len Encrypted text length
 * @param key Cipher key
 * @param iv Cipher IV
 * @return Decrypted string
 */
string AES256::decryptFinal(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv)
{

    EVP_CIPHER_CTX *ctx;
    unsigned char *plaintexts;
    int len;
    int plaintext_len;
    unsigned char *plaintext = new unsigned char[ciphertext_len];
    bzero(plaintext, ciphertext_len);

    if (!(ctx = EVP_CIPHER_CTX_new())) {
        EVP_CIPHER_CTX_cleanup(ctx);
        handleOpenSSLErrors();
    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_cleanup(ctx);
        handleOpenSSLErrors();
    }

    EVP_CIPHER_CTX_set_key_length(ctx, 32);

    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
        EVP_CIPHER_CTX_cleanup(ctx);
        handleOpenSSLErrors();
    }

    plaintext_len = len;
    int pad_len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        EVP_CIPHER_CTX_cleanup(ctx);
        handleOpenSSLErrors();
    }

    plaintext_len += len;
    plaintext[plaintext_len] = 0;

    EVP_CIPHER_CTX_cleanup(ctx);

    string ret = (char *) plaintext;
    delete[] plaintext;
    return ret;
}