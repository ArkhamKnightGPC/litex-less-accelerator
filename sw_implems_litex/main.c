#include <uart.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "LESS.h"

void print_hex(const char *label, const uint8_t *data, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

void test_less_full(){
    // STAGE 1: KEYGEN
    static prikey_t SK;
    static pubkey_t PK;
    LESS_keygen(&SK, &PK);
    printf("--- SK ---\n");
    print_hex("SK Seed", SK.compressed_sk, PRIVATE_KEY_SEED_LENGTH_BYTES);
    printf("----------\n\n");
    printf("--- PK ---\n");
    print_hex("PK G_0 Seed", PK.G_0_seed, SEED_LENGTH_BYTES);
    for (int i = 0; i < NUM_KEYPAIRS - 1; i++) {
        char label[32];
        sprintf(label, "PK SF_G[%d]", i);
        print_hex(label, PK.SF_G[i], RREF_MAT_PACKEDBYTES);
    }
    // STAGE 2: SIGN
    static sign_t sig;
    const char* msg = "test message";
    const uint64_t mlen = strlen(msg);
    LESS_sign(&SK, msg, mlen, &sig);
    print_hex("digest", sig.digest, HASH_DIGEST_LENGTH);
    print_hex("salt", sig.salt, HASH_DIGEST_LENGTH);
    for(int i=0; i<W; i++)
        print_hex("cf_monom_actions", sig.cf_monom_actions[i], N8);
    print_hex("seed_storage", sig.seed_storage, SEED_TREE_MAX_PUBLISHED_BYTES);

    // STAGE 3: VERIFY
    int verif = LESS_verify(&PK, msg, mlen, &sig);
    printf("verif = %d\n", verif);
}

int main(void){
    printf("BEGIN!\n");
    test_less_full();
    printf("END!\n");
}
