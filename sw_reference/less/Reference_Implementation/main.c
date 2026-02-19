#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <valgrind/callgrind.h>
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
    prikey_t* SK = (prikey_t*)malloc(sizeof(prikey_t));
    pubkey_t* PK = (pubkey_t*)malloc(sizeof(pubkey_t));
    LESS_keygen(SK, PK);
    printf("--- SK ---\n");
    print_hex("SK Seed", SK->compressed_sk, PRIVATE_KEY_SEED_LENGTH_BYTES);
    printf("----------\n\n");
    printf("--- PK ---\n");
    print_hex("PK G_0 Seed", PK->G_0_seed, SEED_LENGTH_BYTES);
    for (int i = 0; i < NUM_KEYPAIRS - 1; i++) {
        char label[32];
        sprintf(label, "PK SF_G[%d]", i);
        print_hex(label, PK->SF_G[i], RREF_MAT_PACKEDBYTES);
    }
    // STAGE 2: SIGN
    sign_t* sig = (sign_t*)malloc(sizeof(sign_t));
    const char* msg = "test message";
    const uint64_t mlen = strlen(msg);
    LESS_sign(SK, msg, mlen, sig);
    print_hex("digest", sig->digest, HASH_DIGEST_LENGTH);
    print_hex("salt", sig->salt, HASH_DIGEST_LENGTH);
    for(int i=0; i<W; i++)
        print_hex("cf_monom_actions", sig->cf_monom_actions[i], N8);
    print_hex("seed_storage", sig->seed_storage, SEED_TREE_MAX_PUBLISHED_BYTES);

    // STAGE 3: VERIFY
    int verif = LESS_verify(PK, msg, mlen, sig);
    printf("verif = %d\n", verif);
}

int main(){
    test_less_full();
}