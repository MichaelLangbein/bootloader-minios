#include <stddef.h>
#include <stdint.h>


/**
 * MMIO:
 * Memory mapped io.
 * Peripherals get a portion of memory at a certain offset from peripheral-base-address at 0x0x3F000000
 * 4-byte parts of that portion are called `registers`. 
 * Usually at least on register is a control-register.
 * Commonly another one is a write-register.
 */

static inline void mmio_write(uint32_t reg, uint32_t data) {
    *(volatile uint32_t*)reg = data;
}

static inline uint32_t mmio_read(uint32_t reg) {
    return *(volatile uint32_t*)reg;
}

// Loop <delay> times in a way that the compiler won't optimize away
static inline void delay(int32_t count) {
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
            : "=r"(count): [count]"0"(count) : "cc");
}

enum {
    // The GPIO registers base address.
    GPIO_BASE = 0x3F200000, // for raspi2 & 3, 0x20200000 for raspi1

    GPPUD = (GPIO_BASE + 0x94),
    GPPUDCLK0 = (GPIO_BASE + 0x98),

    // The base address for UART.
    UART0_BASE = 0x3F201000, // for raspi2 & 3, 0x20201000 for raspi1

    UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSRECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),
};

/**
 * Sets some common configuration-flags for various registers.
*/
void uart_init() {
    // disable uart
    mmio_write(UART0_CR, 0x00000000);

    // disabling gpio-pints 14 and 15.
    mmio_write(GPPUD, 0x00000000);
    delay(150);

    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);

    mmio_write(GPPUDCLK0, 0x00000000);

    // clear all pending interrupts
    mmio_write(UART0_ICR, 0x7FF);

    // sets a baud-rate
    mmio_write(UART0_IBRD, 1);
    mmio_write(UART0_FBRD, 40);

    // writes bits 4, 5, 6 to line-control-register.
    // 4: uart holds an 8 item deep fifo
    // 5, 6: data has 8-bit long words
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    // writes ones to interrupt-mask: disabling all interrupts from uart.
    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
            (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    // writes bits 0, 8, 9 to control-register
    // 0: enables uart-hardware
    // 8: enables receiving data
    // 9: enables transmitting data
    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

// FR: flag-register: tells if there's data to read
// DR: data-register: where data is written to and read from.
void uart_putc(unsigned char c) {
    while ( mmio_read(UART0_FR) & (1 << 5) ) { }
    mmio_write(UART0_DR, c);
}

unsigned char uart_getc() {
    while ( mmio_read(UART0_FR) & (1 << 4) ) { }
    return mmio_read(UART0_DR);
}

void uart_puts(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i ++)
        uart_putc((unsigned char)str[i]);
}


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
