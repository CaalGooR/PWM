#include "UART.h"
#include "Timer.h"
#include "Utility.h"
#include <avr/pgmspace.h>

volatile static Note *song;
volatile static Note nota;
volatile static uint8_t idx = 0;
volatile static uint16_t timesNotHolding = 0;
volatile static char op,last_pressed = 0;
volatile static Note song_arr[256];

char str[20];

int main ()
{
    DDRH |= 1<<PH6;
    DDRB |= 1<<PB4;
    PGM_VOID_P *PlayList = getPlayList();
    song =(Note *) PlayList[0];
    Timer0_Ini();
    UART_Ini(0,115200,8,0,1);
    sei();
    clrscr();
    gotoxy(0,0);
    UART0_puts("P.- Play Song\n\rV.- Increase Volume \n\rv.- Decrease Volume \n\rN.- Next  Songn\n\rB.- Previous Song\n\r");
    song_arr[0].freq = fin;
    while(1)
    {
        if (UART0_Available())
        {
            op = UART0_getch();
			switch (op)
            {
				case 'q': nota.freq = C4;  break;
                case '2': nota.freq = Db4; break;
				case 'w': nota.freq = D4;  break;   
				case '3': nota.freq = Eb4; break;
				case 'e': nota.freq = E4;  break;
                case 'r': nota.freq = F4;  break;
                case '5': nota.freq = Gb4; break;
                case 't': nota.freq = G4;  break;
                case '6': nota.freq = Ab4; break;
                case 'y': nota.freq = A4;  break;
                case '7': nota.freq = Bb4; break;
                case 'u': nota.freq = B4;  break;
                case 'i': nota.freq = C5;  break;
                case '9': nota.freq = Db5; break;
                case 'o': nota.freq = D5;  break;
                case '0': nota.freq = Eb5; break;
                case 'p': nota.freq = E5;  break;
                case '[': nota.freq = F5;  break;
                case 'V': Timer2_Volume(1);         break;
                case 'v': Timer2_Volume(-1);        break;

                case 'm':
                    Start_Record(song_arr);
                    gotoxy(20,20);
                    UART0_puts("                           ");
                    gotoxy(20,20);
                    setColor(RED);
                    UART0_puts("RECORDING...");
                    setColor(WHITE);
                break;
                case 'n':
                    gotoxy(20,20);
                    setColor(RED);
                    UART0_puts(itoa(str,Stop_Record(),10));
                    UART0_puts(" NOT RECORDING...");
                    setColor(WHITE);
                break;
                case 'P':
                    Timer2_Play(song_arr);    
                    gotoxy(20,20);
                    UART0_puts("                           ");
                    gotoxy(20,20);
                    setColor(RED);
                    UART0_puts("PLAYING...");
                    setColor(WHITE);
                break;

                default:  nota.freq = 0;   break;
                
            }
            if (last_pressed == op) SetHold();
            if (last_pressed != op) last_pressed = op;

            timesNotHolding = 0;
		}
        else timesNotHolding++;
        if (timesNotHolding > 800)
        {
            ClrHold();
            nota.freq = 0;
        }
        gotoxy(0,10);
        UART0_putchar(getHoldFlag());
        if (!getPlayingSongFlag()) 
            Timer2_PlayFromKey(nota);
    }
    return 0;
}
