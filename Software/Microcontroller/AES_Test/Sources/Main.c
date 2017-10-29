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
	unsigned char Buffer[AES_BLOCK_SIZE], i, Key[AES_256_KEY_SIZE];
	
	SystemInitialize();
	
	// Select encryption or decryption mode
	
	// Receive key
	for (i = 0; i < AES_256_KEY_SIZE; i++) Key[i] = SystemSerialPortReadByte();
	SystemDisplayRenderTextString("Received key.\n");
	SystemDisplayRenderFrameBuffer();
	
	// Initialize operation
	AES256CBCInitialize(Key); // TODO cipher and decipher support
	SystemDisplayRenderTextString("Initialized engine.\n");
	SystemDisplayRenderFrameBuffer();
	
	while (1)
	{
		// Receive a chunk of data from the UART
		for (i = 0; i < AES_BLOCK_SIZE; i++) Buffer[i] = SystemSerialPortReadByte();
		
		// Process it
		AES256CBCUpdate(Buffer);
		
		// Send the processed data back
		for (i = 0; i < AES_BLOCK_SIZE; i++) SystemSerialPortWriteByte(Buffer[i]);
		
		// Update display
		
	}
}
