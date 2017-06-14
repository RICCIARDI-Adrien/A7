/** @file System_Internal_EEPROM.c
 * @see System_Internal_EEPROM.h for description.
 * @author Adrien RICCIARDI
 */
#include <System_Internal_EEPROM.h>
#include <xc.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned char SystemInternalEEPROMReadByte(unsigned short Address)
{
	// Make sure address is in range
	if (Address >= SYSTEM_INTERNAL_EEPROM_SIZE) return 0;
	
	// Set address
	EEADR = (unsigned char) Address;
	EEADRH = Address >> 8;
	
	// Configure read operation
	EECON1 = 0; // Select data EEPROM memory, disable writing, do not start reading directly
	EECON1bits.RD = 1; // Start reading
	
	return EEDATA;
}

void SystemInternalEEPROMWriteByte(unsigned short Address, unsigned char Byte)
{
	// Make sure address is in range
	if (Address >= SYSTEM_INTERNAL_EEPROM_SIZE) return;
	
	// Set address
	EEADR = (unsigned char) Address;
	EEADRH = Address >> 8;
	
	// Set data to write
	EEDATA = Byte;
	
	// Configure write operation
	EECON1 = 0x04; // Select data EEPROM memory, enable writing
	
	// Send the magic sequence
	INTCONbits.GIE = 0; // Disable interrupts as this sequence must not be disturbed
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1bits.WR = 1; // Start write operation
	INTCONbits.GIE = 1; // Re-enable interrupts
	
	// Wait for the write operation to terminate
	while (EECON1bits.WR);
	
	// Disable write operation
	EECON1 = 0;
}
