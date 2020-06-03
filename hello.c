//---------------------------------------------------------
// main example 3
//---------------------------------------------------------
#include "hellocfg.h" //BIOS include file
#include "framework.h"
#include "stdbool.h"
int16_t volatile mask = 0xffff;
bool filterOn = false;
double filterCoef[65] = {
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
float buffer[64] = {0} ;



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

int16_t audio(int16_t s16){
	// put sample into first slot of array and
		// move the samples one to the left
		int i = 0, j = 0;
		buffer[0] = s16;


		if(filterOn){
		 // convolution of buffer and filter coefficients
		 // Separate filter into three if statements
		float sum = 0;
		for (i=0;i<129;i++){
			sum = 0;
		   if(i<65){
		   // overlap has not occurred, use up to current value
		      for(j=0;j<=i;j++)
		      {
		       sum = sum + (buffer[j]*filterCoef[i-j]);
		      }

		   }else
		   if(i>=65 && i<129){
		   // full overlap, use full filter
		      for(j=0;j<65;j++)
		      {
		       sum = sum + (buffer[i-65+j]*filterCoef[64-j]);
		      }
		   }else
		   if(i>=129){
		   // overlap past, use from i to samples remaining
		       for(j=0;j<(129-i);j++)
		       {
		       sum = buffer[(129-i)+j]*filterCoef[65-j] + sum;
		       }
		   }
		   s16 = sum;
		   }

		}


		else{if(filterOn == false){

			s16 = buffer[65];
		}
		}
		for(i=1;i<64;i++){
					if(i<64){
					buffer[i] = buffer[i-1];
				}
			}
return(s16);
}


//---------------------------------------------------------
//---------------------------------------------------------
void audioHWI(void)
{
int16_t s16=0;
s16 = read_audio_sample();
s16 = audio(s16);

if (MCASP->RSLOT)
{
// THIS IS THE LEFT CHANNEL!!!
s16 &= mask;
}
else {
// THIS IS THE RIGHT CHANNEL!!!
s16 &= mask;
}
write_audio_sample(s16);
}
