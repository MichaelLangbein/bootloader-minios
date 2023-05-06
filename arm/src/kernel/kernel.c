#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>
#include <common/stdio.h>
#include <common/stdlib.h>


void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    // In ARM, first three arguments are passed through registers r0, r1, and r2.

    // r0 and r1 are parameters by convention, but not used here.
    (void) r0;
    (void) r1;
    
    (void) atags;

    uart_init();
    uart_puts("Hello, kernel World!\r\n");

    while (1) {
        uart_putc(uart_getc());
        uart_putc('\n');
    }
}
