#include "Utility.h"

unsigned int atoi (char* str)
{
    int r = 0;
    while(*str)
    {
        if (*str < '0' || *str > '9')
            break;
            r = r*10 + *str -'0';
        /*r = (r +(*str - 0x30)) * 10;*/
        str++;
    }
    return r;
}

char* itoa(char* str,uint16_t num, uint8_t base)
{
    int i = 0;
    if (num == 0) /* Caso Base donde si es cero no iterar */
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    while (num != 0)
    {
        int re = num % base;
        /* Operador ternario donde si el numero exede a 9 lo convierte en una letra gracias al offset que se le da */
        str[i++] = (re > 9)? (re-10) + 'A' : re + '0'; 
        num = num/base;
    }
    str[i] = '\0'; /* fin de cadena */
    
    return invertir(str,i); /* retornamos la cadena invertida */
}

char* invertir(char* str, uint8_t tam)
{
    int inicio = 0;
    int fin = tam - 1; /* para no considerar el fin de cadena */
    char aux=' ';

    while (inicio < fin)
    {   
        aux = str[inicio];
        str[inicio++] = str[fin];
        str[fin--] = aux;
    }
    return str;
}

