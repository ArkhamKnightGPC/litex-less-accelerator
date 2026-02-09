#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <valgrind/callgrind.h>
#include "CROSS.h"
#include "pack_unpack.h"
#include "parameters.h"
#include "randombytes.h"

void print_hex(const char *label, const uint8_t *data, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

void test_CROSS_full(){
    // STAGE 1: KEYGEN
    sk_t* SK = (sk_t*)malloc(sizeof(sk_t));
    pk_t* PK = (pk_t*)malloc(sizeof(pk_t));
    CROSS_keygen(SK, PK);
    printf("--- SK ---\n");
    print_hex("seed_sk", SK->seed_sk, KEYPAIR_SEED_LENGTH_BYTES);
    printf("----------\n\n");
    printf("--- PK ---\n");
    print_hex("seed_pk", PK->seed_pk, KEYPAIR_SEED_LENGTH_BYTES);
    print_hex("s", PK->s, DENSELY_PACKED_FP_SYN_SIZE);
    printf("----------\n\n");

    // STAGE 2: SIGN
    CROSS_sig_t* sig = (CROSS_sig_t*)malloc(sizeof(CROSS_sig_t));
    const char* msg = "test message";
    const uint64_t mlen = strlen(msg);
    CROSS_sign(SK, msg, mlen, sig);
    printf("--- SIG ---\n");
    print_hex("salt", sig->salt, SALT_LENGTH_BYTES);
    print_hex("digest_cmt", sig->digest_cmt, HASH_DIGEST_LENGTH);
    print_hex("digest_chall_2", sig->digest_chall_2, HASH_DIGEST_LENGTH);
    print_hex("path", sig->path, TREE_NODES_TO_STORE*SEED_LENGTH_BYTES);
    print_hex("proof", sig->proof, HASH_DIGEST_LENGTH*TREE_NODES_TO_STORE);
    for(int i=0; i<T-W; i++)
        print_hex("resp_1", sig->resp_1[i], HASH_DIGEST_LENGTH);
    print_hex("resp_0", sig->resp_0->y, DENSELY_PACKED_FP_VEC_SIZE);
    printf("----------\n\n");

    // STAGE 3: VERIFY
    int verif = CROSS_verify(PK, msg, mlen, sig);
    printf("--- VERIF ---\n");
    printf("verif = %d\n", verif);
    printf("----------\n\n");

    // free allocated memory
    free(SK);
    free(PK);
    free(sig);
}

int main(){
    test_CROSS_full();
}