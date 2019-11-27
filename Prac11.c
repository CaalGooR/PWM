#include "UART0.h"
#include "Timer.h"

const struct note ImperialMarch[]={
	//for the sheet music see:
	//http://www.musicnotes.com/sheetmusic/mtd.asp?ppn=MN0016254
	//this is just a translation of said sheet music to frequencies / time in ms
	//used TEMPO ms for a quart note
	{a, TEMPO},
	{a, TEMPO},
	{a, TEMPO},
	{f, TEMPO*3/4},
	{cH, TEMPO*1/4},

	{a, TEMPO},
	{f, TEMPO*3/4},
	{cH, TEMPO*1/4},
	{a, TEMPO*2},
	//first bit

	{eH, TEMPO},
	{eH, TEMPO},
	{eH, TEMPO},
	{fH, TEMPO*3/4},
	{cH, TEMPO*1/4},

	{gS, TEMPO},
	{f, TEMPO*3/4},
	{cH, TEMPO*1/4},
	{a, TEMPO*2},
	//second bit...

	{aH, TEMPO},
	{a, TEMPO*3/4},
	{a, TEMPO*1/4},
	{aH, TEMPO},
	{gSH, TEMPO/2},
	{gH, TEMPO/2},

	{fSH, TEMPO*1/4},
	{fH, TEMPO*1/4},
	{fSH, TEMPO/2},
	{0,TEMPO/2},
	{aS, TEMPO/2},
	{dSH, TEMPO},
	{dH, TEMPO/2},
	{cSH, TEMPO/2},
	//start of the interesting bit

	{cH, TEMPO*1/4},
	{b, TEMPO*1/4},
	{cH, TEMPO/2},
	{0,TEMPO/2},
	{f, TEMPO*1/4},
	{gS, TEMPO},
	{f, TEMPO*3/4},
	{a, TEMPO*1/4},

	{cH, TEMPO},
	{a, TEMPO*3/4},
	{cH, TEMPO*1/4},
	{eH, TEMPO*2},
	//more interesting stuff (this doesn't quite get it right somehow)

	{aH, TEMPO},
	{a, TEMPO*3/4},
	{a, TEMPO*1/4},
	{aH, TEMPO},
	{gSH, TEMPO/2},
	{gH, TEMPO/2},

	{fSH, TEMPO*1/4},
	{fH, TEMPO*1/4},
	{fSH, TEMPO/2},
	{0,TEMPO/2},
	{aS, TEMPO/2},
	{dSH, TEMPO},
	{dH, TEMPO/2},
	{cSH, TEMPO/2},
	//repeat... repeat

	{cH, TEMPO*1/4},
	{b, TEMPO*1/4},
	{cH, TEMPO/2},
	{0,TEMPO/2},
	{f, TEMPO/2},
	{gS, TEMPO},
	{f, TEMPO*3/4},
	{cH, TEMPO*1/4},

	{a, TEMPO},
	{f, TEMPO*3/4},
	{c, TEMPO*1/4},
	{a, TEMPO*2}
	//and we're done
};


int main(void)
{
	UART0_Ini();
	UART0_AutoBaudRate();
	
	Timer0_Ini();
    
	while(1){
		if (UART0_available()){
			switch (UART_getchar()){
				case 'p':
				case 'P': // Play Song
						Timer2_Play(ImperialMarch,sizeof(ImperialMarch)/sizeof(struct note));
						break;
				case 'V': //Increase Volume
						Timer2_Volume(1);
						break;
				case 'v'://Decrease Volume
						Timer2_Volume(-1);
						break;    
				case 'n':
				case 'N': //Next song
								break;
						case 'b'
				case 'B'://Previous song
								break;
						
				default:
				break;	
			}
		}
	}
	return 0;
}