/** @file System_Serial_Port.c
 * @see System_Serial_Port.h for description.
 * @author Adrien RICCIARDI
 */
#include <System_Serial_Port.h>
#include <xc.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemSerialPortInitialize(void)
{
	// Configure baud rate generation, computed with following formula : SPBRGH:SPBRG = (Fosc / (4 * Baud_Rate)) - 1
	SPBRGH2 = 0;
	SPBRG2 = 138; // With this value we get 115107.9 bit/s, which is a 0.08% error
	
	// Configure UART for asynchronous operation
	TXSTA2 = 0x24; // Select 8-bit transmission, enable transmitter, select asynchronous mode, enable high speed baud rate generation (to allow the above formula to be used)
	RCSTA2 = 0x90; // Enable serial port, select 8-bit reception, enable receiver
	BAUDCON2 = 0x08; // Use default signal polarities, enable 16-bit baud rate generator, disable auto-baud detection
	
	// Configure UART pins (no need to disable analog feature as RB7 and RB6 have not analog input)
	TRISBbits.RB7 = 1; // Disable pin output driver (set them as input)
	TRISBbits.RB6 = 1;
}

unsigned char SystemSerialPortReadByte(void)
{
	// Wait for a byte to be received
	while (!PIR3bits.RC2IF);
	
	// Get the byte
	return RCREG2;
}

void SystemSerialPortWriteByte(unsigned char Data)
{
	// Wait for the bus to be idle
	while (!PIR3bits.TX2IF);
	
	// Send the byte
	TXREG2 = Data;
}

void SystemSerialPortWriteString(const unsigned char *String)
{
	while (*String != 0)
	{
		// Send the next character (do not call SystemSerialPortWriteByte() to save some cycle and stack space)
		while (!PIR3bits.TX2IF); // Wait for the bus to be idle
		TXREG2 = *String; // Send the character
		
		String++;
	}
}