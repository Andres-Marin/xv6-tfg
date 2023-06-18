# xv6-tfg

Implementación de xv6 para arquitectura arm haciendo uso de device tree para la gestión de las interfaces hardware de la raspberry.

El árbol de directorios es el siguiente:

.

├── dts-dtb

├── xv6

└── README.md

En el directorio dts-dtb se encuentran los ficheros de código de device tree implementados, entre ellos también se encuentra el device tree que qemu utiliza y el oficial de la raspberry pi 4 usado como punto de partida.

En el directorio xv6 se encuentra el sistema operativo, donde he añadido un directorio que incluye la libreria libfdt.
