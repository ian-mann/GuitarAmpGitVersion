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
#include "kiss_fft.h"
#include "_kiss_fft_guts.h"
#include "kiss_fftr.h"
#include "kiss_fft_log.h"
#include "complex.h"


#define SIZE_OF_BUFFER 2048

int16_t volatile mask = 0xffff;
int16_t eqSignal[SIZE_OF_BUFFER];

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
float  low = 0, mid = 0, pres = 0, high = 0, distSignal = 0;

kiss_fftr_cfg fft = kiss_fftr_alloc(SIZE_OF_BUFFER,0,0,0);
kiss_fftr_cfg ifft= kiss_fftr_alloc(SIZE_OF_BUFFER,1,0,0);

// time domain signals
kiss_fft_scalar *buffer = new kiss_fft_scalar[SIZE_OF_BUFFER];
kiss_fft_scalar *signal = new kiss_fft_scalar[SIZE_OF_BUFFER];
kiss_fft_scalar *b_fir = new kiss_fft_scalar[SIZE_OF_BUFFER];

kiss_fft_cpx *fftSignal = new kiss_fft_cpx[SIZE_OF_BUFFER/2+1];
kiss_fft_cpx *cabFFT= new kiss_fft_cpx[SIZE_OF_BUFFER/2+1];
kiss_fft_cpx *finalSignal = new kiss_fft_cpx[SIZE_OF_BUFFER/2+1];


//{0.099868245,		0.34089431,		0.63809788,		0.86921668,		0.96605086,		0.91215849,		0.70056963,		0.37781093,		0.063172527,		-0.16064043,		-0.26962945,		-0.26212081,		-0.17035504,		-0.055681292,		0.027985157,		0.063532181,		0.058243815,		0.025976719,		-0.0065959101,		-0.027907075,		-0.039835636,		-0.039946143,		-0.019803109,		0.0085588107,		0.032564823,		0.054773752,		0.074585147,		0.069757886,		0.036622707,		-0.011783541,		-0.068122216,		-0.11819131,		-0.15328045,		-0.17030998,		-0.16660698,		-0.14788695,		-0.12348086,		-0.10680146,		-0.099278755,		-0.083950467,		-0.069271274,		-0.061660234,		-0.056716029,		-0.053267185,		-0.053699914,		-0.062564798,		-0.073281832,		-0.076733656,		-0.075677104,	-0.066098519,		-0.056319777,		-0.054670278,		-0.067374416,	-0.096073218,		-0.12863536,		-0.15619142,		-0.17218836,		-0.16904409,		-0.14348622,	-0.10059220,		-0.057519022,		-0.025751533,		-0.0013141037,		0.016694965};


int i = 0;
int gain = 60;


s16 = read_audio_sample(); // get current input sample

s16 = s16*2;

// implement distortion algorithm
if(dist){

	// waveshaping
//	distSignal = ((2*s16)/2)*(1-(s16^2)/2);
	 distSignal = s16;
	 clean = s16;

	// apply gain and shift by +0.5
	 distSignal = (distSignal*gain) + 16383;
	// asymmetrical clipping
	if(distSignal > 21844){
		distSignal = 21844;
		clean = clean - distSignal;
	}else
	if(distSignal < -29000){
		distSignal = -29000;
		clean = clean - distSignal;
	}else{
		distSignal = distSignal;
	}

	distSignal = distSignal + clean;
	// gain stage 2
	// apply gain and shift by +0.5
	 distSignal = (distSignal*gain) + 16383;
	// asymmetrical clipping
	if(distSignal > 21844){
		distSignal = 21844;
		clean = clean - distSignal;
	}else
	if(distSignal < -29000){
		distSignal = -29000;
		clean = clean - distSignal;
	}else{
		distSignal = distSignal;
	}

    distSignal = (distSignal+ clean)/10;
}

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
	eqSignal[writeIndex] = high;
}else{eqSignal[writeIndex] = distSignal;}


for(i=0;i<SIZE_OF_BUFFER;i++){
	buffer[i]=eqSignal[i];
	buffer[i]=0;
	}

//kiss_fftr(fft, buffer, fftSignal);
//kiss_fftr(fft, b_fir, cabFFT);


kiss_fftr(ifft, finalSignal, signal);

outputSample = signal[writeIndex];
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
