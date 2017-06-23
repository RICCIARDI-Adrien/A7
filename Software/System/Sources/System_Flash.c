/** @file System_Flash.c
 * @see System_Flash.h for description.
 * @author Adrien RICCIARDI
 */
#include <System_Flash.h>
#include <xc.h>

//--------------------------------------------------------------------------------------------------
// Public functions
//--------------------------------------------------------------------------------------------------
void SystemFlashWriteBlock(unsigned short Block_Address, unsigned char *Pointer_Data_Buffer)
{
	unsigned char i;
	
	// Make sure the address is in the allowed range
	if (Block_Address >= SYSTEM_FLASH_BLOCKS_COUNT) return;
	// Convert the block address to a byte address
	Block_Address <<= 6;

	// Erase the block
	// Load the block starting address
	TBLPTRU = 0; // No need to set the upper bits because there is not enough memory to use them on this microcontroller
	TBLPTRH = Block_Address >> 8;
	TBLPTRL = (unsigned char) Block_Address;
	// Configure an erase cycle
	EECON1 = 0x94; // Access flash program memory, enable block erasing on next write cycle, allow writing to the program memory
	// Execute the special write sequence
	INTCONbits.GIE = 0; // Disable interrupts during the critical operation
	EECON2 = 0x55;
	EECON2 = 0xAA;
	// Start the erase cycle (the core will stall until the erase cycle is finished)
	EECON1bits.WR = 1;
	INTCONbits.GIE = 1; // Re-enable interrupts
	
	// Write the block
	// Reload the block start address as it has been altered by the erase cycle
	TBLPTRU = 0;
	TBLPTRH = Block_Address >> 8;
	TBLPTRL = (unsigned char) Block_Address;
	// Write the data to the holding registers
	for (i = 0; i < SYSTEM_FLASH_BLOCK_SIZE; i++)
	{
		TABLAT = *Pointer_Data_Buffer;
		#asm
			tblwt*+ // Write the byte to the next holding register and automatically increment the address
		#endasm
		Pointer_Data_Buffer++;
	}
	// Return to the block by setting the address another time (it is now targeting the next block due to the previous automatic address incrementation)
	TBLPTRU = 0;
	TBLPTRH = Block_Address >> 8;
	TBLPTRL = (unsigned char) Block_Address;
	// Configure a write cycle
	EECON1 = 0x84; // Access flash program memory, allow writing to the program memory
	// Execute the special write sequence
	INTCONbits.GIE = 0; // Disable interrupts during the critical operation
	EECON2 = 0x55;
	EECON2 = 0xAA;
	// Start the write cycle
	EECON1bits.WR = 1;
	INTCONbits.GIE = 1; // Re-enable interrupts
	
	// Disable writing to flash memory
	EECON1bits.WREN = 0;
}
