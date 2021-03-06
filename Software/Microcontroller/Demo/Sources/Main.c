/** @file Main.c
 * Determine which demo to display and display it.
 * @author Adrien RICCIARDI
 */
#include <string.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** How many demos are available. */
#define DEMOS_COUNT (sizeof(Demo_Functions) / sizeof(Demo_Functions[0]))

/** The maximum reachable height in pixels for a falling snow column. */
#define DEMO_FALLING_SNOW_MAXIMUM_COLUMN_HEIGHT 48

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
static void DemoFillScreenSquarePattern(void);
static void DemoFallingSnow(void);

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
	DemoFillScreenRightToLeft,
	DemoFillScreenSquarePattern,
	DemoFallingSnow
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

/** Fill the screen following a square path. */
static void DemoFillScreenSquarePattern(void)
{
	unsigned char Current_X, Current_Y, Starting_X = 0, Starting_Y = 0, Ending_X = SYSTEM_DISPLAY_WIDTH - 1, Ending_Y = SYSTEM_DISPLAY_HEIGHT - 1;
	unsigned short Remaining_Pixels = SYSTEM_DISPLAY_WIDTH * SYSTEM_DISPLAY_HEIGHT;
	
	do
	{
		// Draw left to right row
		Current_Y = Starting_Y;
		for (Current_X = Starting_X; Current_X <= Ending_X; Current_X++)
		{
			SystemDisplaySetPixelState(Current_X, Current_Y, 1);
			SystemDisplayUpdate();
			Remaining_Pixels--;
			
			// Exit if user pressed a key
			if (SystemKeyboardIsKeyAvailable())
			{
				Last_Pressed_Key = SystemKeyboardReadCharacter();
				return;
			}
		}
		// Upper row has been completely filled
		Starting_Y++;
		
		// Draw up to down column
		Current_X = Ending_X;
		for (Current_Y = Starting_Y; Current_Y <= Ending_Y; Current_Y++)
		{
			SystemDisplaySetPixelState(Current_X, Current_Y, 1);
			SystemDisplayUpdate();
			Remaining_Pixels--;
			
			// Exit if user pressed a key
			if (SystemKeyboardIsKeyAvailable())
			{
				Last_Pressed_Key = SystemKeyboardReadCharacter();
				return;
			}
		}
		// Right side column has been completely filled
		Ending_X--;
		
		// Draw right to left row
		Current_Y = Ending_Y;
		for (Current_X = Ending_X; (Current_X >= Starting_X) && (Current_X != 255); Current_X--)
		{
			SystemDisplaySetPixelState(Current_X, Current_Y, 1);
			SystemDisplayUpdate();
			Remaining_Pixels--;
			
			// Exit if user pressed a key
			if (SystemKeyboardIsKeyAvailable())
			{
				Last_Pressed_Key = SystemKeyboardReadCharacter();
				return;
			}
		}
		// Lower row has been completely filled
		Ending_Y--;
		
		// Draw down to up column
		Current_X = Starting_X;
		for (Current_Y = Ending_Y; (Current_Y >= Starting_Y) && (Current_Y != 255); Current_Y--)
		{
			SystemDisplaySetPixelState(Current_X, Current_Y, 1);
			SystemDisplayUpdate();
			Remaining_Pixels--;
			
			// Exit if user pressed a key
			if (SystemKeyboardIsKeyAvailable())
			{
				Last_Pressed_Key = SystemKeyboardReadCharacter();
				return;
			}
		}
		// Left side column has been completely filled
		Starting_X++;
	} while (Remaining_Pixels > 0);
}

/** Falling and stacking snow. */
static void DemoFallingSnow(void)
{
	unsigned char X, Y, Stacks_Height[SYSTEM_DISPLAY_WIDTH], Ending_Y; // No need to use "static" keyword to put Stacks_Height array out of stack because PIC compiler will allocate it in standard RAM
	unsigned short Remaining_Snowflakes = SYSTEM_DISPLAY_WIDTH * DEMO_FALLING_SNOW_MAXIMUM_COLUMN_HEIGHT;
	
	// Reset heights
	memset(Stacks_Height, 0, sizeof(Stacks_Height));
	
	while (Remaining_Snowflakes > 0)
	{
		// Select next column to fill (it must not be completely filled)
		do
		{
			X = SystemRandomGetNumber() % SYSTEM_DISPLAY_WIDTH;
		} while (Stacks_Height[X] >= DEMO_FALLING_SNOW_MAXIMUM_COLUMN_HEIGHT);

		// Initialize falling boundaries
		Y = 0;
		Ending_Y = SYSTEM_DISPLAY_HEIGHT - 1 - Stacks_Height[X]; // Compute the number of rows to cross
		
		// Simulate the falling snowflake
		while (1)
		{
			// Show snowflake
			SystemDisplaySetPixelState(X, Y, 1);
			SystemDisplayUpdate();
			__delay_ms(200); // Let enough time for the pixel to be turned on (it takes about 200ms on A7 STN LCD display)
			
			// Stop falling when the top of the column is reached
			if (Y == Ending_Y) break;
			
			// Hide trace
			SystemDisplaySetPixelState(X, Y, 0);
			SystemDisplayUpdate();
			
			// Go to lower row
			Y++;
			
			// Exit if user pressed a key
			if (SystemKeyboardIsKeyAvailable())
			{
				Last_Pressed_Key = SystemKeyboardReadCharacter();
				return;
			}
		}
		
		// Update column height
		Stacks_Height[X]++;
		
		Remaining_Snowflakes--;
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
