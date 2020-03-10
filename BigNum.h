#pragma once

#include "core/variant.h"

#include <openssl/bn.h>

class BigNum{
public:
    BigNum();
    BigNum(const char* data);

    BigNum from_hex(const char* data);
    BigNum from_dec(const char* data);
    BigNum from_bin(const unsigned char* data, int length);

    unsigned char* to_rev_bytearray(int length);
    unsigned char* to_bytearray(int length);

    PoolByteArray to_poolbytearray(int length) const;

    char* to_hex_string() const;

    BigNum operator+(const BigNum& b);
    BigNum operator-(const BigNum& b);
    BigNum operator*(const BigNum& b);
    BigNum operator/(const BigNum& b);

    BigNum operator=(const BigNum& b);

    BigNum exp(const BigNum& b);
    BigNum mod_exp(const BigNum& b, const BigNum& d);

    BIGNUM* bn;
private:
};
