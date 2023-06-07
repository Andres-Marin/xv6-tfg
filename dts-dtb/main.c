#include<stdio.h>
#include<stdlib.h>

// #include "dtc/libfdt/libfdt.h"
// #include "dtc/tests/tests.h"
// #include "dtc/tests/testdata.h"
#include<libfdt.h>	

int main(int argc, char *argv[]){

	const char *filename;
	void *fdt;
	size_t len;
	int err;

	// comprueba argumentos recibidos, ya veremos si esto en el xv6 hay que usarlo
	// test_init(argc, argv);

	// // funciones para cargar el btb desde argumentos en este caso de manera distintas
	// fdt = load_blob_arg(argc, argv);
	// fdt = load_blob(argv[1]);

	return 0;
}
