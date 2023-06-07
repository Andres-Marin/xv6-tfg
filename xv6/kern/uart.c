#include "base.h"
#include "gpio.h"
#include "uart.h"

#include "arm.h"

#include<stdio.h>
#include<stdlib.h>


// esto irá luego en la misma carpeta
#include "../libfdt/libfdt.h"
#include "../libfdt/tests.h"
#include "../libfdt/testdata.h"

#define MMIO_BASE 0xFE000000

#define AUX_ENABLES         0
#define AUX_MU_IO_REG       1
#define AUX_MU_IER_REG      2
#define AUX_MU_IIR_REG      3
#define AUX_MU_LCR_REG      4
#define AUX_MU_MCR_REG      5
#define AUX_MU_LSR_REG      6
#define AUX_MU_MSR_REG      7
#define AUX_MU_SCRATCH      8
#define AUX_MU_CNTL_REG     9
#define AUX_MU_STAT_REG     10
#define AUX_MU_BAUD_REG     11

#define AUX_MU_BAUD(baud)   ((AUX_UART_CLOCK/(baud*8))-1)

int static AUX_BASE, AUX_UART_CLOCK;

int static AUX_REGISTERS[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

void read_uart_registers_dt(void *fdt);

void
uart_putchar(int c)
{
    while (!(get32(AUX_REGISTERS[AUX_MU_LSR_REG]) & 0x20)) ;
    put32(AUX_REGISTERS[AUX_MU_IO_REG], c & 0xFF);
    /* Fix Windows's '\r'. */
    if (c == '\n')
        uart_putchar('\r');
}

int
uart_getchar()
{
    int stat = get32(AUX_REGISTERS[AUX_MU_IIR_REG]);
    if ((stat & 1) || (stat & 6) != 4)
        return -1;
    return get32(AUX_REGISTERS[AUX_MU_IO_REG]) & 0xFF;
}



void
uart_init()
{
    // Lectura de los registros
	void *fdt = load_blob("rpi4-xv6V1.dtb"); //DIRECCION DEL DTB

    read_uart_registers_dt(fdt);


    put32(GPPUD, 0);
    delayus(5);
    put32(GPPUDCLK0, (1 << 14) | (1 << 15));
    delayus(5);
    put32(GPPUDCLK0, 0);
   
    /* Enable mini uart and enable access to its registers. */
    put32(AUX_REGISTERS[AUX_ENABLES], 1);
    /* Disable auto flow control and disable receiver and transmitter (for now). */
    put32(AUX_REGISTERS[AUX_MU_CNTL_REG], 0);
    /* Enable receive interrupts. */
    put32(AUX_REGISTERS[AUX_MU_IER_REG], 3 << 2 | 1);
    /* Enable 8 bit mode. */
    put32(AUX_REGISTERS[AUX_MU_LCR_REG], 3);
    /* Set RTS line to be always high. */
    put32(AUX_REGISTERS[AUX_MU_MCR_REG], 0);
    /* Set baud rate to 115200 */
    put32(AUX_REGISTERS[AUX_MU_BAUD_REG], AUX_MU_BAUD(115200));
    /* Clear receive and transmit FIFO. */
    put32(AUX_REGISTERS[AUX_MU_IIR_REG], 6);
    /* Finally, enable transmitter and receiver. */
    put32(AUX_REGISTERS[AUX_MU_CNTL_REG], 3);
}


// int main(int argc, char *argv[]){

//     uart_init();

//     return 0;
// }

void read_uart_registers_dt(void *fdt){

	//DEFINICION DE PARAMETROS A UTILIZAR EN LA LECTURA DE REGISTROS

	int offset, lenp = 0;
	const char *filename;
	uint64_t address, size = 0;
	const struct fdt_property *propiedad;

	//LECTURA DEL NODO REFERENTE AL PUERTO SERIE Y LECTURA DE SU REGISTRO

	offset = fdt_path_offset(fdt, "uart1");
	fdt_simple_addr_size(fdt, offset, 0, &address, &size);
    
    // ASIGNACION DEL VALOR AUX_BASE
    AUX_BASE = MMIO_BASE + address;

	
	//LECTURA DEL NODO ENCARGADO DE LOS REGISTROS Y LECTURA DE REGISTROS

	offset = fdt_next_node(fdt, offset, 0);

	//LECTURA DE TODOS LOS REGISTROS

	offset = fdt_path_offset(fdt, "aux_enables");
	fdt_simple_addr_size(fdt, offset, 0, &address, &size);
	
    AUX_REGISTERS[AUX_ENABLES] = AUX_BASE + address;

	for (int i = 1; i < 12; i++){

		offset = fdt_next_node(fdt, offset, 0);
		fdt_simple_addr_size(fdt, offset, 0, &address, &size);

        AUX_REGISTERS[i] = AUX_BASE + address;
	}

	//LECTURA DEL NODO ENCARGADO DEL RELOJ DEL PUERTO SERIE Y PROPIEDAD CON SU FRECUENCIA

	offset = fdt_path_offset(fdt, "uart_clock");
	propiedad = fdt_get_property(fdt, offset, "clock-frequency", 0);

    AUX_UART_CLOCK = strtol(propiedad->data, NULL, 10);

    // printf("REGISTROS: AUX_BASE 0x%x - AUX_UART_CLOCK:%d - AUX_ENABLES: 0x%x - AUX_MU_IO_REG: 0x%x - AUX_MU_LSR_REG: 0x%x\n", AUX_BASE, AUX_UART_CLOCK, AUX_ENABLES, AUX_MU_IO_REG, AUX_MU_LSR_REG);

    // for (int i = 0; i < 12; i++){

	// 	printf("\t%d: 0x%x\n", i, AUX_REGISTERS[i]);
	// }

}







// ///////////////////////////////////////////////////////////////////////////////////////


// #include "base.h"
// #include "gpio.h"
// #include "uart.h"

// #include "arm.h"

// #define AUX_BASE            (MMIO_BASE + 0x215000)

// #define AUX_ENABLES         (AUX_BASE + 0x04)
// #define AUX_MU_IO_REG       (AUX_BASE + 0x40)
// #define AUX_MU_IER_REG      (AUX_BASE + 0x44)
// #define AUX_MU_IIR_REG      (AUX_BASE + 0x48)
// #define AUX_MU_LCR_REG      (AUX_BASE + 0x4C)
// #define AUX_MU_MCR_REG      (AUX_BASE + 0x50)
// #define AUX_MU_LSR_REG      (AUX_BASE + 0x54)
// #define AUX_MU_MSR_REG      (AUX_BASE + 0x58)
// #define AUX_MU_SCRATCH      (AUX_BASE + 0x5C)
// #define AUX_MU_CNTL_REG     (AUX_BASE + 0x60)
// #define AUX_MU_STAT_REG     (AUX_BASE + 0x64)
// #define AUX_MU_BAUD_REG     (AUX_BASE + 0x68)

// // #if RASPI <= 3
// #define AUX_UART_CLOCK      250000000
// // #elif RASPI == 4
// // #define AUX_UART_CLOCK      500000000
// // #endif

// #define AUX_MU_BAUD(baud)   ((AUX_UART_CLOCK/(baud*8))-1)

// void
// uart_putchar(int c)
// {
//     while (!(get32(AUX_MU_LSR_REG) & 0x20)) ;
//     put32(AUX_MU_IO_REG, c & 0xFF);
//     /* Fix Windows's '\r'. */
//     if (c == '\n')
//         uart_putchar('\r');
// }

// int
// uart_getchar()
// {
//     int stat = get32(AUX_MU_IIR_REG);
//     if ((stat & 1) || (stat & 6) != 4)
//         return -1;
//     return get32(AUX_MU_IO_REG) & 0xFF;
// }

// void
// uart_init()
// {
//     // uint32_t sel;

//     // sel = get32(GPFSEL1);
//     // sel &= ~(7 << 12);          /* Clean GPIO14. */
//     // sel |= 2 << 12;             /* Set alt5 for GPIO14. */
//     // sel &= ~(7 << 15);          /* Clean GPIO15. */
//     // sel |= 2 << 15;             /* Set alt5 for GPIO15. */
//     // put32(GPFSEL1, sel);

//     put32(GPPUD, 0);
//     delayus(5);
//     put32(GPPUDCLK0, (1 << 14) | (1 << 15));
//     delayus(5);
//     put32(GPPUDCLK0, 0);

//     /* Enable mini uart and enable access to its registers. */
//     put32(AUX_ENABLES, 1);
//     /* Disable auto flow control and disable receiver and transmitter (for now). */
//     put32(AUX_MU_CNTL_REG, 0);
//     /* Enable receive interrupts. */
//     put32(AUX_MU_IER_REG, 3 << 2 | 1);
//     /* Enable 8 bit mode. */
//     put32(AUX_MU_LCR_REG, 3);
//     /* Set RTS line to be always high. */
//     put32(AUX_MU_MCR_REG, 0);
//     /* Set baud rate to 115200 */
//     put32(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));
//     /* Clear receive and transmit FIFO. */
//     put32(AUX_MU_IIR_REG, 6);
//     /* Finally, enable transmitter and receiver. */
//     put32(AUX_MU_CNTL_REG, 3);
// }
