#include <generated/csr.h>
#include <generated/soc.h>
#include <uart.h>

int main(void) {
    uart_init();

    while (1) {
        uart_rxtx_write('A');
        for (volatile int i = 0; i < 100; i++);
    }
}