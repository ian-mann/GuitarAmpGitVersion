//---------------------------------------------------------
// main example 3
//---------------------------------------------------------
#include "hellocfg.h" //BIOS include file
#include "framework.h"
#include "stdbool.h"
int16_t volatile mask = 0xffff;
bool filterOn = false;
//---------------------------------------------------------
//---------------------------------------------------------
void main(void)
{
initAll();
return; // return to BIOS scheduler
}
//---------------------------------------------------------
//---------------------------------------------------------
void dipPRD(void)
{
uint8_t dip_status8;
uint8_t dip_status1;
DIP_get(DIP_8, &dip_status8);
DIP_get(DIP_1, &dip_status1);

if(dip_status8) // switch 8 acts as a mute switch
	{
	mask = 0xffff;
	LED_turnOn(LED_2);
	}
	else
	{
		mask = 0x0000;
		LED_turnOff(LED_2);
	}
if(dip_status1) // switch 1 currently acts as a volume dip, will activate an FIR
{
// do something
	filterOn = true;
}
else
{
// dont do something
	filterOn = false;
}

}


int16_t audio(int16_t sample)
{
	if(filterOn){
		sample = sample/2;
	}
	else{
		sample = sample;
	}

	return sample;
}
//---------------------------------------------------------
//---------------------------------------------------------
void audioHWI(void)
{
int16_t s16;
s16 = read_audio_sample();

if (MCASP->RSLOT)
{
// THIS IS THE LEFT CHANNEL!!!
s16 = audio(s16);
s16 &= mask;
}
else {
// THIS IS THE RIGHT CHANNEL!!!
s16 = audio(s16);
s16 &= mask;
}
write_audio_sample(s16);
}
