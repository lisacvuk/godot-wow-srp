#ifndef WOW_SRP_H
#define WOW_SRP_H

#include "core/reference.h"
#include "core/method_bind_ext.gen.inc"
#include "string.h"
#include "core/variant.h"
#include "BigNum.h"

#include <openssl/evp.h>

class Wow_SRP : public Reference {
    GDCLASS(Wow_SRP, Reference);
    BigNum k, a, A, x, M, S, K;

    EVP_MD_CTX* digest;
protected:
    static void _bind_methods();

public:
    PoolByteArray generateHashLogonProof() const;
    void step1(String username, String password,
                String B, String g, String N, String s);

    PoolByteArray get_S() const;
    PoolByteArray get_M() const;
    PoolByteArray get_A() const;
    PoolByteArray get_K() const;

    Wow_SRP();
};

#endif
