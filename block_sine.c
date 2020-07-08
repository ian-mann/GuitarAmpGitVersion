// ======== block_sine.c =================================
// The coefficient A and the three initial values
// generate a 500Hz tone (sine wave) when running
// at a sample rate of 48KHz.
//
// Even though the calculations are done in floating
// point, this function returns a short value since
// this is what's needed by a 16-bit codec (DAC).

// ======== Prototypes ===================================
void blockSine(short *buf, int len);
short sineGen(void);

// ======== Definitions ==================================
// Initial values
#define Y1 0.0654031	// = sin((f_tone/f_samp) * 360)
						// = sin((500Hz / 48KHz) * 360)
						// = sin (3.75)
#define AA 1.9957178	// = 2 * cos(3.75)

// ======== Globals =====================================
static float y[3] = {0,Y1,0};
static float A = AA;

// ======== sineGen ======================================
// Generate a single element of sine data
short sineGen(void)
{
	y[0] = y[1] * A - y[2];
	y[2] = y[1];
	y[1] = y[0];

	// To scale full 16-bit range we would multiply y[0]
	// by 32768 using a number slightly less than this
	// (such as 32000) helps to prevent overflow.
	y[0] *= 32000;
	// We recast the result to a short value upon returning it
	// since the D/A converter is programmed to accept 16-bit
	// signed values.
	return((short)y[0]);
}

// ======== blockSine ========
// Generate a block of sine data using sineGen
void blockSine(short *buf, int len)
{
	int i = 0;
	for (i = 0;i < len; i++) {
		buf[i] = sineGen();
	}
}

