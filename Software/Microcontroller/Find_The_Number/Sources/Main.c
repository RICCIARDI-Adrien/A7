/** @file Main.c
 * A simple game using dichotomy to find an unknown number.
 * @author Adrien RICCIARDI
 */
#include <stdlib.h>
#include <string.h>
#include <Strings.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** How many digits are allowed for the number read from the keyboard. */
#define NUMBER_MAXIMUM_DIGITS_COUNT 9 // Maximum amount of digits that can be stored on a 4-byte integer.

/** How many times the player can try to win. */
#define PLAYER_MAXIMUM_ATTEMPTS_COUNT 28 // A computer would need log2(1000000000) = 29.9 attempts to win, make it harder for a human

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Generate the number the player will have to find.
 * @param String_Number On output, contain the number converted to string. Make sure the buffer is at least NUMBER_MAXIMUM_DIGITS_COUNT + 1 bytes large.
 */
static inline void MainGenerateRandomNumber(unsigned char *String_Number)
{
	unsigned char i, Random_Digit;
	
	for (i = 0; i < NUMBER_MAXIMUM_DIGITS_COUNT; i++)
	{
		Random_Digit = SystemRandomGetNumber() % 10;
		String_Number[i] = Random_Digit + 48; // Convert number to ASCII representation
	}
	// Terminate string
	String_Number[NUMBER_MAXIMUM_DIGITS_COUNT] = 0;
}

/** Read a number from the keyboard. Only digits, enter and backspace keys are accepted.
 * @param String_Number On output, contain the read number converted to a string. Make sure the provided buffer has room for NUMBER_MAXIMUM_DIGITS_COUNT + 1 bytes. The number is aligned on the right and filled with ASCII '0' on the left to allow comparison with the number to find string.
 */
static inline void MainReadPlayerNumber(unsigned char *String_Number)
{
	unsigned char Character, i = 0, String_Unaligned_Number[NUMBER_MAXIMUM_DIGITS_COUNT + 1], j;
	
	// Display the user prompt
	SystemDisplayRenderTextString(STRINGS_INSERT_PLAYER_NUMBER);
	SystemDisplayRenderFrameBuffer();
	
	while (1)
	{
		Character = SystemKeyboardReadCharacter();
		
		switch (Character)
		{
			// Backspace
			case '\b':
				// There must be at least one character to remove
				if (i == 0) break;

				// Remove the character from the string
				String_Unaligned_Number[i] = 0;
				i--;
				
				// Display the backspace to erase the current line last character
				SystemDisplayRenderTextCharacter('\b');
				SystemDisplayRenderFrameBuffer();
				break;
				
			// New line
			case '\n':
				// Forbid empty string
				if (i == 0) break;
				
				// Add terminating zero
				String_Unaligned_Number[i] = 0; // 'i' contains the string length
				
				// Display the new line character
				SystemDisplayRenderTextCharacter('\n');
				SystemDisplayRenderFrameBuffer();
				
				// Shift the string to the right and fill leftmost empty digits with ASCII '0' to be able to compare the string with the random number one
				// Fill number beginning with zeroes
				for (j = 0; j < NUMBER_MAXIMUM_DIGITS_COUNT - i; j++)
				{
					*String_Number = '0';
					String_Number++;
				}
				// Copy number at the string end
				for (j = 0; j < i; j++)
				{
					*String_Number = String_Unaligned_Number[j];
					String_Number++;
				}
				// Terminate string
				*String_Number = 0;
					
				return;
				
			// Escape
			case SYSTEM_KEYBOARD_KEY_CODE_ESCAPE:
				SystemExitProgram();
			
			default:
				// Only digits are allowed
				if ((Character < '0') || (Character > '9')) break;
				
				// Make sure the maximum number size is not crossed
				if (i >= NUMBER_MAXIMUM_DIGITS_COUNT) break;
				
				// Append the digit to the string
				String_Unaligned_Number[i] = Character;
				i++;
				
				// Display the digit
				SystemDisplayRenderTextCharacter(Character);
				SystemDisplayRenderFrameBuffer();
				break;
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	unsigned char String_Number_To_Find[NUMBER_MAXIMUM_DIGITS_COUNT + 1], String_Player_Number[NUMBER_MAXIMUM_DIGITS_COUNT + 1], Remaining_Attempts = PLAYER_MAXIMUM_ATTEMPTS_COUNT, i;
	signed char Result;
	
	SystemInitialize();
	
	while (1)
	{
		// Generate the number to find
		SystemRandomInitialize();
		MainGenerateRandomNumber(String_Number_To_Find);
		
		// Clean the display and return cursor to home
		SystemDisplayClearFrameBuffer();
		SystemDisplaySetTextCursor(0, 0);
		
		while (1)
		{
			MainReadPlayerNumber(String_Player_Number);
			Remaining_Attempts--;
			
			// Is the player number the good one ?
			Result = strcmp((const char *) String_Player_Number, (const char *) String_Number_To_Find);
			// Player number is too small
			if (Result < 0) SystemDisplayRenderTextString(STRINGS_NUMBER_TOO_SMALL);
			// Player number is too big
			else if (Result > 0) SystemDisplayRenderTextString(STRINGS_NUMBER_TOO_BIG);
			// Player found the number
			else
			{
				SystemDisplayRenderTextString(STRINGS_PLAYER_WON);
				SystemDisplayRenderFrameBuffer();
				break;
			}
			
			// Display remaining attempts
			SystemDisplayRenderTextString(STRINGS_REMAINING_ATTEMPTS);
			SystemDisplayRenderTextString((const unsigned char *) itoa(String_Player_Number, Remaining_Attempts, 10)); // Recycle String_Player_Number
			SystemDisplayRenderTextCharacter('\n');
			SystemDisplayRenderFrameBuffer();
			
			// Did the player loose ?
			if (Remaining_Attempts == 0) 
			{
				SystemDisplayRenderTextString(STRINGS_PLAYER_LOST);
				SystemDisplayRenderFrameBuffer();
				break;
			}
		}
		
		// Wait for the player choice to start a new game or to quit
		while (1)
		{
			i = SystemKeyboardReadCharacter(); // Recycle 'i' variable
			if (i == '\n') break;
			if (i == SYSTEM_KEYBOARD_KEY_CODE_ESCAPE) SystemExitProgram();
		}
	}
}
