#ifndef INC_UART_H
#define INC_UART_H

void devicetree_init();

uint64_t get_uart_register(uint64_t reg);
uint64_t get_uart_clock();
uint64_t get_gpio_register(uint64_t reg);

void read_uart_registers_dt(void *fdt);
void read_gpios_registers_dt(void *fdt);


#endif
