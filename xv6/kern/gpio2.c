#include "base.h"
#include "gpio.h"
#include "gpio2.h"

#include "arm.h"

int turn_on_gpio(int gpio, int mode){
    //Compruebo a que selector corresponde esta gpio
    if (gpio > MAX_GPIO || gpio < MIN_GPIO){
        return 1; // error
    }

    unsigned int registro = 0;
    int gpio_selector = gpio / REGISTER_PER_SELECTOR; 
    int gpio_number = gpio % REGISTER_PER_SELECTOR;

    
    if (gpio_selector == 0){
        registro = get32(GPFSEL0);
        registro &= ~(7<<gpio_number); // limpio los bits de la gpio que quiero utilizar ahora
        registro |= mode << gpio_number; // asigno la gpio como de salida
        put32(GPFSEL0, registro);
    }
    else if (gpio_selector == 1){
        registro = get32(GPFSEL1);
        registro &= ~(7<<gpio_number); // limpio los bits de la gpio que quiero utilizar ahora
        registro |= mode << gpio_number*3; // asigno la gpio como de salida
        put32(GPFSEL1, registro);
    }
    else if (gpio_selector == 2){
        registro = get32(GPFSEL2);
        registro &= ~(7<<gpio_number); // limpio los bits de la gpio que quiero utilizar ahora
        registro |= mode << gpio_number; // asigno la gpio como de salida
        put32(GPFSEL2, registro);
    }
    else if (gpio_selector == 3){
        registro = get32(GPFSEL3);
        registro &= ~(7<<gpio_number); // limpio los bits de la gpio que quiero utilizar ahora
        registro |= mode << gpio_number; // asigno la gpio como de salida
        put32(GPFSEL3, registro);
    }
    else if (gpio_selector == 4){
        registro = get32(GPFSEL4);
        registro &= ~(7<<gpio_number); // limpio los bits de la gpio que quiero utilizar ahora
        registro |= mode << gpio_number; // asigno la gpio como de salida
        put32(GPFSEL4, registro);
    }
    else{
        registro = get32(GPFSEL5);
        registro &= ~(7<<gpio_number); // limpio los bits de la gpio que quiero utilizar ahora
        registro |= mode << gpio_number; // asigno la gpio como de salida
        put32(GPFSEL5, registro);
    }

    return 0;
}

int set_gpio(int gpio){
    
    if (gpio > MAX_GPIO || gpio < MIN_GPIO)
        return 1;

    int gpio_number = gpio % REGISTER_PER_GPSET_GPCLEAR;

    if (gpio < 32){
        put32(GPSET0, 1 << gpio_number);
    }
    else {
        put32(GPSET1, 1 << gpio_number);
    }

    return 0;
}


int clear_gpio(int gpio){

    if (gpio > MAX_GPIO || gpio < MIN_GPIO)
        return 1;

    int gpio_number = gpio % REGISTER_PER_GPSET_GPCLEAR;

    if (gpio < 32){
        put32(GPCLR0, 1 << gpio_number);
    }
    // else {
    //     put32(GPCLR1, 1 << gpio_number);
    // }

    return 0;

}



/**
 * Funciones para devilver el valor de cada registro de las gpios desde del devicetree
*/