#ifndef INC_UART_H
#define INC_UART_H

// #define AUX_ENABLES         (0)
// #define AUX_MU_IO_REG       (1)
// #define AUX_MU_IER_REG      (2)
// #define AUX_MU_IIR_REG      (3)
// #define AUX_MU_LCR_REG      (4)
// #define AUX_MU_MCR_REG      (5)
// #define AUX_MU_LSR_REG      (6)
// #define AUX_MU_MSR_REG      (7)
// #define AUX_MU_SCRATCH      (8)
// #define AUX_MU_CNTL_REG     (9)
// #define AUX_MU_STAT_REG     (10)
// #define AUX_MU_BAUD_REG     (11)

void uart_init();
void uart_intr();
void uart_putchar(int);
int  uart_getchar();

#endif
