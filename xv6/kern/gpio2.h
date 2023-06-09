#ifndef INC_GPI2_H
#define INC_GPIO2_H

#define REGISTER_PER_SELECTOR 10

int turn_on_gpio(int gpio, int mode);
int set_gpio(int gpio);
int clear_gpio(int gpio);

/**
 * Funciones para devilver el valor de cada registro de las gpios desde del devicetree
*/

#endif