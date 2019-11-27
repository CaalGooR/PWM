#ifndef _UTILITY_H
#define _UTILITY_H

#include <inttypes.h>
#include <stdio.h>
#include "UART.h"

unsigned int atoi (char* str);
char* itoa(char* str,uint16_t num, uint8_t base);
char* invertir(char* str, uint8_t tam);

#endif /* _TIMER_H */
