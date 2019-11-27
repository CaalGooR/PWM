#ifndef _UART_H
#define _UART_H
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iomxx0_1.h>

#define ENTER 13
#define BACKSPACE 8
#define SPACE 32
#define DEL 127
#define BUFFER_SIZE 256
#define DIR_BASE 0xC0
#define BLUE    "34"
#define MAGENTA "35" 
#define CYAN    "36"
#define WHITE   "37" 
#define BLACK   "30"
#define RED     "31"
#define GREEN   "32" 
#define YELLOW  "33" 

// Calcula el baurate considerando a U2X = 0
#define Baud(baud) ((16E6/baud/8)-1)
// Calcula el modulo binario
#define MOD(idx) ((idx)&(BUFFER_SIZE-1))
// Regresa 1 si el buffer esta vacio
#define IS_BUFFER_EMPTY(buffer) ((buffer.in_idx == buffer.out_idx))
// Regresa 1 si el buffer esta lleno
#define IS_BUFFER_FULL(buffer) ((buffer.out_idx == MOD(buffer.in_idx+1)))

typedef struct {
    char buffer[BUFFER_SIZE];
    volatile unsigned char in_idx;
    volatile unsigned char out_idx;
}ring_buffer_t;

/**
 *  Inicializa los el dispositivo de comunicación serial
 * 
    *  uint8_t com = UART 0 a 2
    *  uint16_t baudrate = velocidad de comunicación
    *  uint8_t size = Tamaño del dato puede ser de 5 a 8 
    *  uint8_t parity = 0:(No paridad), 1:(Paridad impart, 2:(Paridad par))
    *  uint8_t stop = puede ser 1 o 2 */
void UART_Ini (uint8_t com,uint32_t baudrate,uint8_t size, uint8_t parity, uint8_t stop);
/* Envía un dato por encuesta por comunicación serial */
void UART0_sendData (char data);
/* Recibe un dato por encuesta por comunicación serial */
uint8_t UART0_getData ( void );
// Transmite una cadena por comunicación serial
void UART0_puts_polling(const char *str);
// Recibe una cadena por comunicación serial
void UART0_gets_polling(char *str);
// Limpia la pantalla 
void clrscr( void );
// Posiciona el cursor en una coordenada (x,y)
void gotoxy(uint8_t x, uint8_t y);
// Cambia el color de las letras en la terminal
void setColor(const char *color);
// Regresa si hay un elemento en el buffer
uint8_t UART0_Available( void );
// Trasmite un dato por comunicacion serial
void UART0_putchar(const char data);
// Recibe un dato por comunicacion serial
uint8_t UART0_getch();
// Habilita las interrupciones de transmision
void UDR0_EMPTY_INTERRUPT_ENABLE( void );
// Deshabilita las interrupciones de transmision
void UDR0_EMPTY_INTERRUPT_DISABLE( void ); 
// Imprime una cadena por comunicación serial
void UART0_puts(const char *str);
// Recibe una cadena por comunicación serial
void UART0_gets(char *str);

#endif /* _UART_H */
