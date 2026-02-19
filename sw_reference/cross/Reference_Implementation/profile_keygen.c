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

void profile_keygen(){
    sk_t* SK = (sk_t*)malloc(sizeof(sk_t));
    pk_t* PK = (pk_t*)malloc(sizeof(pk_t));

    CALLGRIND_START_INSTRUMENTATION;
    CALLGRIND_TOGGLE_COLLECT;
    CROSS_keygen(SK, PK);
    CALLGRIND_TOGGLE_COLLECT;
    CALLGRIND_STOP_INSTRUMENTATION;

    free(SK);
    free(PK);
}

int main(){
    profile_keygen();
}