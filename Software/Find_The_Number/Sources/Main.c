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
	unsigned char i, Inverted_Value, Value;
	
	
	SystemInitialize();
	
	SystemSerialPortWriteString("aaa init string\r\n"); // Sending "aaa" first allows to use UART on RB6 and RB7 pins without being too bored by mikroProg on a Easy PIC 7 MikroElektronika board (value found by chance)
	SystemSerialPortWriteString("This is a test string\r\n");
	
	LATB = 0;
	TRISB = 0;
	while (1)
	{
		// Wait for a character to be received
		/*while (!PIR1bits.RC1IF);
		
		Inverted_Value = RCREG1;
		Value = 0;
		for (i = 0; i < 7; i++)
		{
			Value |= Inverted_Value & 1;
			Inverted_Value >>= 1;
			Value <<= 1;
		}
		LATB = Value;*/
		//LATB = RCREG1;
		
		SystemSerialPortWriteString("bla ");
		__delay_ms(500);
		SystemSerialPortWriteString("blo ");
		__delay_ms(500);
	}
}
