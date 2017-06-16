/** @file Main.c
 * Manage applications stored on the external EEPROM memory.
 * @author Adrien RICCIARDI
 */
#include <File_System.h>
#include <Menu.h>
#include <String.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** This function is located in the microcontroller reset vector, this is the first executed code. */
static void MainEntryPoint(void) __at(0)
{
	// Jump to the program entry point, which is located at the end of program memory to let the beginning available to other programs
	#asm // Do not use asm("xxx") form because the C preprocessor does not replace constants in quoted strings
		goto STARTING_ADDRESS
	#endasm
}

/** Display the main menu and handle the keyboard actions. */
static inline void MainMenu(void)
{
	unsigned char Character;
	
Start:
	while (1)
	{
		// Display menu
		SystemDisplayClearFrameBuffer();
		// Menu title
		SystemDisplaySetTextCursor(0, 0);
		SystemDisplayRenderTextString(STRING_MENU_MAIN_TITLE);
		// Menu items
		SystemDisplaySetTextCursor(0, 2);
		SystemDisplayRenderTextString(STRING_MENU_MAIN_ITEM_1);
		SystemDisplaySetTextCursor(0, 3);
		SystemDisplayRenderTextString(STRING_MENU_MAIN_ITEM_2);
		SystemDisplaySetTextCursor(0, 4);
		SystemDisplayRenderTextString(STRING_MENU_MAIN_ITEM_3);
		// Render to screen
		SystemDisplayRenderFrameBuffer();
		
		// Wait for a menu item to be selected
		while (1)
		{
			Character = SystemKeyboardReadCharacterNoInterrupt();
			
			switch (Character)
			{
				case '1':
					SystemSerialPortWriteString("menu 1\r\n");
					goto Start;
					
				case '2':
					MenuDownloadFile();
					goto Start;
		
				case '3':
					SystemSerialPortWriteString("menu trouai\r\n");
					goto Start;
					
				default:
					break;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	// Initialize needed peripherals
	SystemInitialize(); // This function automatically enables interrupts
	INTCON = 0; // Immediately disable interrupts as they can't be used in this program because it is relocated at the end of the program memory (so the interrupt vectors are not available)
	
	// Load files information from the internal EEPROM
	FileSystemLoad();
	
	// TEST
	SystemSerialPortWriteString("aaa init string\r\n"); // Sending "aaa" first allows to use UART on RB6 and RB7 pins without being too bored by mikroProg on a Easy PIC 7 MikroElektronika board (value found by chance)
	SystemSerialPortWriteString("This is a test string\r\n");
	
	// Display the main menu forever
	MainMenu();
	
	// Call the entry point function to force it to be compiled, but call it from somewhere that can't be reached to avoid executing it one more time
	MainEntryPoint();
}
