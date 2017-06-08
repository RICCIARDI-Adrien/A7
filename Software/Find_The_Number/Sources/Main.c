/** @file Main.c
 * A simple game using dichotomy to find an unknown number.
 * @author Adrien RICCIARDI
 */
#include <System.h>

void hex(unsigned char val, unsigned char *msb, unsigned char *lsb)
{
	unsigned char i;
	
	i = val >> 4;
	if (i < 10) *msb = i + '0';
	else *msb = i + 'A' - 10;
	
	i = val & 0x0F;
	if (i < 10) *lsb = i + '0';
	else *lsb = i + 'A' - 10;
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	unsigned char c, buf1[256], buf2[256], msb, lsb;
	unsigned short i;
	
	SystemInitialize();
	
	// TEST
	SystemSerialPortWriteString("aaa init string\r\n"); // Sending "aaa" first allows to use UART on RB6 and RB7 pins without being too bored by mikroProg on a Easy PIC 7 MikroElektronika board (value found by chance)
	SystemSerialPortWriteString("This is a test string\r\n");
	
	SystemDisplayClearFrameBuffer();
	
	SystemDisplaySetTextCursor(0, 0);
	SystemDisplayRenderTextString("ABCDEFGHIJKLM");
	SystemDisplaySetTextCursor(0, 1);
	SystemDisplayRenderTextString("NOPQRSTUVWXYZ");
	SystemDisplaySetTextCursor(0, 2);
	SystemDisplayRenderTextString("0123456789");
	
	SystemDisplayRenderFrameBuffer();
	
#if 0
	while (1)
	{
		c = SystemSerialPortReadByte();
		
		if ((c >= 'A') && (c <= 'Z')) c += 32;
		
		SystemSerialPortWriteByte(c);
	}
#endif

#if 0
	for (i = 0; i < 256; i++)
	{
		buf1[i] = 255 - (unsigned char) i;
		buf2[i] = 0;
	}

	SystemExternalEEPROMWritePage(0, buf1);
	
	SystemExternalEEPROMReadPage(0, buf2);
	
	for (i = 0; i < 256; i++)
	{
		hex(i, &msb, &lsb);
		SystemSerialPortWriteByte(msb); SystemSerialPortWriteByte(lsb);
		SystemSerialPortWriteString(" : ");
		
		hex(buf2[i], &msb, &lsb);
		SystemSerialPortWriteByte(msb); SystemSerialPortWriteByte(lsb);
		SystemSerialPortWriteString("\r\n");
	}
#endif
	
	// TEST
	while (1);
}
