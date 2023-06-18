
#include "base.h"
#include "uart.h"
#include "devicetree.h"

#include "arm.h"

#include<stdio.h>
#include<stdlib.h>

#define AUX_ENABLE 0

// #include "../libfdt/libfdt.h"
// #include "../libfdt/tests.h"
// #include "../libfdt/testdata.h"
#include <libfdt.h>

uint64_t AUX_UART_CLOCK;

uint64_t AUX_REGISTERS[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

uint64_t GPIO_REGISTERS[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void devicetree_init(){
    void *fdt = load_blob("rpi4-xv6.dtb");

    read_uart_registers_dt(fdt);

    read_gpios_registers_dt(fdt);
}



uint64_t get_uart_register(uint64_t reg){
    return AUX_REGISTERS[reg];
}

uint64_t get_uart_clock(){
    return AUX_UART_CLOCK;
}

uint64_t get_gpio_register(uint64_t reg){
    return GPIO_REGISTERS[reg];
}

void read_uart_registers_dt(void *fdt){

	//DEFINICION DE PARAMETROS A UTILIZAR EN LA LECTURA DE REGISTROS

	int offset, lenp = 0;
	const char *filename;
	uint64_t address, size = 0;
	const struct fdt_property *propiedad;


	//LECTURA DE TODOS LOS REGISTROS

	offset = fdt_path_offset(fdt, "aux");
	fdt_simple_addr_size(fdt, offset, 0, &address, &size);
	
    AUX_REGISTERS[AUX_ENABLE] = address;

	offset = fdt_path_offset(fdt, "uart1");
	fdt_simple_addr_size(fdt, offset, 0, &address, &size);

	for (int i = 0; i < 12; i++){

		fdt_simple_addr_size(fdt, offset, i, &address, &size);

        AUX_REGISTERS[i+1] = address;
	}

	//LECTURA DEL NODO ENCARGADO DEL RELOJ DEL PUERTO SERIE Y PROPIEDAD CON SU FRECUENCIA

	offset = fdt_path_offset(fdt, "clocks");
	propiedad = fdt_get_property(fdt, offset, "clock-frequency", 0);

    AUX_UART_CLOCK = strtol(propiedad->data, NULL, 10);

    for (int i = 0; i < 12; i++){

		printf("\t%d: %x\n", i, AUX_REGISTERS[i]);
	}

}


void read_gpios_registers_dt(void *fdt){
	
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


    for (int i = 0; i < 18; i++){

		printf("\t%d: %x\n", i, GPIO_REGISTERS[i]);
	}

}