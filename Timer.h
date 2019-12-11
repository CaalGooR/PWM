#ifndef _TIMER_H
#define _TIMER_H
#include "song.h"

void Timer0_Ini ( void );
void Timer2_Freq_Gen(uint8_t ticks);
void Timer2_Play(const Note song_ptr[]);
void Timer2_Volume(int8_t direction);
void Timer2_PlayFromKey (Note nt);
uint8_t getHoldFlag ();
void SetHold();
void ClHold();
void Timer1_Freq_Gen(uint8_t ticks);
void Timer1_Play(const Note song_ptr[]);
void Timer1_Volume(int8_t direction);
void Start_Record(Note * note);
void StopTimer();
uint8_t Stop_Record();

#endif
