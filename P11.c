#include "UART.h"
#include "Timer.h"
#include <avr/pgmspace.h>

volatile static Note *song;
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
			switch (UART0_getch())
            {
				case 'p':
				case 'P': // Play Song
						Timer2_Play(PlayList[idx]);
                break;
				
                case 'V': // Increase Volume
						Timer2_Volume(1);
                break;
				
                case 'v':// Decrease Volume
						Timer2_Volume(-1);
                break;   

				case 'n':
				case 'N':
                if (idx < 2)
                    Timer2_Play(PlayList[++idx]);
                break;

				case 'b':
				case 'B':
                    if (idx > 0)
                        Timer2_Play(PlayList[--idx]);
                break;
            }
		}
    }
    return 0;
}
