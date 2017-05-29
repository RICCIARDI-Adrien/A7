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
	// Seed the random numbers generator with a random value (timer 4 started counting before the main oscillator was stable, so it's value is hard to predict)
	System_Random_Seed = TMR4;
}

unsigned char SystemRandomGetNumber(void)
{
	System_Random_Seed = (((System_Random_Seed << 1) + System_Random_Seed) - 7) ^ TMR4; // New_Seed = (Previous_Seed * 3 - 7) XOR Arbitrary_Value
	return System_Random_Seed;
}