//---------------------------------------------------------
// main example 3
//---------------------------------------------------------
#include "hellocfg.h" //BIOS include file
#include "framework.h"
#include "stdbool.h"
#define SIZE_OF_BUFFER 64;

int16_t volatile mask = 0xffff;
int16_t buffer[64];
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
float test[3] = {0.3333,0.3333,0.3334};

// setup global buffer
int bufferLength = 0;
int readIndex = 1;
int writeIndex = 0;

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
int16_t sum = 0;

//int i = 0;
//int j = 0;

int prevRead = 0;
int prevPrevRead = 0;
s16 = read_audio_sample(); // get current input sample

buffer[writeIndex] = s16;
writeIndex++;
if (writeIndex == 64)
{
    writeIndex = 0;
}

prevRead = readIndex;
prevPrevRead = prevRead-1;
if (prevRead == 0){
	prevPrevRead = 64;
}
readIndex++;
if (readIndex == 64)
{
    readIndex = 0;
}

// implement buffer and convolution

if(filterOn){
	sum = (buffer[readIndex]*test[2] + buffer[prevRead]*test[1] + buffer[prevPrevRead]*test[0]);
		   outputSample = sum;
}
else{

outputSample = buffer[readIndex];
}

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
