//---------------------------------------------------------
// main example 3
//---------------------------------------------------------
#include "hellocfg.h" //BIOS include file
#include "framework.h"
#include "stdbool.h"
#include "stdio.h"
#include "math.h"
#include "data.h"
#define SIZE_OF_BUFFER 50

int16_t volatile mask = 0xffff;
int16_t buffer[SIZE_OF_BUFFER];
bool filterOn = false;
bool dist = false;

float lowPass1[16] = {0.0047, -0.0245,0.0017,0.0475,
		   -0.0236,-0.1019,0.1137,
		    0.4761,0.4761, 0.1137,-0.1019,-0.0236,
		    0.0475,0.0017,-0.0245,0.0047};

float lowPass2[3] = {0.3333, 0.3333, 0.3333};

// setup global buffer
int writeIndex = 0;


//---------------------------------------------------------
//---------------------------------------------------------
void main(void)
 {
	int i = 0;
	for(i=0;i<SIZE_OF_BUFFER;i++){
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
uint8_t dip_status3;
DIP_get(DIP_8, &dip_status8);
DIP_get(DIP_1, &dip_status1);
DIP_get(DIP_3, &dip_status3);

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

if(dip_status3)
{
	dist = true;
}else{
	dist = false;
}

}


//---------------------------------------------------------
//---------------------------------------------------------
void audioHWI(void)
{
int16_t s16 = 0;
int16_t outputSample = 0; // initialise sample variable
int16_t signal = 0;

int i = 0;


s16 = read_audio_sample(); // get current input sample

writeIndex = (writeIndex + SIZE_OF_BUFFER - 1) % SIZE_OF_BUFFER;
buffer[writeIndex] = s16;

// implement distortion algorithm


// implement buffer and convolution

if(filterOn){
	for(i=0;i<SIZE_OF_BUFFER;i++){
// y = x[n]b - y[n-1]a
	signal += (buffer[(writeIndex + i) % SIZE_OF_BUFFER] * b_fir[SIZE_OF_BUFFER-i]);

	}
signal = signal/a_fir[0];

}

else{

signal = buffer[writeIndex];
}



outputSample = signal;

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
}
