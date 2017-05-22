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
	// Use the unknown value stored in the CCPR5 registers to initialize the random number generator (it is possible because CCP5 module is not used, so it's content is not altered)
	System_Random_Seed = CCPR5L;

	// Initialize the timer 4 to count at its maximum frequency
	T4CON = 0x04; // Enable the timer with a 1:1 prescaler and 1:1 postscaler
}

unsigned char SystemRandomGetNumber(void)
{
	System_Random_Seed = (((System_Random_Seed << 1) + System_Random_Seed) - 7) ^ TMR4; // New_Seed = (Previous_Seed * 3 - 7) XOR Arbitrary_Value
	return System_Random_Seed;
}