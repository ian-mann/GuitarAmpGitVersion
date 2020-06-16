//---------------------------------------------------------
// main example 3
//---------------------------------------------------------
#include "hellocfg.h" //BIOS include file
#include "framework.h"
#include "stdbool.h"
<<<<<<< Updated upstream
=======
#include "stdio.h"
#include "math.h"
#define SIZE_OF_BUFFER 115

>>>>>>> Stashed changes
int16_t volatile mask = 0xffff;
bool filterOn = false;
<<<<<<< Updated upstream
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
=======
bool dist = false;

float lowPass1[16] = {0.0047, -0.0245,0.0017,0.0475,
		   -0.0236,-0.1019,0.1137,
		    0.4761,0.4761, 0.1137,-0.1019,-0.0236,
		    0.0475,0.0017,-0.0245,0.0047};
float b_fir[115] = { -0.5754148f, -0.5896387f, -0.5989829f, -0.6004975f, -0.5866649f, -0.5497405f, -0.4913402f, -0.4165098f, -0.3331017f, -0.2444414f, -0.1530165f, -0.0629647f, 0.0199576f, 0.0868580f, 0.1351727f, 0.1624212f, 0.1670499f, 0.1552913f, 0.1346696f, 0.1063597f, 0.0730300f, 0.0381056f, 0.0044063f, -0.0257079f, -0.0494872f, -0.0653318f, -0.0727434f, -0.0766303f, -0.0848365f, -0.1009002f, -0.1254545f, -0.1543583f, -0.1794888f, -0.1937449f, -0.1982255f, -0.1960887f, -0.1845738f, -0.1612266f, -0.1268770f, -0.0838952f, -0.0372428f, 0.0088834f, 0.0509044f, 0.0823900f, 0.0951202f, 0.0881258f, 0.0711371f, 0.0561935f, 0.0537979f, 0.0699705f, 0.1043703f, 0.1501353f, 0.2038217f, 0.2638244f, 0.3224580f, 0.3715794f, 0.4092365f, 0.4387230f, 0.4671314f, 0.5022833f, 0.5443884f, 0.5842426f, 0.6146141f, 0.6353303f, 0.6441354f, 0.6399713f, 0.6261089f, 0.6045788f, 0.5737941f, 0.5326034f, 0.4813460f, 0.4234440f, 0.3655206f, 0.3121295f, 0.2642741f, 0.2242847f, 0.1976960f, 0.1845612f, 0.1775637f, 0.1699609f, 0.1613397f, 0.1539324f, 0.1494795f, 0.1473434f, 0.1403642f, 0.1210576f, 0.0911203f, 0.0552433f, 0.0127345f, -0.0412670f, -0.1087119f, -0.1803046f, -0.2408597f, -0.2835957f, -0.3088048f, -0.3170241f, -0.3126892f, -0.3027193f, -0.2910105f, -0.2781734f, -0.2658190f, -0.2572001f, -0.2517777f, -0.2492671f, -0.2511320f, -0.2592731f, -0.2749218f, -0.2966038f, -0.3215005f, -0.3454913f, -0.3676802f, -0.3871153f, -0.4006417f, -0.4070456f, -0.4068140f };
float a_fir = -2.37366f;

>>>>>>> Stashed changes



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

<<<<<<< Updated upstream
	for(i=1;i<64;i++){ // move every item in buffer one to the right
			buffer[i] = buffer[i-1];
=======
writeIndex = (writeIndex + SIZE_OF_BUFFER - 1) % SIZE_OF_BUFFER;
buffer[writeIndex] = s16;

// implement distortion algorithm
if(dist){
}

// implement buffer and convolution

if(filterOn){
	for(i=0;i<SIZE_OF_BUFFER;i++){

	signal += buffer[(writeIndex + i) % SIZE_OF_BUFFER] * b_fir[SIZE_OF_BUFFER-1-i];

>>>>>>> Stashed changes
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
