/** @file Main.c
 * A simple game using dichotomy to find an unknown number.
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	SystemInitialize();
	
	LATB = 0xAA;
	TRISB = 0;
	while (1)
	{
		LATB = ~PORTB;
		__delay_ms(500);
	}
}
