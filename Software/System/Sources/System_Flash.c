/** @file System_Flash.c
 * @see System_Flash.h for description.
 * @author Adrien RICCIARDI
 */
#include <System_Flash.h>
#include <xc.h>

//--------------------------------------------------------------------------------------------------
// Public functions
//--------------------------------------------------------------------------------------------------
void SystemFlashWriteBlock(unsigned long Block_Address, unsigned char *Pointer_Data_Buffer)
{
	unsigned char i;

	// Erase the block
	// Load the block starting address
	TBLPTRU = Block_Address >> 16;
	TBLPTRH = Block_Address >> 8;
	TBLPTRL = (unsigned char) Block_Address;
	// Configure an erase cycle
	EECON1 = 0x94; // Access flash program memory, enable block erasing on next write cycle, allow writing to the program memory
	// Execute the special write sequence
	EECON2 = 0x55;
	EECON2 = 0xAA;
	// Start the erase cycle (the core will stall until the erase cycle is finished)
	EECON1bits.WR =1;
	
	// Write the block
	// Reload the block start address as it has been altered by the erase cycle
	TBLPTRU = Block_Address >> 16;
	TBLPTRH = Block_Address >> 8;
	TBLPTRL = (unsigned char) Block_Address;
	// Write the data to the holding registers
	for (i = 0; i < SYSTEM_FLASH_BLOCK_SIZE; i++)
	{
		TABLAT = *Pointer_Data_Buffer;
		asm("tblwt*+\n"); // Write the byte to the next holding register and automatically increment the address
		Pointer_Data_Buffer++;
	}
	// Return to the good block without setting the address another time
	asm("tblrd*-\n");
	// Configure a write cycle
	EECON1 = 0x84; // Access flash program memory, allow writing to the program memory
	// Execute the special write sequence
	EECON2 = 0x55;
	EECON2 = 0xAA;
	// Start the write cycle
	EECON1bits.WR = 1;
	
	// Disable writing to flash memory
	EECON1bits.WREN = 0;
}
