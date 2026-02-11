#include <uart.h>

int main(void)
{
    uart_init();
    uart_write('H');
    uart_write('e');
    uart_write('l');
    uart_write('l');
    uart_write('o');
    uart_write(' ');
    uart_write('w');
    uart_write('o');
    uart_write('r');
    uart_write('l');
    uart_write('d');
    uart_write('!');
    uart_write('\n');

    return 0;
}
