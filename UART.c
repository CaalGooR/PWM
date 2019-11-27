#include "UART.h"

static ring_buffer_t buffer_tx,buffer_rx;

void UART_Ini (uint8_t com,uint32_t baudrate,uint8_t size, uint8_t parity, uint8_t stop)
{
    static uint8_t *reg_ptr =(uint8_t *) DIR_BASE;
    
     /* Si el COM tiene un valor valido entonces se asigna la dirección correspondiente 
        de lo contrario entonces se le deja la dirección base de UART0 */
    if (com <= 2) 
        reg_ptr =(uint8_t *) ((DIR_BASE) + (8*com));
    
    // UCSRnA
    *reg_ptr =  1<<U2X0;
     reg_ptr++;

    // UCSRnB
    *reg_ptr = (1<<RXCIE0) | (3<<TXEN0);

    if (size > 8) 
        *reg_ptr = *reg_ptr | (1<<UCSZ02);

    reg_ptr++;
    // UCSRnC
    if (stop == 1) *reg_ptr = *reg_ptr & (~(1<<USBS0));
    if (stop == 2) *reg_ptr = *reg_ptr | (1<<USBS0);

    if (parity == 1) *reg_ptr = *reg_ptr | (3<<UPM00);      // ODD
    else if (parity == 2) *reg_ptr = *reg_ptr | (2<<UPM00); // EVEN
    else *reg_ptr = *reg_ptr & ~(3<<UPM00);                 // No parity
    
    *reg_ptr = *reg_ptr & ~(3<<UMSEL00);                    // Asynchronous
    reg_ptr += 2;

    // Calculo UBRR
    *reg_ptr = Baud(baudrate);
}

void UART0_sendData(const char data)
{
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

uint8_t UART0_getData( void )
{
    while (!(UCSR0A & 1<<RXC0));
    return UDR0;
}

void UART0_puts_polling(const char *str)
{
    while (*str)
    {
        UART0_sendData(*str);
        str++;
    }
}

void UART0_gets_polling(char *str)
{
    char c = ' ';
    char count = 0; /* Lleva conteo de los caracteres almacenados en la cadena */ 
    do
    {
        
        c = UART0_getData();
       
        if (c >= SPACE && c < DEL && count < 20)
        {
            UART0_sendData(c);
            count++;
            *str = c;
            str++; /* Aumenta la posicion a la que apunta */
        }

        if (c == BACKSPACE && count > 0) /* Borra y caracteres de la cadena */
        {
            str--;
            count--;
            *str = '\0';
            UART0_sendData(BACKSPACE);
            UART0_sendData(' ');
            UART0_sendData(BACKSPACE);
        }
        
    }while(c != ENTER);
    *str = '\0'; /* Agrega el fin de cadena */
    UART0_sendData('\n');
    UART0_sendData('\r');
}

void clrscr( void ){ UART0_puts("\033[2J"); }

void gotoxy(uint8_t x, uint8_t y)
{
	char aux[5]={0,0,0,0,0}; 
	UART0_puts("\033[");  // Secuencia
	itoa(aux,y,10); 	  // Y coord
	UART0_puts(aux);	  
	UART0_putchar(';');
	itoa(aux,x,10);		 //X coord
	UART0_puts(aux);	
	UART0_putchar('f');  //resto de la secuencia
}

void setColor(const char *color)
{
	UART0_puts("\033[0;");//secuencia
	UART0_puts(color);	  //Color recibido como par�metro
	UART0_puts(";40m");   //resto de la secuencia 
}

void UART0_puts(const char *str)
{
    while (*str)
    {
        UART0_putchar(*str);
        str++;
    }
}

void UART0_gets(char *str)
{
    char c = ' ';
    char count = 0; /* Lleva conteo de los caracteres almacenados en la cadena */ 
    do
    {
        
        c = UART0_getch();
       
        if (c >= SPACE && c < DEL && count < 20)
        {
            UART0_putchar(c);
            count++;
            *str = c;
            str++; /* Aumenta la posicion a la que apunta */
        }

        if (c == BACKSPACE && count > 0) /* Borra y caracteres de la cadena */
        {
            str--;
            count--;
            *str = '\0';
            UART0_putchar(BACKSPACE);
            UART0_putchar(' ');
            UART0_putchar(BACKSPACE);
        }
        
    }while(c != ENTER);
    *str = '\0'; /* Agrega el fin de cadena */
    UART0_putchar('\n');
    UART0_putchar('\r');
}

uint8_t UART0_getch()
{
    while (IS_BUFFER_EMPTY(buffer_rx));
    uint8_t data = buffer_rx.buffer[buffer_rx.out_idx];
    buffer_rx.out_idx = MOD(buffer_rx.out_idx+1);
    return data;
}

void UART0_putchar(const char data)
{
    while (IS_BUFFER_FULL(buffer_tx))
        ;
    buffer_tx.buffer[buffer_tx.in_idx] = data;
    buffer_tx.in_idx = MOD(buffer_tx.in_idx+1);
    UDR0_EMPTY_INTERRUPT_ENABLE();
}

ISR (USART0_RX_vect)
{
    if (!IS_BUFFER_FULL(buffer_rx))
    {
        buffer_rx.buffer[buffer_rx.in_idx] = UDR0;
        buffer_rx.in_idx = MOD(buffer_rx.in_idx+1);
    }
}

ISR (USART0_UDRE_vect)
{
    if (!IS_BUFFER_EMPTY(buffer_tx))
    {
        UDR0 = buffer_tx.buffer[buffer_tx.out_idx];
        buffer_tx.out_idx = MOD(buffer_tx.out_idx+1);
    }
    else UDR0_EMPTY_INTERRUPT_DISABLE();
}

void UDR0_EMPTY_INTERRUPT_ENABLE( void ) { UCSR0B |= 1<<UDRIE0; }
uint8_t UART0_Available( void ) {return !(IS_BUFFER_EMPTY(buffer_rx));}
void UDR0_EMPTY_INTERRUPT_DISABLE( void ) { UCSR0B &= ~(1<<UDRIE0); }
