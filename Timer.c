#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"
#include "Utility.h"
#include <avr/pgmspace.h>

#define TRUE 1
#define FALSE 0

uint8_t TICKS(uint16_t frec) {	return frec ? ((16E6/1024)/frec) : 0 ; }

volatile static uint8_t isPlaying,PlayingNote,persent=50;
volatile static uint16_t noteTime,silenceTime,idx;
volatile static Note *song;
static char str[20];

void Timer0_Ini ( void )
{
    TCNT0 = 0;
    OCR0A = 250;
	TCCR0A = 2;
    TCCR0B = 3;
    TIMSK0 = 2;
}

ISR(TIMER0_COMPA_vect)
{ 
	if (isPlaying)
	{
		if (!noteTime && !idx && PlayingNote)
			Timer2_Freq_Gen(TICKS(pgm_read_word(&song[idx].freq)));
		
		if (noteTime == pgm_read_word(&song[idx].delay) && PlayingNote)
		{
			noteTime = 0;
			PlayingNote = 0;
			Timer2_Freq_Gen(0);
		}

		if (!PlayingNote)
		{
			silenceTime++;
			if (silenceTime == SILENCE)
			{
				Timer2_Freq_Gen(TICKS(pgm_read_word(&song[++idx].freq)));
				silenceTime = 0;
				PlayingNote = TRUE;
			}
		}
		noteTime++;

		if (pgm_read_word(&song[idx].freq) == fin && isPlaying)
		{
			idx = 0;
			noteTime = 0;
		}
	}
}

void Timer2_Freq_Gen(uint8_t ticks)
{
	if (pgm_read_word(&song[idx].freq) == fin)
		return;

	if (pgm_read_word(&song[idx].freq) == 0 || persent == 0 )
	{
		TCCR2A = 0; 
		TCCR2B = 0;
		return;
	}
	if (ticks > 0)
	{
		DDRH |= (1<<PH6);
		TCNT2 = 0;
		OCR2A = ticks;
		OCR2B = ((OCR2A/2)*persent)/100;
		TCCR2A = (3<<WGM20) | (2<<COM2B0);
		TCCR2B = (1<<WGM22) | (7<<CS20);
	}
	else
	{
		TCCR2B &= ~(7<<CS20);
		PORTH &= ~(1<<PH6);
	}
}

void Timer2_Play(const Note song_ptr[])
{
	song =(Note *) song_ptr;
    isPlaying = TRUE;
	noteTime = 0;
	idx = 0;
	PlayingNote = TRUE;
	TCNT0 = 0;
	UART0_puts(itoa(str,pgm_read_word(&song[0].freq),10));
}

void Timer2_Volume(int8_t direction)
{
	if (persent > 0 && persent < 100)
		persent += direction;
	OCR2B = ((OCR2A/2)*persent)/100;
	gotoxy(0,6);
	UART0_puts("Volume = ");
	UART0_puts(itoa(str,persent,10));
}
