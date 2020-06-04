//---------------------------------------------------------
// main example 3
//---------------------------------------------------------
#include "hellocfg.h" //BIOS include file
#include "framework.h"
#include "stdbool.h"
int16_t volatile mask = 0xffff;
bool filterOn = false;
float filterCoef[65] = {
	   -0.0000,	    0.0000,	    0.0000,
	   -0.0000,	   -0.0000,	    0.0000,	    0.0000,	   -0.0000,	   -0.0000,	   -0.0000,
	    0.0000,	    0.0001,	   -0.0000,	   -0.0004,	   -0.0003,	    0.0006,	    0.0014,
	   -0.0000,	   -0.0030,	   -0.0026,	    0.0037,	    0.0082,	   -0.0000,	   -0.0151,
	   -0.0124,	    0.0165,	    0.0354,	   -0.0001,	   -0.0647,	   -0.0570,	    0.0900,
	    0.2998,	    0.3999,		0.2998,	    0.0900,	   -0.0570,	   -0.0647,	   -0.0001,
	    0.0354,	    0.0165,	   -0.0124,	   -0.0151,    -0.0000,     0.0082,		0.0037,
	   -0.0026,	   -0.0030,	   -0.0000,	    0.0014,	    0.0006,	   -0.0003,	   -0.0004,
	   -0.0000,	    0.0001,	    0.0000,	   -0.0000,	   -0.0000,    -0.0000,		0.0000,
	    0.0000,	   -0.0000,    -0.0000,	    0.0000,	    0.0000,    -0.0000
};
float unfilteredCoef[65] = {1,1,1,1,1,1,1,1,1,1,
						1,1,1,1,1,1,1,1,1,1,
						1,1,1,1,1,1,1,1,1,1,
						1,1,1,1,1,1,1,1,1,1,
						1,1,1,1,1,1,1,1,1,1,
						1,1,1,1,1,1,1,1,1,1,
						1,1,1,1,1};
int16_t buffer[64] = {0,0,0,0,0,0,0,0,0,0,0,0,
					  0,0,0,0,0,0,0,0,0,0,0,0,
					  0,0,0,0,0,0,0,0,0,0,0,0,
					  0,0,0,0,0,0,0,0,0,0,0,0,
					  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};



//---------------------------------------------------------
//---------------------------------------------------------
void main(void)
{
	int i = 0;
	for(i=0;i<64;i++){
		buffer[i]=0;
					 }

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


//---------------------------------------------------------
//---------------------------------------------------------
void audioHWI(void)
{
int16_t s16 = 0;
int16_t outputSample = 0; // initialise sample variable

int i = 0;
int j = 0;
s16 = read_audio_sample(); // get current input sample
buffer[0] = s16; // put input sample at 0 spot of buffer

	for(i=1;i<64;i++){ // move every item in buffer one to the right
			buffer[i] = buffer[i-1];
	}

outputSample = buffer[64];

	if (MCASP->RSLOT)
	{
		// THIS IS THE LEFT CHANNEL!!!
		outputSample &= mask;
	}
	else {
		// THIS IS THE RIGHT CHANNEL!!!
		outputSample &= mask;
	}

write_audio_sample(outputSample);
buffer[0] = 0; // 0 first sample for placing in buffer
}
