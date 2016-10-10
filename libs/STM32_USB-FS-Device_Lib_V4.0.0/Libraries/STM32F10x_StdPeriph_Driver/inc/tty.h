#ifndef TTY_H
#define TTY_H

#define TTY_NONE()          uart_sendStr("\033[m")
#define TTY_RED()           uart_sendStr("\033[41;30m")
#define TTY_LIGHT_RED()     uart_sendStr("\033[1;31m")
#define TTY_GREEN()         uart_sendStr("\033[0;32;32m")
#define TTY_LIGHT_GREEN()   uart_sendStr("\033[1;32m")
#define TTY_BLUE()          uart_sendStr("\033[46;30m")
#define TTY_LIGHT_BLUE()    uart_sendStr("\033[1;34m")
#define TTY_DARY_GRAY()     uart_sendStr("\033[1;30m")
#define TTY_CYAN()          uart_sendStr("\033[0;36m")
#define TTY_LIGHT_CYAN()    uart_sendStr("\033[1;36m")
#define TTY_PURPLE()        uart_sendStr("\033[0;35m")
#define TTY_LIGHT_PURPLE()  uart_sendStr("\033[1;35m")
#define TTY_BROWN()         uart_sendStr("\033[0;33m")
#define TTY_YELLOW()        uart_sendStr("\033[1;33m")
#define TTY_LIGHT_GRAY()    uart_sendStr("\033[0;37m")
#define TTY_WHITE()         uart_sendStr("\033[1;37m")

#define TTY_COLORRTN()    uart_sendStr("\33[7m")
#define TTY_CLEAR()       uart_sendStr("\33[2J")
#define TTY_UNDERLINE()   uart_sendStr("\33[4m")
#define TTY_FLASH()       uart_sendStr("\33[5m")
#define TTY_OPTOFF()      uart_sendStr("\33[0m")

#endif
