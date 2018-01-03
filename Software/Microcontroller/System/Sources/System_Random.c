/** @file System_Random.c
 * @see System_Random.h for description.
 * @author Adrien RICCIARDI
 */
#include <System_Random.h>
#include <xc.h>

//--------------------------------------------------------------------------------------------------
// Private variables
//--------------------------------------------------------------------------------------------------
/** The random seed. */
static unsigned char System_Random_Seed;

//--------------------------------------------------------------------------------------------------
// Public functions
//--------------------------------------------------------------------------------------------------
void SystemRandomInitialize(void)
{
	// Seed the random numbers generator with a random value (timers started counting before the main oscillator was stable, so their values are hard to predict)
	System_Random_Seed = TMR4 ^ TMR0;
}

unsigned char SystemRandomGetNumber(void)
{
	unsigned char Is_Least_Significant_Bit_Set;
	
	// Use a 8-bit Galois LFSR to generate good pseudo-random numbers (see https://en.wikipedia.org/wiki/Linear-feedback_shift_register#Galois_LFSRs)
	if (System_Random_Seed & 0x01) Is_Least_Significant_Bit_Set = 1;
	else Is_Least_Significant_Bit_Set = 0;
	System_Random_Seed >>= 1;
	if (Is_Least_Significant_Bit_Set) System_Random_Seed ^= 0xB4;
	
	// Add some more randomness thanks to this unknown timer value
	System_Random_Seed ^= TMR4;
	
	return System_Random_Seed;
}
