#ifndef INC_GPI2_H
#define INC_GPIO2_H

#define REGISTER_PER_SELECTOR 10
#define MAX_GPIO 57
#define MIN_GPIO 0
#define REGISTER_PER_GPSET_GPCLEAR 32

#define GPFSEL0         (0)
#define GPFSEL1         (1)
#define GPFSEL2         (2)
#define GPFSEL3         (3)
#define GPFSEL4         (4)
#define GPFSEL5         (5)
#define GPSET0          (6)
#define GPSET1          (7)
#define GPCLR0          (8)
#define GPLEV0          (9)
#define GPLEV1          (10)
#define GPEDS0          (11)
#define GPEDS1          (12)
#define GPHEN0          (13)
#define GPHEN1          (14)
#define GPPUD           (15)
#define GPPUDCLK0       (16)
#define GPPUDCLK1       (17)

int turn_on_gpio(int gpio, int mode);
int set_gpio(int gpio);
int clear_gpio(int gpio);

/**
 * Funciones para devilver el valor de cada registro de las gpios desde del devicetree
*/

#endif