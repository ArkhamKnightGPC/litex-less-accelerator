#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <valgrind/callgrind.h>
#include "LESS.h"

void profile_keygen(){
    prikey_t* SK = (prikey_t*)malloc(sizeof(prikey_t));
    pubkey_t* PK = (pubkey_t*)malloc(sizeof(pubkey_t));

    CALLGRIND_START_INSTRUMENTATION;
    CALLGRIND_TOGGLE_COLLECT;
    LESS_keygen(SK, PK);
    CALLGRIND_TOGGLE_COLLECT;
    CALLGRIND_STOP_INSTRUMENTATION;

    free(SK);
    free(PK);
}

int main(){
    profile_keygen();
}