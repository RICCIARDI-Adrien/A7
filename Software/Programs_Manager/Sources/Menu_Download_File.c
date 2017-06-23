/** @file Menu_Download_File.c
 * @see Menu.h for description.
 * @author Adrien RICCIARDI
 */
#include <File_System.h>
#include <Menu.h>
#include <String.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** The code sent to initiate a download. */
#define MENU_DOWNLOAD_FILE_PROTOCOL_CODE_START_DOWNLOADING 'S'
/** The code sent to validate a protocol step. */
#define MENU_DOWNLOAD_FILE_PROTOCOL_CODE_ACKNOWLEDGE 'A'

/** How many bytes to send at a time when transfering application data. */
#define MENU_DOWNLOAD_FILE_PROTOCOL_DATA_CHUNK_SIZE SYSTEM_EXTERNAL_EEPROM_PAGE_SIZE

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void MenuDownloadFile(void)
{
	unsigned char Byte = 0, String_File_Name[FILE_SYSTEM_FILE_NAME_SIZE], i, File_ID, External_EEPROM_Buffer[MENU_DOWNLOAD_FILE_PROTOCOL_DATA_CHUNK_SIZE];
	unsigned short File_Size, External_EEPROM_Page_Address, Bytes_To_Receive_Count, j;
	TFileSystemFileInformation *Pointer_File_Information;
	
	// Ask for the file slot to use to store the downloaded file
	MenuShowFiles(STRING_MENU_DOWNLOAD_FILE_TITLE);
	while (1)
	{
		File_ID = SystemKeyboardReadCharacterNoInterrupt();
		
		// Handle menu items choice
		if ((File_ID >= '1') && (File_ID <= '4'))
		{
			// Extract EEPROM bank from the selected menu
			File_ID -= '0'; // Convert from ASCII to binary
			File_ID--; // EEPROM banks start from 0, not from 1
			
			// Compute the EEPROM starting page
			External_EEPROM_Page_Address = (File_ID * FILE_SYSTEM_MAXIMUM_FILE_SIZE) / SYSTEM_EXTERNAL_EEPROM_PAGE_SIZE;
			break;
		}
		else if (File_ID == SYSTEM_KEYBOARD_KEY_CODE_ESCAPE) return;
	}
	
	SystemDisplayClearFrameBuffer();
	
	// Display the "connecting" message
	SystemDisplaySetTextCursor(0, 0);
	SystemDisplayRenderTextString(STRING_MENU_DOWNLOAD_FILE_CONNECTING);
	// Display the "exit" message on the display bottom
	//SystemDisplaySetTextCursor(0, SYSTEM_DISPLAY_TEXT_CHARACTER_HEIGHT - 1);
	//SystemDisplayRenderTextString(STRING_MENU_EXIT); // TODO : hard to do due to no interrupts on the keyboard
	SystemDisplayRenderFrameBuffer();
	
	// Initiate the connection
	SystemSerialPortWriteByte(MENU_DOWNLOAD_FILE_PROTOCOL_CODE_START_DOWNLOADING);
	// Wait for the server clearance
	while (SystemSerialPortReadByte() != MENU_DOWNLOAD_FILE_PROTOCOL_CODE_ACKNOWLEDGE);
	
	// Display the "downloading" message
	SystemDisplaySetTextCursor(0, 1);
	SystemDisplayRenderTextString(STRING_MENU_DOWNLOAD_FILE_DOWNLOADING);
	SystemDisplayRenderFrameBuffer();
	
	// Receive the file name
	for (i = 0; i < FILE_SYSTEM_FILE_NAME_SIZE; i++) String_File_Name[i] = SystemSerialPortReadByte();
	
	// Receive the file size in bytes
	File_Size = (SystemSerialPortReadByte() << 8) | SystemSerialPortReadByte();
	// Adjust file size if it crosses file slot bound
	if (File_Size > FILE_SYSTEM_MAXIMUM_FILE_SIZE) File_Size = FILE_SYSTEM_MAXIMUM_FILE_SIZE;
	
	// Receive the file data chunk by chunk
	while (File_Size > 0)
	{
		// Compute how many bytes to receive
		if (File_Size > MENU_DOWNLOAD_FILE_PROTOCOL_DATA_CHUNK_SIZE) Bytes_To_Receive_Count = MENU_DOWNLOAD_FILE_PROTOCOL_DATA_CHUNK_SIZE;
		else Bytes_To_Receive_Count = File_Size;
		
		// Receive data
		for (j = 0; j < Bytes_To_Receive_Count; j++) External_EEPROM_Buffer[i] = SystemSerialPortReadByte();
		
		// Write data to external EEPROM
		SystemExternalEEPROMWritePage(External_EEPROM_Page_Address, External_EEPROM_Buffer);
		
		// Prepare for next chunk
		File_Size -= Bytes_To_Receive_Count;
		External_EEPROM_Page_Address++;
		
		// Send an acknowledge to tell that data has been successfully written
		SystemSerialPortWriteByte(MENU_DOWNLOAD_FILE_PROTOCOL_CODE_ACKNOWLEDGE);
	}
	
	// Update file system only at the end to be sure that all data were successfully downloaded
	Pointer_File_Information = &File_System_Files_Information[File_ID];
	for (i = 0; i < FILE_SYSTEM_FILE_NAME_SIZE; i++) Pointer_File_Information->String_Name[i] = String_File_Name[i];
	Pointer_File_Information->Size = File_Size;
	FileSystemStore();
}
