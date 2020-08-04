//---------------------------------------------------------
// main example 3
//---------------------------------------------------------
#include "hellocfg.h" //BIOS include file
#include "framework.h"
#include "stdbool.h"
#include "stdio.h"
#include "math.h"
#include "data.h"
#include "biquad.h"
#define SIZE_OF_BUFFER 64

int16_t volatile mask = 0xffff;
float eqSignal[SIZE_OF_BUFFER];
int32_t fftSignal[SIZE_OF_BUFFER];
float buffer[SIZE_OF_BUFFER];
float wLow[3] = {0,0,0};
float wMid[3] = {0,0,0};
float wPres[3] = {0,0,0};
float wHigh[3] = {0,0,0};

bool eqBypass = false;
bool lowCut = false;
bool midCut = false;
bool presCut = false;
bool highCut = false;
bool dist = true;
biquad_t low;
biquad_t mid;
biquad_t pres;
biquad_t high;

// setup global buffer
int writeIndex = 0;

//---------------------------------------------------------
//---------------------------------------------------------
void main(void)
{
	int i = 0;
	for(i=0;i<SIZE_OF_BUFFER;i++){
	eqSignal[i]=0;
	buffer[i] = 0;
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
	lowCut = true;
}else{
	lowCut = false;
}

if(dip_status2) // boosts volume of mid frequencies
{
	midCut = true;
}else{
	midCut = false;
}

if(dip_status3) // boosts volume of pres frequencies
{
	presCut = true;
}else{
	presCut = false;
}

if(dip_status4) // boosts volume of high frequencies
{
	highCut = true;
}else{
	highCut = false;
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
int32_t s16 = 0;
int32_t clean = 0;
int16_t outputSample = 0; // initialise sample variable
float signal = 0, low = 0, mid = 0, pres = 0, high = 0, distSignal = 0;


int i = 0;
int gain = 2;


s16 = read_audio_sample();// get current input sample


s16 = s16*gain;


// implement distortion algorithm
if(dist)
{
	if(s16 < -32223) {
	s16= -10690;
	}else	if(s16 > 29478){
	s16 = 22995;
	}else	if(s16 >= -32223 && s16 <= -16613){
	s16 = (537)*(s16*s16*s16) + (14221)*(s16^2) + (11233)*(s16) + (-17944);
	}else	if(s16 > -16613 && s16 <= -6802){
	s16 = (7753)*(s16*s16*s16) + (4343)*(s16^2) + (5823)*(s16) + (-23793);
	}else	if(s16 > -6802 && s16 <= -69){
	s16 = (3543)*(s16*s16*s16) + (34970)*(s16^2) + (5220)*(s16) + (-24597);
	}else	if(s16 > -69 && s16 <= 6567){
	s16 = (-4171)*(s16*s16*s16) + (31021)*(s16^2) + (5221)*(s16) + (-1089);
	}else	if(s16 > 6567 && s16 <= 16404){
	s16 = (-35573)*(s16*s16*s16) + (40530)*(s16^2) + (5540)*(s16) + (-25050);
	}else	if(s16 > 16404 && s16 <= 29478){
	s16 = (9968)*(s16*s16*s16) + (-3693)*(s16*s16) + (54224)*(s16) + (-10630);
	}
}

distSignal = s16;

writeIndex = (writeIndex + SIZE_OF_BUFFER - 1) % SIZE_OF_BUFFER;

// implement EQ Stage
if(eqBypass){
	if(lowCut){
		wLow[2] = distSignal - aLow[1]*wLow[1] - aLow[2]*wLow[0];
		low = bLow[0]*wLow[2] + bLow[1]*wLow[1] + bLow[2]*wLow[0];
	}else{low = distSignal;}
	if(midCut){
		wMid[2] = low - aMid[1]*wMid[1] - aMid[2]*wMid[0];
		mid = bMid[0]*wMid[2] + bMid[1]*wMid[1] + bMid[2]*wMid[0];
	}else{mid = low;}
	if(presCut){
		wPres[2] = mid - aPres[1]*wPres[1] - aPres[2]*wPres[0];
		pres = bPres[0]*wPres[2] + bPres[1]*wPres[1] + bPres[2]*wPres[0];
	}else{pres = mid;}
	if(highCut){
		wHigh[2] = pres - aHigh[1]*wHigh[1] - aHigh[2]*wHigh[0];
		high = bLow[0]*wHigh[2] + bHigh[1]*wHigh[1] + bHigh[2]*wHigh[0];
	}else{high = pres;}
	buffer[writeIndex] = high;
}else{buffer[writeIndex] = distSignal;}


//apply cab sim
//	for(i=0;i<SIZE_OF_BUFFER;i++){
//	signal += (buffer[writeIndex+i % SIZE_OF_BUFFER] * b_fir[SIZE_OF_BUFFER-i]);
//}
	signal = buffer[writeIndex];

//outputSample = round(buffer[writeIndex]/2);
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
