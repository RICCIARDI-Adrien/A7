/** @file System_External_EEPROM.c
 * @see System_External_EEPROM.h for description.
 * @author Adrien RICCIARDI
 * @note Pins mapping : /SS = RC2, SCK = RC3, SDI = RC4, SDO = RC5.
 */
#include <System_External_EEPROM.h>
#include <xc.h>

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Read data chip command. */
#define SYSTEM_EXTERNAL_EEPROM_COMMAND_READ_DATA 0x03
/** Write data chip command. */
#define SYSTEM_EXTERNAL_EEPROM_COMMAND_WRITE_DATA 0x02
/** Read status register chip command. */
#define SYSTEM_EXTERNAL_EEPROM_COMMAND_READ_STATUS_REGISTER 0x05
/** Write enable chip command. */
#define SYSTEM_EXTERNAL_EEPROM_COMMAND_ENABLE_WRITE_OPERATION 0x06

/** Set chip /SS to low level (hold time must be at least 50ns, so loose a 62.5ns instruction to make sure /SS is held). */
#define SYSTEM_EXTERNAL_EEPROM_SELECT_DEVICE() \
{ \
	LATCbits.LATC2 = 0; \
	asm("nop"); \
}

/** Set chip /SS to high level (hold time must be at least 50ns, so loose a 62.5ns instruction to make sure /SS is held). */
#define SYSTEM_EXTERNAL_EEPROM_UNSELECT_DEVICE() \
{ \
	LATCbits.LATC2 = 1; \
	asm("nop"); \
}

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Transfer a byte on the SPI bus.
 * @param Byte The byte to transmit.
 * @return The byte received from the addressed device.
 */
static unsigned char SystemExternalEEPROMTransferSPIByte(unsigned char Byte)
{
	// Start transfer
	SSP1BUF = Byte;
	
	// Wait for transfer end
	while (!PIR1bits.SSP1IF);
	PIR1bits.SSP1IF = 0; // Reset interrupt flag
	
	// Interrupt flag is set too soon (in the middle of the last clock pulse), so wait a full period (= 1/4MHz) to be sure transmission has finished
	asm("nop\n");
	asm("nop\n");
	asm("nop\n");
	asm("nop\n");
	
	// Get byte received from device
	return SSP1BUF;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemExternalEEPROMInitialize(void)
{
	// Prepare pins to be used by the SPI module
	// Set all pins to digital
	ANSELC &= 0xC3;
	// Configure pins direction
	TRISCbits.TRISC2 = 0; // /SS pin
	TRISCbits.TRISC3 = 0; // SCK pin
	TRISCbits.TRISC4 = 1; // SDI pin
	TRISCbits.TRISC5 = 0; // SDO pin
	
	// Configure MSSP1 for SPI master operation
	SSP1STAT = 0; // Sample input data at middle of data output time, transmit on transition from idle to active clock state
	SSP1CON1 = 0x21; // Enable SPI module, set idle state as low level, select SPI master mode with Fosc/16 = 4MHz clock (this is the fastest clock that seems to be working and stable)
}

void SystemExternalEEPROMReadPage(unsigned short Page_Number, unsigned char *Pointer_Buffer)
{
}

void SystemExternalEEPROMWritePage(unsigned short Page_Number, unsigned char *Pointer_Buffer)
{
	unsigned short i;
	unsigned char Status_Register;
	
	// Make sure address is in range
	if (Page_Number >= SYSTEM_EXTERNAL_EEPROM_PAGES_COUNT) return;
	
	// Allow writing by setting the Write Enable Latch
	SYSTEM_EXTERNAL_EEPROM_SELECT_DEVICE();
	SystemExternalEEPROMTransferSPIByte(SYSTEM_EXTERNAL_EEPROM_COMMAND_ENABLE_WRITE_OPERATION);
	SYSTEM_EXTERNAL_EEPROM_UNSELECT_DEVICE(); // Device must be selected then unselected for the command to work, as specified by datasheet
	
	// Write data to specified page
	SYSTEM_EXTERNAL_EEPROM_SELECT_DEVICE();
	// Send write command
	SystemExternalEEPROMTransferSPIByte(SYSTEM_EXTERNAL_EEPROM_COMMAND_WRITE_DATA);
	// Send 24-bit address (as the parameter provided to the function is a 256-byte page address, there is only to multiply this value by 256 to get the byte address. This will lead to the 8 least significant bits set to zero. Below code avoids doing this useless computation and directly sends zero as the last address part)
	SystemExternalEEPROMTransferSPIByte(Page_Number >> 8);
	SystemExternalEEPROMTransferSPIByte((unsigned char) Page_Number);
	SystemExternalEEPROMTransferSPIByte(0);
	// Send data to write
	for (i = 0; i < SYSTEM_EXTERNAL_EEPROM_PAGE_SIZE; i++)
	{
		SystemExternalEEPROMTransferSPIByte(*Pointer_Buffer);
		Pointer_Buffer++;
	}
	SYSTEM_EXTERNAL_EEPROM_UNSELECT_DEVICE(); // Start write cycle
	
	// Wait for the write cycle to finish
	do
	{
		// Read status register
		SYSTEM_EXTERNAL_EEPROM_SELECT_DEVICE();
		SystemExternalEEPROMTransferSPIByte(SYSTEM_EXTERNAL_EEPROM_COMMAND_READ_STATUS_REGISTER); // Send command
		Status_Register = SystemExternalEEPROMTransferSPIByte(0); // Send a dummy byte to allow the device to transmit its answer
		SYSTEM_EXTERNAL_EEPROM_UNSELECT_DEVICE();
	} while (Status_Register & 0x01);
}