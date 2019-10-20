/** @file Main.c
 * Determine which demo to display and display it.
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Keep the last pressed key. */
static unsigned char Last_Pressed_Key;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Fill the screen with characters from up to bottom. */
static void DemoFillScreenUpToDown(void)
{
	unsigned char X, Y;
	
	for (Y = 0; Y < SYSTEM_DISPLAY_HEIGHT; Y++)
	{
		for (X = 0; X < SYSTEM_DISPLAY_WIDTH / 2; X++)
		{
			// Update screen
			SystemDisplaySetPixelState(X, Y, 1); // Set leftmost pixel
			SystemDisplaySetPixelState(SYSTEM_DISPLAY_WIDTH - 1 - X, Y, 1); // Set rightmost pixel
			SystemDisplayUpdate();
			
			// Exit if a key is pressed
			if (SystemKeyboardIsKeyAvailable())
			{
				Last_Pressed_Key = SystemKeyboardReadCharacter();
				return;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	SystemInitialize();
	
	// Reinitialize the variable each time the demo is called, or it will contain the last hit key, which may be the exiting character...
	Last_Pressed_Key = 0;
	
	while (1)
	{
		SystemDisplayClear();
		
		DemoFillScreenUpToDown();
		
		
		// Change transition or quit program according to pressed key
		if (Last_Pressed_Key == SYSTEM_KEYBOARD_KEY_CODE_ESCAPE) SystemExitProgram();
	}
}
