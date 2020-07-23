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
float eqSignal[SIZE_OF_BUFFER];
int32_t fftSignal[SIZE_OF_BUFFER];
float buffer[SIZE_OF_BUFFER];

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
	buffer[i] = 0;
	}

	/* BQ_init() links the ring buffers and sets inital conditions to 0 */
	BQ_init(&low);
	BQ_init(&mid);
	BQ_init(&pres);
	BQ_init(&high);

	/* Set the 2nd order transfer function numerator coeffecients
	 *  analog to matlab's B = [0.0039 0.0078 0.0039]
	 */
	BQ_setNum(&low, 1.0066410392970078,	-1.967424639045396,	0.9611629126895641);
	BQ_setNum(&mid, 1.0213222535332775,	-1.892567957556463,	0.8753065318405447);
	BQ_setNum(&pres, 1.073191101889334,	-1.5891086635291682,	0.5719752455457148);
	BQ_setNum(&high, 1.1529584489838083,	-0.8898590742736775,	0.1054923224547937);

	/* Set the 2nd order transfer function numerator coeffecients
	 *  analog to matlab's A = [1.0 -1.8153 0.8310]
	 *  only two values are needed, filter coeffecients must be normalized
	 *  with respect to A[0]
	 */
	BQ_setDen(&low, -1.967424639045396,	0.967803951986572);
	BQ_setDen(&mid, -1.892567957556463,	0.8966287853738223);
	BQ_setDen(&pres, -1.5891086635291682,	0.645166347435049);
	BQ_setDen(&high, -0.8898590742736775,	0.25845077143860207);

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
int16_t outputSample = 0; // initialise sample variable
int16_t signal = 0;


int i = 0;
int gain = 2;


s16 = read_audio_sample(); // get current input sample



// implement distortion algorithm
if(dist){
	s16 = s16*gain;
	// gain stage 1
	if(s16 > 8192){
		buffer[writeIndex] = round(s16/(2*gain));
	//buffer[writeIndex] = buffer[writeIndex]/(4*gain);
	}else
	if(s16 < -21844){
		buffer[writeIndex] = round(s16/(2*gain));
	}else{
		buffer[writeIndex] = s16;
	}
}else{
	buffer[writeIndex]= s16;
}
writeIndex = (writeIndex + SIZE_OF_BUFFER - 1) % SIZE_OF_BUFFER;


// implement EQ Stage
if(eqBypass){
	if(lowCut){
	    buffer[writeIndex] = BQ_process(&low, buffer[writeIndex]);
	}
	if(midCut){
		buffer[writeIndex] = BQ_process(&mid, buffer[writeIndex]);
	}
	if(presCut){
		buffer[writeIndex] = BQ_process(&pres, buffer[writeIndex]);
	}
	if(highCut){
		buffer[writeIndex] = BQ_process(&high, buffer[writeIndex]);
    }
}


//convert to frequency domain for FIR
//fftss_plan fftss_plan_dft_1d(SIZE_OF_BUFFER, in*, out*, i, FFTSS_ESTIMATE);
//apply cab sim
//	for(i=0;i<SIZE_OF_BUFFER;i++){
//	signal += (buffer[writeIndex+i % SIZE_OF_BUFFER] * b_fir[i]);
//}

// revert to time domain
outputSample = round(buffer[writeIndex]);

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
