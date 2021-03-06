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
/** Display the main menu and handle the keyboard actions. */
static inline void MainMenu(void)
{
	unsigned char Character;
	
Start:
	while (1)
	{
		// Display menu
		SystemDisplayClear();
		// Menu title
		SystemDisplayRenderTextString(STRING_MENU_MAIN_TITLE);
		// Menu items
		SystemDisplaySetTextCursor(0, 2);
		SystemDisplayRenderTextString(STRING_MENU_MAIN_ITEM_1);
		SystemDisplaySetTextCursor(0, 3);
		SystemDisplayRenderTextString(STRING_MENU_MAIN_ITEM_2);
		SystemDisplaySetTextCursor(0, 4);
		SystemDisplayRenderTextString(STRING_MENU_MAIN_ITEM_3);
		// Render to screen
		SystemDisplayUpdate();
		
		// Wait for a menu item to be selected
		while (1)
		{
			Character = SystemKeyboardReadCharacterNoInterrupt();
			
			switch (Character)
			{
				case '1':
					MenuStartProgram();
					goto Start;
					
				case '2':
					MenuDownloadFile();
					goto Start;
		
				case '3':
					MenuDeleteFile();
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
	// Configure low-level hardware and only needed peripherals
	SystemInitializeProgramsManager();
	
	// Load files information from the internal EEPROM
	FileSystemLoad();
	
	// Display the main menu forever
	MainMenu();
}
