/** @file Menu_Start_Program.c
 * @see Menu.h for description.
 * @author Adrien RICCIARDI
 */
#include <File_System.h>
#include <Menu.h>
#include <System.h>
#include <String.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void MenuStartProgram(void)
{
	unsigned char File_ID, External_EEPROM_Buffer[SYSTEM_EXTERNAL_EEPROM_PAGE_SIZE], j, *Pointer_Data_To_Write;
	unsigned short External_EEPROM_Page_Address, i, File_Size;
	unsigned long Flash_Block_Address = CONFIGURATION_APPLICATION_BASE_ADDRESS / SYSTEM_FLASH_BLOCK_SIZE;
	
	// Ask for the program to start
	MenuShowFiles(STRING_MENU_START_PROGRAM_TITLE);
	while (1)
	{
		File_ID = SystemKeyboardReadCharacterNoInterrupt();
		
		// Handle menu items choice
		if ((File_ID >= '1') && (File_ID <= '4'))
		{
			// Extract EEPROM bank from the selected menu
			File_ID -= '0'; // Convert from ASCII to binary
			File_ID--; // EEPROM banks start from 0, not from 1
			
			// Make sure the selected external EEPROM slot is not empty
			if (File_System_Files_Information[File_ID].String_Name[0] != 0xFF)
			{
				// Compute the EEPROM starting page
				External_EEPROM_Page_Address = (File_ID * FILE_SYSTEM_MAXIMUM_FILE_SIZE) / SYSTEM_EXTERNAL_EEPROM_PAGE_SIZE;
				break;
			}
		}
		else if (File_ID == SYSTEM_KEYBOARD_KEY_CODE_ESCAPE) return;
	}
	
	// Display the "loading" message
	SystemDisplayClearFrameBuffer();
	SystemDisplaySetTextCursor(0, 0);
	SystemDisplayRenderTextString(STRING_MENU_START_PROGRAM_LOADING);
	SystemDisplayRenderFrameBuffer();
	
	// Load the file from the external EEPROM into the program memory
	File_Size = File_System_Files_Information[File_ID].Size;
	for (i = 0; i < File_Size; i += SYSTEM_EXTERNAL_EEPROM_PAGE_SIZE)
	{
		// Load a whole EEPROM page
		SystemExternalEEPROMReadPage(External_EEPROM_Page_Address, External_EEPROM_Buffer);
		
		// Write it to flash memory
		Pointer_Data_To_Write = External_EEPROM_Buffer;
		for (j = 0; j < SYSTEM_EXTERNAL_EEPROM_PAGE_SIZE / SYSTEM_FLASH_BLOCK_SIZE; j++)
		{
			SystemFlashWriteBlock(Flash_Block_Address, Pointer_Data_To_Write);
			Flash_Block_Address++;
			Pointer_Data_To_Write += SYSTEM_FLASH_BLOCK_SIZE;
		}
		
		External_EEPROM_Page_Address++;
	}
	
	// Jump to the application entry point
	#asm
		goto CONFIGURATION_APPLICATION_BASE_ADDRESS
	#endasm
}