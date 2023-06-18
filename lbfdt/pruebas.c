// #include "base.h"
// #include "gpio.h"
// #include "uart.h"

// #include "arm.h"

// esto irá luego en la misma carpeta
#include "libfdt/libfdt.h"
#include "libfdt/tests.h"
#include "libfdt/testdata.h"

#include <stdio.h>

// #include "libfdt/libfdt.a"

// #include <libfdt.h>





#define MMIO_BASE 0xFE000000

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

// #if RASPI <= 3
// #define AUX_UART_CLOCK      250000000
// #elif RASPI == 4
// #define AUX_UART_CLOCK      500000000
// #endif

#define AUX_MU_BAUD(baud)   ((AUX_UART_CLOCK/(baud*8))-1)

int static AUX_BASE, AUX_UART_CLOCK;

int static AUX_REGISTERS[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

void read_uart_registers_dt(void *fdt){

	printf("\nLECTURA DE REGISTROS UART PARA XV6\n");

	//DEFINICION DE PARAMETROS A UTILIZAR EN LA LECTURA DE REGISTROS

	int offset, lenp = 0;
	const char *filename;
	uint64_t address, size = 0;
	const struct fdt_property *propiedad;


	//LECTURA DE TODOS LOS REGISTROS

	offset = fdt_path_offset(fdt, "aux");
	fdt_simple_addr_size(fdt, offset, 0, &address, &size);
	
    AUX_REGISTERS[AUX_ENABLES] = address;

	offset = fdt_path_offset(fdt, "uart1");
	fdt_simple_addr_size(fdt, offset, 0, &address, &size);

	for (int i = 0; i < 12; i++){

		fdt_simple_addr_size(fdt, offset, i, &address, &size);

        AUX_REGISTERS[i+1] = 	address;
	}

	//LECTURA DEL NODO ENCARGADO DEL RELOJ DEL PUERTO SERIE Y PROPIEDAD CON SU FRECUENCIA

	offset = fdt_path_offset(fdt, "clocks");
	propiedad = fdt_get_property(fdt, offset, "clock-frequency", 0);

    AUX_UART_CLOCK = strtol(propiedad->data, NULL, 10);

    printf("REGISTROS: AUX_UART_CLOCK:%d\n", AUX_UART_CLOCK);

    for (int i = 0; i < 12; i++){

		printf("\t%d: %x\n", i, AUX_REGISTERS[i]);
	}


	printf("\nFINALIZACION LECTURA DE XV6\n--------------------------------\n");
}

int static GPIO_BASE;

int static GPIO_REGISTERS[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


void read_gpios_registers_dt(void *fdt){
	
	printf("\nLECTURA DE REGISTROS GPIOS PARA XV6\n\n");

	//DEFINICION DE PARAMETROS A UTILIZAR EN LA LECTURA DE REGISTROS

	int offset, lenp = 0;
	const char *filename;
	uint64_t address, size = 0;
	const struct fdt_property *propiedad;

	//LECTURA DEL NODO REFERENTE A LAS GPIOS

	offset = fdt_path_offset(fdt, "gpio");

	//LECTURA DE REGISTROS DE LA GPIO
	for (int i = 0; i < 18; i++){
		fdt_simple_addr_size(fdt, offset, i, &address, &size);
		GPIO_REGISTERS[i] = address;
	}


    printf("REGISTROS GPIO:\n\n");

    for (int i = 0; i < 18; i++){

		printf("\t%d: %x\n", i, GPIO_REGISTERS[i]);
	}


	printf("\nFINALIZACION LECTURA REGISTROS GPIOS\n-------------------------------------\n");


}

int main(int argc, char *argv[]){

    // test_init(argc, argv); 

	void *fdt = load_blob("rpi4-xv6.dtb");

	read_uart_registers_dt(fdt);

	read_gpios_registers_dt(fdt);

    return 0;
}
