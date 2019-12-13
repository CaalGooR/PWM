#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"
#include "Utility.h"
#include <avr/pgmspace.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

uint8_t TICKS(uint16_t frec) {	return frec ? ((16E6/1024)/frec) : 0 ; }

volatile static uint8_t isHolding,isRecording,isPlayingSong = 0,PlayingNote,PlayingNote1;
volatile static uint16_t noteTime,silenceTime,idx,last_freq = 0;
volatile static uint16_t noteTime1,silenceTime1,idx1;
volatile static Note *song;
volatile static Note *song1;
volatile static Note nota;
int16_t x = 256;
int8_t persent=50;

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
	// LOGICA TECLADO
	if (!isPlayingSong)
	{
		if ((!isHolding && nota.freq) || nota.freq != last_freq)
		{
			if (isRecording && nota.freq != last_freq)
			{
				song[idx].freq = last_freq;
				song[idx++].delay = noteTime;
				noteTime = 0;
			}
			Timer2_Freq_Gen(TICKS(nota.freq));
			last_freq = nota.freq;
		}
		if (isRecording)
		{
			if (PlayingNote) noteTime++;
			
			if (nota.freq == 0)
			{
				PlayingNote = FALSE;
				silenceTime++;
			}
			if (nota.freq != 0 && PlayingNote)
			{
				/*
				song[idx].freq = last_freq;
				song[idx++].delay = silenceTime;
			 	silenceTime = 0;*/
			}
		}
	}

	if (isPlayingSong)
	{
		// TIMER 0
			if (!noteTime && !idx && PlayingNote)
				Timer2_Freq_Gen(TICKS(song[idx].freq));
			
			if (noteTime == song[idx].delay && PlayingNote)
			{
				noteTime = 0;
				PlayingNote = FALSE;
				Timer2_Freq_Gen(0);
			}

			if (!PlayingNote)
			{
				silenceTime++;
				if (silenceTime == SILENCE)
				{
					Timer2_Freq_Gen(TICKS(song[++idx].freq));
					silenceTime = 0;
					PlayingNote = TRUE;
				}
			}
			noteTime++;

			if (song[idx].freq == fin)
			{
				idx = 0;
				noteTime = 0;
			}
		/* TIMER 1
			if (!noteTime1 && !idx1 && PlayingNote1)
				Timer1_Freq_Gen(TICKS(pgm_read_word(&song1[idx1].freq)));
			
			if (noteTime1 == pgm_read_word(&song1[idx1].delay) && PlayingNote1)
			{
				noteTime1 = 0;
				PlayingNote1 = 0;
				Timer1_Freq_Gen(0);
			}

			if (!PlayingNote1)
			{
				silenceTime1++;
				if (silenceTime1 == SILENCE)
				{
					Timer1_Freq_Gen(TICKS(pgm_read_word(&song1[++idx1].freq)));
					silenceTime1 = 0;
					PlayingNote1 = TRUE;
				}
			}
			noteTime1++;

			if (pgm_read_word(&song1[idx1].freq) == fin && isPlaying)
			{
				idx1 = 0;
				noteTime1 = 0;
			}*/	
	}
}

void SetHold() { isHolding = 1; }
void ClrHold() { isHolding = 0; }
uint8_t getHoldFlag () {return isHolding + '0';}

void Start_Record(Note *rec_song) 
{
	idx = 0;
	song =(Note *) rec_song;
	isRecording = TRUE; 
}

uint8_t Stop_Record() 
{
	isRecording = FALSE;
	song[idx].freq = fin;
	song[idx++].delay = SILENCE;
	return idx;
}

void Timer2_Freq_Gen(uint8_t ticks)
{
	if (isPlayingSong)
	{
		if (song[idx].freq == fin)
			return;

		if (song[idx].freq == 0 || persent == 0 )
		{
			StopTimer();
			return;
		}
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

void Timer2_Play(Note song_ptr[])
{
	song =(Note *) song_ptr;
    isPlayingSong = TRUE;
	UART0_putchar(isPlayingSong + '0');
	PlayingNote = TRUE;
	noteTime = 0;
	silenceTime = 0;
	idx = 0;
	TCNT0 = 0;
}

void Timer2_Volume(int8_t direction)
{
	if (direction==-1 && persent >= 2)
	{
		persent -= log(x)/log(2);
		if (persent < 0) persent = 0;
	}
	else if(direction && persent < 100)
	{
		persent += log(x)/log(2);
		if (persent > 100) persent = 100;
	}

	gotoxy(0,6);
	UART0_puts("Volume =      ");
	gotoxy(8,6);
	UART0_puts(itoa(str,persent,10));
}


void Timer1_Freq_Gen(uint8_t ticks)
{
	if (pgm_read_word(&song[idx1].freq) == fin)
		return;

	if (pgm_read_word(&song[idx1].freq) == 0 || persent == 0 )
	{
		TCCR1A = 0; 
		TCCR1B = 0;
		return;
	}
	if (ticks > 0)
	{
		DDRB |= (1<<PB6);
		TCNT1 = 0;
		OCR1AL = ticks;
		OCR1BL = ((OCR1AL/2)*persent)/100;
		TCCR1A = (3<<WGM10) | (2<<COM1B0);
		TCCR1B = (3<<WGM12) | (5<<CS10);
	}
	else
	{
		TCCR1B &= ~(5<<CS10);
		PORTB &= ~(1<<PB6);
	}
}

void Timer1_Play(const Note song_ptr[])
{
	song1 =(Note *) song_ptr;
	noteTime1 = 0;
	idx1 = 0;
	PlayingNote1 = TRUE;
	TCNT1 = 0;
	OCR1AH = 0;
	OCR1BH = 0;
}

void Timer2_PlayFromKey (Note nt)
{
	nota = nt;
	PlayingNote = TRUE;
	isPlayingSong = FALSE;
	UART0_putchar(isRecording + '0');
	if (nt.freq == 0 || persent == 0) StopTimer();
}

void StopTimer()
{
	TCCR2A = 0; 
	TCCR2B = 0;
}

uint8_t getPlayingSongFlag(){return isPlayingSong;}