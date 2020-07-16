//---------------------------------------------------------
// main example 3
//---------------------------------------------------------
#include "hellocfg.h" //BIOS include file
#include "framework.h"
#include "stdbool.h"
#include "stdio.h"
#include "math.h"
#include "data.h"
#include "fftss.h"

#define SIZE_OF_BUFFER 2048

int16_t volatile mask = 0xffff;
int32_t buffer[SIZE_OF_BUFFER];
int32_t eqSignal[SIZE_OF_BUFFER];
int32_t fftSignal[SIZE_OF_BUFFER];
int16_t low[6];
int16_t mid[11];
int16_t pres[11];
int16_t high[6];

int16_t wLow[6];
int16_t wMid[11];
int16_t wPres[11];
int16_t wHigh[6];

int lowGain = 1, midGain = 1, presGain = 1, highGain = 1;

float filt[3] = {0.333, 0.333, 0.333};
double in, out;
bool eqBypass = false;
bool dist = true;

// setup global buffer
int writeIndex = 0;

//---------------------------------------------------------
//---------------------------------------------------------
void main(void)
 {
	int i = 0;
	for(i=0;i<SIZE_OF_BUFFER;i++){
	buffer[i]=0;
	eqSignal[i]=0;
}

	for(i=0;i<6;i++){
	low[i]=0;
	high[i]=0;
	wLow[i]=0;
	wHigh[i]=0;
	}
	for(i=0;i<11;i++){
	mid[i]=0;
	pres[i]=0;
	wMid[i]=0;
	wPres[i]=0;
	}

// end loop
initAll();
return; // return to BIOS scheduler
}
//---------------------------------------------------------
//---------------------------------------------------------
void dipPRD(void)
{
uint8_t dip_status1;
uint8_t dip_status2;
uint8_t dip_status3;
uint8_t dip_status4;
uint8_t dip_status6;
uint8_t dip_status8;

DIP_get(DIP_1, &dip_status1);
DIP_get(DIP_2, &dip_status2);
DIP_get(DIP_3, &dip_status3);
DIP_get(DIP_4, &dip_status4);
DIP_get(DIP_6, &dip_status6);
DIP_get(DIP_8, &dip_status8);


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
if(dip_status1) // boosts volume of low frequencies
{
	lowGain = 4;
}else{
	lowGain = 1;
}

if(dip_status2) // boosts volume of mid frequencies
{
	midGain = 4;
}else{
	midGain = 1;
}

if(dip_status3) // boosts volume of pres frequencies
{
	presGain = 4;
}else{
	presGain = 1;
}

if(dip_status4) // boosts volume of high frequencies
{
	highGain = 4;
}else{
	highGain = 1;
}

if(dip_status6) // bypass EQ
{
	eqBypass = true;
}
else
{
	eqBypass = false;
}

}


//---------------------------------------------------------
//---------------------------------------------------------
void audioHWI(void)
{
int16_t s16 = 0;
int16_t outputSample = 0; // initialise sample variable
int32_t signal = 0;

int i = 0;
int gain = 10;


s16 = read_audio_sample(); // get current input sample

writeIndex = (writeIndex + SIZE_OF_BUFFER - 1) % SIZE_OF_BUFFER;
buffer[writeIndex] = s16;

// implement distortion algorithm
if(dist){
	// shift audio file for asymmetry
	buffer[writeIndex] = (gain*buffer[writeIndex])+16383;
	// gain stage 1
	if(buffer[writeIndex] > 21844){
		buffer[writeIndex] = round(buffer[writeIndex]/(4*gain));
	//buffer[writeIndex] = buffer[writeIndex]/(4*gain);
	}
	if(buffer[writeIndex] < -21844){
		buffer[writeIndex] = round(buffer[writeIndex]/(4*gain));
	}
}

// implement EQ Stage
if(eqBypass){
	for(i=0;i<6;i++){
	wLow[writeIndex] += -(low[(writeIndex+i) % 6] * aLow[i]);
	wHigh[writeIndex] += -(high[(writeIndex+i) % 6] * aHigh[i]);
	}
	wLow[writeIndex] = buffer[writeIndex] + wLow[writeIndex];
	wHigh[writeIndex] = buffer[writeIndex] + wHigh[writeIndex];

	// different loops due to different filter sizes
	for(i=0;i<11;i++){
	wMid[writeIndex] += -(mid[(writeIndex+i) % 11] * aMid[i]);
	wPres[writeIndex] += -(pres[(writeIndex+i) % 11] * aPres[i]);
	}
	wMid[writeIndex] += buffer[writeIndex] + wMid[writeIndex];
	wPres[writeIndex] += buffer[writeIndex] + wPres[writeIndex];


	for(i=0;i<6;i++){
	low[writeIndex] += (wLow[(writeIndex+i) % 6] * bLow[i]);
	high[writeIndex] += (wHigh[(writeIndex+i) % 6] * bHigh[i]);
	}
	for(i=0;i<11;i++){
	mid[writeIndex] += (wMid[(writeIndex+i) % 11] * bMid[i]);
	pres[writeIndex] += (wPres[(writeIndex+i) % 11] * bPres[i]);
	}

	eqSignal[writeIndex] = low[writeIndex]*lowGain + mid[writeIndex]*midGain + pres[writeIndex]*presGain + high[writeIndex]*highGain;
}else{
	eqSignal[writeIndex] = buffer[writeIndex];
}

//convert to frequency domain for FIR
fftss_plan fftss_plan_dft_1d(SIZE_OF_BUFFER, in*, out*, i, FFTSS_ESTIMATE);

// apply cab sim
	for(i=0;i<SIZE_OF_BUFFER;i++){
	signal += (fftSignal[(writeIndex+i) % SIZE_OF_BUFFER] * b_fir[i]);
}

// revert to time domain

// signal = eqSignal[writeIndex];

outputSample = signal/2;

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
