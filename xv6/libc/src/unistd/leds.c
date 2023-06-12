#include <unistd.h>
#include <errno.h>
#include "syscall.h"

int
turn_on_gpio(int gpio, int mode){

    __syscall_cp(SYS_turn_on_gpio, gpio, mode);

    return 0;
}

int
turn_off_gpio(int gpio){
    
     __syscall_cp(SYS_turn_off_gpio, gpio);

    return 0;

}