/** @file Main.c
 * AES test entry point and main loop.
 * @author Adrien RICCIARDI
 */
#include <AES.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	unsigned char Input_Buffer[AES_BLOCK_SIZE], Output_Buffer[AES_BLOCK_SIZE], i;
	
	SystemInitialize();
	
	// Select encryption or decryption mode (TODO remove if CTR only ?)
	
	// Receive key
	
	// Receive initialization vector
	
	while (1)
	{
		// Receive a chunk of data from the UART
		for (i = 0; i < AES_BLOCK_SIZE; i++) Input_Buffer[i] = SystemSerialPortReadByte();
		
		// Process it
		
		// Send the processed data back
		for (i = 0; i < AES_BLOCK_SIZE; i++) SystemSerialPortWriteByte(Output_Buffer[i]);
		
		// Update display
		
	}
}
