/** @file Main.c
 * Determine which demo to display and display it.
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** How many demos are available. */
#define DEMOS_COUNT (sizeof(Demo_Functions) / sizeof(Demo_Functions[0]))

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A demo function callback. */
typedef void (*TDemoFunction)(void);

//-------------------------------------------------------------------------------------------------
// Prototypes
//-------------------------------------------------------------------------------------------------
static void DemoFillScreenUpToDown(void);
static void DemoFillScreenDownToUp(void);
static void DemoFillScreenLeftToRight(void);
static void DemoFillScreenRightToLeft(void);

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Keep the last pressed key. */
static unsigned char Last_Pressed_Key = 0;

/** All available demos. */
TDemoFunction Demo_Functions[] =
{
	DemoFillScreenUpToDown,
	DemoFillScreenDownToUp,
	DemoFillScreenLeftToRight,
	DemoFillScreenRightToLeft
};

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Fill the screen from top to bottom. */
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

/** Fill the screen from bottom to top. */
static void DemoFillScreenDownToUp(void)
{
	unsigned char X, Y;
	
	for (Y = SYSTEM_DISPLAY_HEIGHT - 1; Y != 255 ; Y--)
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

/** Fill the screen from left to right. */
static void DemoFillScreenLeftToRight(void)
{
	unsigned char X, Y;
	
	for (X = 0; X < SYSTEM_DISPLAY_WIDTH; X++)
	{
		for (Y = 0; Y < SYSTEM_DISPLAY_HEIGHT / 2; Y++)
		{
			// Update screen
			SystemDisplaySetPixelState(X, Y, 1); // Set upper pixel
			SystemDisplaySetPixelState(X, SYSTEM_DISPLAY_HEIGHT - 1 - Y, 1); // Set downer pixel
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

/** Fill the screen from right to left. */
static void DemoFillScreenRightToLeft(void)
{
	unsigned char X, Y;
	
	for (X = SYSTEM_DISPLAY_WIDTH - 1; X != 255; X--)
	{
		for (Y = 0; Y < SYSTEM_DISPLAY_HEIGHT / 2; Y++)
		{
			// Update screen
			SystemDisplaySetPixelState(X, Y, 1); // Set upper pixel
			SystemDisplaySetPixelState(X, SYSTEM_DISPLAY_HEIGHT - 1 - Y, 1); // Set downer pixel
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
	unsigned char i = 0;
	
	SystemInitialize();
	
	while (1)
	{
		SystemDisplayClear();
		
		// Start next demo
		Demo_Functions[i]();
		i++;
		if (i >= DEMOS_COUNT) i = 0;
		
		// Change demo or quit program according to pressed key
		if (Last_Pressed_Key == SYSTEM_KEYBOARD_KEY_CODE_ESCAPE) SystemExitProgram();
	}
}
