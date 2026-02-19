#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <valgrind/callgrind.h>
#include "LESS.h"

void profile_verify(){
    prikey_t* SK = (prikey_t*)malloc(sizeof(prikey_t));
    pubkey_t* PK = (pubkey_t*)malloc(sizeof(pubkey_t));
    LESS_keygen(SK, PK);

    sign_t* sig = (sign_t*)malloc(sizeof(sign_t));
    const char* msg = "test message";
    const uint64_t mlen = strlen(msg);
    LESS_sign(SK, msg, mlen, sig);

    CALLGRIND_START_INSTRUMENTATION;
    CALLGRIND_TOGGLE_COLLECT;
    int verif = LESS_verify(PK, msg, mlen, sig);
    CALLGRIND_TOGGLE_COLLECT;
    CALLGRIND_STOP_INSTRUMENTATION;

    free(SK);
    free(PK);
    free(sig);
}

int main(){
    profile_verify();
}