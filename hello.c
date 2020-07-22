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

#define SIZE_OF_BUFFER 2048

int16_t volatile mask = 0xffff;
int16_t eqSignal[SIZE_OF_BUFFER];
int32_t fftSignal[SIZE_OF_BUFFER];
int16_t buffer[SIZE_OF_BUFFER];

bool eqBypass = false;
bool lowCut = false;
bool midCut = false;
bool presCut = false;
bool highCut = false;
bool dist = false;
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
	/* BQ_init() links the ring buffers and sets inital conditions to 0 */
	BQ_init(&low);
	BQ_init(&mid);
	BQ_init(&pres);
	BQ_init(&high);

	/* Set the 2nd order transfer function numerator coeffecients
	 *  analog to matlab's B = [0.0039 0.0078 0.0039]
	 */
	BQ_setNum(&low, 0.97712229,	-1.95424458,0.97712229);
	BQ_setNum(&mid, 0.06482542,	0,	0.06482542);
	BQ_setNum(&pres, 0.15472029, 0,	-0.15472029);
	BQ_setNum(&high,0.09762616,0.19525232,0.09762616);

	/* Set the 2nd order transfer function numerator coeffecients
	 *  analog to matlab's A = [1.0 -1.8153 0.8310]
	 *  only two values are needed, filter coeffecients must be normalized
	 *  with respect to A[0]
	 */
	BQ_setDen(&low, -1.95372128,	0.95476788);
	BQ_setDen(&mid, -1.86134293,	0.87034917);
	BQ_setDen(&pres, -1.63295501,	0.69055942);
	BQ_setDen(&high, -0.94276158,	0.33326621);

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
int16_t s16 = 0;
int16_t outputSample = 0; // initialise sample variable

int i = 0;
int gain = 10;


s16 = read_audio_sample(); // get current input sample



// implement distortion algorithm
if(dist){
	// shift audio file for asymmetry
	s16 = (gain*s16)+16383;
	// gain stage 1
	if(s16 > 21844){
		s16 = round(s16/(4*gain));
	//buffer[writeIndex] = buffer[writeIndex]/(4*gain);
	}
	if(s16 < -21844){
		s16 = round(s16/(4*gain));
	}
}

buffer[writeIndex] = s16;
writeIndex = (writeIndex + SIZE_OF_BUFFER - 1) % SIZE_OF_BUFFER;


// implement EQ Stage
if(eqBypass){
	if(lowCut){
		buffer[writeIndex] = round(BQ_process(&low, buffer[writeIndex]));
	}
	if(midCut){
		buffer[writeIndex] = round(BQ_process(&mid, buffer[writeIndex]));
	}
	if(presCut){
		buffer[writeIndex] = round(BQ_process(&pres, buffer[writeIndex]));
	}
	if(highCut){
		buffer[writeIndex] = round(BQ_process(&high, buffer[writeIndex]));
    }
}


//convert to frequency domain for FIR
//fftss_plan fftss_plan_dft_1d(SIZE_OF_BUFFER, in*, out*, i, FFTSS_ESTIMATE);

// apply cab sim
//	for(i=0;i<SIZE_OF_BUFFER;i++){
//	signal += (fftSignal[(writeIndex+i) % SIZE_OF_BUFFER] * b_fir[i]);
//}

// revert to time domain
outputSample = buffer[writeIndex];

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
