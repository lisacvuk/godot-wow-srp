#ifndef WOW_SRP_H
#define WOW_SRP_H

#include "core/reference.h"
#include "core/method_bind_ext.gen.inc"
#include "string.h"
#include "BigNum.h"

#include <openssl/evp.h>

class Wow_SRP : public Reference {
    GDCLASS(Wow_SRP, Reference);
    BigNum k, a, A, x, M, S, K;

    EVP_MD_CTX* digest;
protected:
    static void _bind_methods();

public:
    void step1(String username, String password,
                String B, String g, String N, String s);

    String get_S() const;

    Wow_SRP();
};

#endif
