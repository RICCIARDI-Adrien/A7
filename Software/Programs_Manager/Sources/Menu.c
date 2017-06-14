/** @file Menu.c
 * @see Menu.h for description.
 * @author Adrien RICCIARDI
 */
#include <File_System.h>
#include <Menu.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void MenuShowFiles(unsigned char *String_Menu_Title)
{
	unsigned char i;
	
	SystemDisplayClearFrameBuffer();
	SystemDisplaySetTextCursor(0, 0);
	
	// Display title
	SystemDisplayRenderTextString(String_Menu_Title);
	
	// Display available programs
	for (i = 0; i < FILE_SYSTEM_MAXIMUM_FILES_COUNT; i++)
	{
		SystemDisplaySetTextCursor(0, i + 2);
		
		// Display the menu preceding number
		SystemDisplayRenderTextCharacter(i + 1 + '0'); // Quick number to ASCII conversion, as there will not be 2-digit numbers
		SystemDisplayRenderTextCharacter('.');
		SystemDisplayRenderTextCharacter(' ');
		
		// Display the file name
		SystemDisplayRenderTextString(File_System_Files_Information[i].String_Name);
	}
	
	// TODO display exit text
		
	SystemDisplayRenderFrameBuffer();
}
