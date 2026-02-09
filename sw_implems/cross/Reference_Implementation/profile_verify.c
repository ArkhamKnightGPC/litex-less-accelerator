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

void profile_verify(){
    sk_t* SK = (sk_t*)malloc(sizeof(sk_t));
    pk_t* PK = (pk_t*)malloc(sizeof(pk_t));
    CROSS_keygen(SK, PK);

    CROSS_sig_t* sig = (CROSS_sig_t*)malloc(sizeof(CROSS_sig_t));
    const char* msg = "test message";
    const uint64_t mlen = strlen(msg);
    CROSS_sign(SK, msg, mlen, sig);

    CALLGRIND_START_INSTRUMENTATION;
    CALLGRIND_TOGGLE_COLLECT;
    int verif = CROSS_verify(PK, msg, mlen, sig);
    CALLGRIND_TOGGLE_COLLECT;
    CALLGRIND_STOP_INSTRUMENTATION;

    free(SK);
    free(PK);
    free(sig);
}

int main(){
    profile_verify();
}