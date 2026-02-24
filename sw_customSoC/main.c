#include <stdint.h>
#include <generated/csr.h>
#include <generated/soc.h>
#include <uart.h>

#include "codes.h"
#include "rng.h"

// Put large objects in BSS, not stack
static generator_mat_t G;
static uint8_t is_pivot_column[N];
static uint8_t was_pivot_column[N];

static void sample_full_matrix(generator_mat_t *G,
                               const unsigned char seed[SEED_LENGTH_BYTES]) {
    SHAKE_STATE_STRUCT state;
    initialize_csprng(&state, seed, SEED_LENGTH_BYTES);

    for (uint32_t i = 0; i < K; i++) {
        for (uint32_t j = 0; j < N; j++) {
            unsigned char byte;
            xof_shake_extract(&state, &byte, 1);
            G->values[i][j] = byte % Q;
        }
    }
}

static void print(const char *s) {
    while (*s)
        uart_rxtx_write(*s++);
}

static void print_uint32(uint32_t v) {
    char buf[11];
    int i = 10;
    buf[i--] = '\0';

    if (v == 0) {
        uart_rxtx_write('0');
        return;
    }

    while (v && i >= 0) {
        buf[i--] = '0' + (v % 10);
        v /= 10;
    }

    const char *p = &buf[i+1];
    while (*p)
        uart_rxtx_write(*p++);
}

static void print_matrix(const generator_mat_t *M) {
    for (uint32_t i = 0; i < K; i++) {
        for (uint32_t j = 0; j < N; j++) {
            print_uint32(M->values[i][j]);
            if (j != N - 1)
                uart_rxtx_write(' ');
        }
        uart_rxtx_write('\n');
    }
}

int main(void) {
    uart_init();

    print("RREF test start...\n");
    //Check toy parameters...
    print_uint32(N); print("\n");
    print_uint32(K); print("\n");
    print_uint32(N_pad); print("\n");

    // sample toy generator matrix
    unsigned char seed[SEED_LENGTH_BYTES] = {0};
    sample_full_matrix(&G, seed);

    // Clear pivot arrays
    for (int i = 0; i < N; i++) {
        is_pivot_column[i] = 0;
        was_pivot_column[i] = 0;
    }
    int limit = 2;

    print("Matrix BEFORE RREF:\n");
    print_matrix(&G);

    int result = generator_RREF_pivot_reuse(
        &G,
        is_pivot_column,
        was_pivot_column,
        limit
    );

    print("Matrix AFTER RREF:\n");
    print_matrix(&G);

    if (result)
        print("RREF SUCCESS\n");
    else
        print("RREF FAILURE\n");

    while (1);
}