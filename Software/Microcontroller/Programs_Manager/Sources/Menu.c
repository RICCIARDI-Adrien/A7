/** @file Menu.c
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
void MenuShowFiles(const unsigned char *String_Menu_Title)
{
	unsigned char i;
	
	SystemDisplayClear();
	
	// Display title
	SystemDisplayRenderTextString((const unsigned char *) String_Menu_Title);
	
	// Display available programs
	for (i = 0; i < FILE_SYSTEM_MAXIMUM_FILES_COUNT; i++)
	{
		SystemDisplaySetTextCursor(0, i + 2);
		
		// Display the menu preceding number
		SystemDisplayRenderTextCharacter(i + 1 + '0'); // Quick number to ASCII conversion, as there will not be 2-digit numbers
		SystemDisplayRenderTextCharacter('.');
		SystemDisplayRenderTextCharacter(' ');
		
		// Display the file name TODO 18-byte string does not have terminating zero, but is going out display right bound...
		if (File_System_Files_Information[i].String_Name[0] != 0xFF) SystemDisplayRenderTextString((const unsigned char *) File_System_Files_Information[i].String_Name); // Do not display the file name if the EEPROM contains only erased data
	}
	
	// Display the exit command at the display bottom
	SystemDisplaySetTextCursor(0, 7);
	SystemDisplayRenderTextString(STRING_MENU_EXIT);
	
	SystemDisplayUpdate();
}
