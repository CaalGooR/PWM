#include "UART.h"
#include "Timer.h"
#include <avr/pgmspace.h>

volatile static Note *song;
volatile static Note nota;
volatile static uint8_t idx = 0;

int main ()
{
    DDRH |= 1<<PH6;
    DDRB |= 1<<PB4;
    PGM_VOID_P *PlayList = getPlayList();
    song =(Note *) PlayList[0];
    Timer0_Ini();
    UART_Ini(0,19200,8,0,1);
    sei();
    clrscr();
    gotoxy(0,0);
    UART0_puts("P.- Play Song\n\rV.- Increase Volume \n\rv.- Decrease Volume \n\rN.- Next  Songn\n\rB.- Previous Song\n\r");
    
    while(1)
    {
        if (UART0_Available())
        {
            //char last_pressed = 0;
			switch (UART0_getch())
            {
				case 'q': nota.freq = C4;  break;
                case '2': nota.freq = Db4; break;
				case 'w': nota.freq = D4;  break;   
				case '3': nota.freq = Eb4; break;
				case 'e': nota.freq = E4;  break;
                case '4': nota.freq = F4;  break;
                case 'r': nota.freq = Gb4; break;
                case '5': nota.freq = G4;  break;
                case 't': nota.freq = Ab4; break;
                case '6': nota.freq = A4;  break;
                case 'y': nota.freq = Bb4; break;
                case '7': nota.freq = B4;  break;
                case 'u': nota.freq = C5;  break;
                case '8': nota.freq = Db5; break;
                case 'i': nota.freq = D5;  break;
                case '9': nota.freq = Eb5; break;
                case 'o': nota.freq = E5;  break;
                default:  nota.freq = 0;   break;
                
            }
            Timer2_PlayFromKey(nota);
		}//else nota.freq = 0;
        
        Timer2_PlayFromKey(nota);
    }
    return 0;
}
