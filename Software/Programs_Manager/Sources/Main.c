/** @file Main.c
 * Manage applications stored on the external EEPROM memory.
 * @author Adrien RICCIARDI
 */
#include <File_System.h>
#include <Menu.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** This function is located in the microcontroller reset vector, this is the first executed code. */
static void EntryPoint(void) __at(0)
{
	// Jump to the program entry point, which is located at the end of program memory to let the beginning available to other programs
	#asm // Do not use asm("xxx") form because the C preprocessor does not replace constants in quoted strings
		goto STARTING_ADDRESS
	#endasm
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	unsigned char c;
	
	// Initialize needed peripherals
	SystemInitialize(); // This function automatically enables interrupts
	INTCON = 0; // Immediately disable interrupts as they can't be used in this program because it is relocated at the end of the program memory (so the interrupt vectors are not available)
	
	// Load files information from the internal EEPROM
	FileSystemLoad();
	
	// TEST
	MenuShowFiles("TEST TITLE");
	
	while (1)
	{
		c = SystemKeyboardReadCharacterNoInterrupt();
		SystemSerialPortWriteByte(c);
	}
	
	// Call the entry point function to force it to be compiled, but call it from somewhere that can't be reached to avoid executing it one more time
	EntryPoint();
}
