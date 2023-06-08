# xv6-tfg

Implementación de xv6 para arquitectura arm haciendo uso de device tree para la getión de las interfaces hardware de la raspberry.

El árbol de directorios es el siguiente:

.

├── dts-dtb

├── lbfdt

├── README.md

└── xv6

En el directorio dts-dtb se encuentran los ficheros de código de device tree implementados, entre ellos también se encuentra el device tree que qemu utiliza y el oficial de la raspberry pi 4 usado como punto de partida.

En el directorio lbfdt se encuentra la librería que interactua con los device tree binarios (dtb), en esta carpeta se prueba y se crea la librería para moverla al xv6.

En el directorio xv6 se encuentra el sistema operativo, donde he añadido un directorio que incluye la libreria libfdt.
