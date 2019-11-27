#ifndef _SONG_H
#define _SONG_H
#include <inttypes.h>
#include <avr/pgmspace.h>
/* Notas */
#define E3 165
#define G3 196
#define Ab3 208
#define A3 220
#define Bb3 233
#define B3 247
#define C4 262
#define Db4 277
#define D4 	294
#define Eb4  311
#define E4 	329
#define F4 	349
#define Gb4 370
#define G4 	391
#define Ab4 415
#define A4 	440
#define Bb4 466
#define B4 	493
#define C5 	523
#define Db5 554
#define D5 	587
#define Eb5 622
#define E5 	659
#define F5 	698
#define Gb5 740
#define G5 	784
#define Ab5 830
#define A5 	880
#define Bb5 932
#define B5	988
#define C6	1047
#define D6	1175
#define Eb6 1244
#define E6	1318
#define fin 1

#define BPM 164
#define SILENCE 10
#define TEMPO 355
#define TEMPO_500 500
#define TEMPO_180BPS 333

typedef struct note
{
	uint16_t freq;
	uint16_t delay;
}Note;

PGM_VOID_P getPlayList();

#endif
