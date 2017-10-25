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
	unsigned char Input_Buffer[AES_BLOCK_SIZE], Output_Buffer[AES_BLOCK_SIZE], i, Key[AES_256_KEY_SIZE];
	
	SystemInitialize();
	
	// Select encryption or decryption mode
	
	// Receive key
	for (i = 0; i < AES_256_KEY_SIZE; i++) Key[i] = SystemSerialPortReadByte();
	SystemDisplayRenderTextString("Received key.\n");
	SystemDisplayRenderFrameBuffer();
	
	// Receive initialization vector (TODO remove if CTR only ?)
	
	// Initialize operation
	AES256CTRInitialize(Key); // TODO cipher and decipher support
	SystemDisplayRenderTextString("Initialized AES\nengine.\n");
	SystemDisplayRenderFrameBuffer();
	
	while (1)
	{
		// Receive a chunk of data from the UART
		for (i = 0; i < AES_BLOCK_SIZE; i++) Input_Buffer[i] = SystemSerialPortReadByte();
		SystemDisplayRenderTextString("Received data.\n");
		SystemDisplayRenderFrameBuffer();
		
		// Process it
		AES256CTRUpdate(Input_Buffer, Output_Buffer);
		SystemDisplayRenderTextString("FUCK.\n");
		SystemDisplayRenderFrameBuffer();
		
		// Send the processed data back
		for (i = 0; i < AES_BLOCK_SIZE; i++) SystemSerialPortWriteByte(Output_Buffer[i]);
		
		// Update display
		
	}
}
