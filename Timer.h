#ifndef _TIMER_H
#define _TIMER_H
#include "song.h"

void Timer0_Ini ( void );
void Timer2_Freq_Gen(uint8_t ticks);
void Timer2_Play(const Note song_ptr[]);
void Timer2_Volume(int8_t direction);

#endif
