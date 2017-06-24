/** @file Menu_Delete_File.c
 * @see Menu.h for description.
 * @author Adrien RICCIARDI
 */
#include <File_System.h>
#include <Menu.h>
#include <String.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void MenuDeleteFile(void)
{
	unsigned char File_ID, Character;
	
	// Ask for the file slot to use to store the downloaded file
	MenuShowFiles(STRING_MENU_DELETE_FILE_TITLE);
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
			if (File_System_Files_Information[File_ID].String_Name[0] != 0xFF) break;
		}
		else if (File_ID == SYSTEM_KEYBOARD_KEY_CODE_ESCAPE) return;
	}
	
	// Ask the user for confirmation
	SystemDisplayClearFrameBuffer();
	SystemDisplaySetTextCursor(0, 0);
	SystemDisplayRenderTextString(STRING_MENU_DELETE_FILE_CONFIRMATION_MESSAGE_1);
	SystemDisplaySetTextCursor(0, 1);
	SystemDisplayRenderTextString(STRING_MENU_DELETE_FILE_CONFIRMATION_MESSAGE_2);
	SystemDisplayRenderFrameBuffer();
	while (1)
	{
		Character = SystemKeyboardReadCharacterNoInterrupt();
		
		if (Character == '\n') break;
		if (Character == SYSTEM_KEYBOARD_KEY_CODE_ESCAPE) return;
	}
	
	// Delete the file
	File_System_Files_Information[File_ID].String_Name[0] = 0xFF;
	FileSystemStore();
}
