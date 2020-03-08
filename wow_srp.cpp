#include "wow_srp.h"
#include "BigNum.h"

#include "string.h"

#include <string>
#include <algorithm>

#include <iostream>

#include <openssl/sha.h>
#include <openssl/evp.h>

Wow_SRP::Wow_SRP() {
    k.from_dec("3");
    a.from_hex("8266845392F83273477A763E6356B795737D49");
}

void Wow_SRP::step1(String username, String password,
                String _B, String _g, String _N, String _s) {

    BigNum B, g, N, s;
    B.from_hex(_B.ascii());
    g.from_hex(_g.ascii());
    N.from_hex(_N.ascii());
    s.from_hex(_s.ascii());

    unsigned char* result = new unsigned char[20];
    digest = EVP_MD_CTX_new();

    username = username.to_upper();
    password = password.to_upper();


    A = g.mod_exp(a, N);

    EVP_DigestInit(digest, EVP_sha1());
    EVP_DigestUpdate(digest, A.to_rev_bytearray(32), 32);
    EVP_DigestUpdate(digest, B.to_rev_bytearray(32), 32);

    EVP_DigestFinal(digest, result, NULL);
    std::reverse(result, result+20);

    BigNum u;
    u.from_bin(result, 20);

    EVP_DigestInit(digest, EVP_sha1());
    EVP_DigestUpdate(digest, username.ascii(), username.length());
    EVP_DigestUpdate(digest, ":", 1);
    EVP_DigestUpdate(digest, password.ascii(), password.length());

    EVP_DigestFinal(digest, result, NULL);
    std::reverse(result, result+20);

    BigNum p;
    p.from_bin(result, 20);

    EVP_DigestInit(digest, EVP_sha1());
    EVP_DigestUpdate(digest, s.to_rev_bytearray(32), 32);
    EVP_DigestUpdate(digest, p.to_bytearray(20), 20);
    
    EVP_DigestFinal(digest, result, NULL);
    std::reverse(result, result+20);

    BigNum x;
    x.from_bin(result, 20);

    S = (B - (g.mod_exp(x, N) * k)).mod_exp(a + (u * x), N);

    unsigned char* t = S.to_rev_bytearray(32);
    unsigned char* t1 = new unsigned char[16];
    unsigned char* t2 = new unsigned char[16];
    unsigned char* vK = new unsigned char[40];

    for(int i = 0; i < 16; ++i){
        t1[i] = t[i * 2];
        t2[i] = t[i * 2 + 1];
    }

    EVP_DigestInit(digest, EVP_sha1());
    EVP_DigestUpdate(digest, t1, 16);

    EVP_DigestFinal(digest, result, NULL);

    for(int i = 0; i < 20; ++i){
        vK[i * 2] = result[i];
    }

    EVP_DigestInit(digest, EVP_sha1());
    EVP_DigestUpdate(digest, t2, 16);

    EVP_DigestFinal(digest, result, NULL);

    for(int i = 0; i < 20; ++i){
        vK[i * 2 + 1] = result[i];
    }

    unsigned char* N_hash = new unsigned char[20];
    unsigned char* g_hash = new unsigned char[20];

    EVP_DigestInit(digest, EVP_sha1());
    EVP_DigestUpdate(digest, N.to_rev_bytearray(32), 32);
    EVP_DigestFinal(digest, N_hash, NULL);

    EVP_DigestInit(digest, EVP_sha1());
    EVP_DigestUpdate(digest, g.to_rev_bytearray(1), 1);
    EVP_DigestFinal(digest, g_hash, NULL);

    for(int i = 0; i < 20; ++i){
        N_hash[i] = N_hash[i] ^ g_hash[i];
    }

    std::reverse(N_hash, N_hash+20);
    BigNum t3;
    t3.from_bin(N_hash, 20);

    std::reverse(vK, vK+40);
    BigNum K;
    K.from_bin(vK, 40);

    EVP_DigestInit(digest, EVP_sha1());
    EVP_DigestUpdate(digest, username.c_str(), username.length());

    EVP_DigestFinal(digest, result, NULL);
 
    std::reverse(result, result+20);
    BigNum t4;
    t4.from_bin(result, 20);

    EVP_DigestInit(digest, EVP_sha1());
    EVP_DigestUpdate(digest, t3.to_rev_bytearray(20), 20);
    EVP_DigestUpdate(digest, t4.to_rev_bytearray(20), 20);
    EVP_DigestUpdate(digest, s.to_rev_bytearray(32), 32);
    EVP_DigestUpdate(digest, A.to_rev_bytearray(32), 32);
    EVP_DigestUpdate(digest, B.to_rev_bytearray(32), 32);
    EVP_DigestUpdate(digest, K.to_rev_bytearray(40), 40);

    EVP_DigestFinal(digest, result, NULL);

    BigNum M;
    M.from_bin(result, 20);
}

String Wow_SRP::get_S() const{
    return S.to_hex_string();
}

void Wow_SRP::_bind_methods() {
    ClassDB::bind_method(D_METHOD("step1", "username", "password", "B", "g", "N", "s"), &Wow_SRP::step1);
    ClassDB::bind_method(D_METHOD("get_S"), &Wow_SRP::get_S);
}