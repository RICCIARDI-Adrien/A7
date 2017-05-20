/** @file Main.c
 * A simple game using dichotomy to find an unknown number.
 * @author Adrien RICCIARDI
 */
#include <System.h>

void bla(void);
extern unsigned char System_Display_Frame_Buffer[512];

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	unsigned char i, Inverted_Value, Value, tmp;
	signed short x = 0, y = 0, Vertical_Speed = 8, Horizontal_Speed = 1;
	
	
	SystemInitialize();
	
	SystemSerialPortWriteString("aaa init string\r\n"); // Sending "aaa" first allows to use UART on RB6 and RB7 pins without being too bored by mikroProg on a Easy PIC 7 MikroElektronika board (value found by chance)
	SystemSerialPortWriteString("This is a test string\r\n");
	
	//LATB = 0;
	//TRISB = 0;
	while (1)
	{
		#if 0
		// Wait for a character to be received
		while (!PIR1bits.RC1IF);
		RCSTA1bits.CREN = 0;
		
		tmp = RCSTA1 & 0x01;
		
		Inverted_Value = RCREG1;
		/*Value = 0;
		for (i = 0; i < 7; i++)
		{
			Value |= Inverted_Value & 1;
			Inverted_Value >>= 1;
			Value <<= 1;
		}
		LATB = Value;*/
		//LATB = Inverted_Value;
		
		Inverted_Value >>= 1;
		if (tmp) Inverted_Value |= 0x80;
		SystemSerialPortWriteByte(Inverted_Value);
		SystemSerialPortWriteByte(tmp);
		SystemSerialPortWriteByte('\r');
		SystemSerialPortWriteByte('\n');
		
		RCSTA1bits.CREN = 1;
		
		#endif
		
		#if 0
		SystemSerialPortWriteByte(SystemKeyboardReadCharacter());
		#endif
		
		SystemDisplayBeginRendering();
		
		/*System_Display_Frame_Buffer[0] = 0xAA;
		System_Display_Frame_Buffer[127] = 0x55;
		System_Display_Frame_Buffer[128] = 0xCC;*/
		
		System_Display_Frame_Buffer[y * 128 + x] = 0xFF;
		System_Display_Frame_Buffer[y * 128 + x + 1] = 0xFF;
		System_Display_Frame_Buffer[y * 128 + x + 2] = 0xFF;
		System_Display_Frame_Buffer[y * 128 + x + 3] = 0xFF;
		System_Display_Frame_Buffer[y * 128 + x + 4] = 0xFF;
		System_Display_Frame_Buffer[y * 128 + x + 5] = 0xFF;
		System_Display_Frame_Buffer[y * 128 + x + 6] = 0xFF;
		System_Display_Frame_Buffer[y * 128 + x + 7] = 0xFF;
		
		SystemDisplayEndRendering();
		__delay_ms(66);
	//__delay_ms(1000);
		
		x += Vertical_Speed;
		y += Horizontal_Speed;
		
		if (x >= 120)
		{
			x = 119;
			Vertical_Speed = -8;
		}
		else if (x < 0)
		{
			x = 0;
			Vertical_Speed = 8;
		}
		
		if (y >= 8)
		{
			y = 7;
			Horizontal_Speed = -1;
		}
		else if (y < 0)
		{
			y = 0;
			Horizontal_Speed = 1;
		}
	}
}
