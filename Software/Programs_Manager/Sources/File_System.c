/** @file File_System.c
 * @see File_System.h for description.
 * @author Adrien RICCIARDI
 */
#include <File_System.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
TFileSystemFileInformation File_System_Files_Information[FILE_SYSTEM_MAXIMUM_FILES_COUNT];

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Read a file information (file name and size).
 * @param File_ID The file identifier (in range [0; FILE_SYSTEM_MAXIMUM_FILES_COUNT[).
 * @param String_Name On output, contain the file name. Make sure the provided buffer is large enough (at least FILE_SYSTEM_FILE_NAME_SIZE + 1).
 * @param Pointer_File_Size On output, contain the file size in bytes.
 * @note Function returns an empty file name and a zero-byte size if the provided file ID is invalid.
 */
static inline void FileSystemReadFileInformation(unsigned char File_ID, unsigned char *String_Name, unsigned short *Pointer_File_Size)
{
	unsigned short Internal_EEPROM_Address;
	unsigned char i, Byte;
	
	// Make sure the requested file is existing
	if (File_ID >= FILE_SYSTEM_MAXIMUM_FILES_COUNT)
	{
		// Return empty values
		String_Name[0] = 0;
		*Pointer_File_Size = 0;
		return;
	}
	
	// Compute the file information base address
	Internal_EEPROM_Address = File_ID * (FILE_SYSTEM_FILE_NAME_SIZE + sizeof(unsigned short));
	
	// Read the file name
	for (i = 0; i < FILE_SYSTEM_FILE_NAME_SIZE; i++)
	{
		*String_Name = SystemInternalEEPROMReadByte(Internal_EEPROM_Address);
		
		// Do not continue if the string is terminated
		if (*String_Name == 0) break;
		
		Internal_EEPROM_Address++;
		String_Name++;
	}
	// Add a terminating zero (useful only for maximum-length strings
	*String_Name = 0;
	
	// Read the file size
	// Least significant byte
	Byte = SystemInternalEEPROMReadByte(Internal_EEPROM_Address);
	Internal_EEPROM_Address++;
	*Pointer_File_Size = Byte;
	// Most significant byte
	Byte = SystemInternalEEPROMReadByte(Internal_EEPROM_Address);
	*Pointer_File_Size |= (Byte << 8) & 0xFF00;	
}

/** Write a file information.
 * @param File_ID The file identifier (in range [0; FILE_SYSTEM_MAXIMUM_FILES_COUNT[).
 * @param String_Name The file name (only the first FILE_SYSTEM_FILE_NAME_SIZE bytes are written).
 * @param File_Size File size in bytes.
 * @note Function does nothing if the provided file ID is invalid.
 */
static inline void FileSystemWriteFileInformation(unsigned char File_ID, unsigned char *String_Name, unsigned short File_Size)
{
	unsigned short Internal_EEPROM_Address;
	unsigned char i;
	
	// Make sure the requested file is existing
	if (File_ID >= FILE_SYSTEM_MAXIMUM_FILES_COUNT) return;
	
	// Compute the file information base address
	Internal_EEPROM_Address = File_ID * (FILE_SYSTEM_FILE_NAME_SIZE + sizeof(unsigned short));
	
	// Write the file name
	for (i = 0; i < FILE_SYSTEM_FILE_NAME_SIZE; i++)
	{
		SystemInternalEEPROMWriteByte(Internal_EEPROM_Address, *String_Name);
		
		// Do not continue if the string is terminated
		if (*String_Name == 0) break;
		
		Internal_EEPROM_Address++;
		String_Name++;
	}
	
	// Write the file size
	// Least significant byte
	SystemInternalEEPROMWriteByte(Internal_EEPROM_Address, (unsigned char) File_Size);
	Internal_EEPROM_Address++;
	// Most significant byte
	SystemInternalEEPROMWriteByte(Internal_EEPROM_Address, File_Size >> 8);
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void FileSystemLoad(void)
{
	unsigned char i;
	
	// TEST
	FileSystemWriteFileInformation(0, "FICH 1", 12);
	FileSystemWriteFileInformation(1, "UN AUTRE NOM", 12);
	FileSystemWriteFileInformation(2, "01234567890123456789", 12);
	FileSystemWriteFileInformation(3, "", 12);
	
	for (i = 0; i < FILE_SYSTEM_MAXIMUM_FILES_COUNT; i++) FileSystemReadFileInformation(i, (unsigned char *) File_System_Files_Information[i].String_Name, &File_System_Files_Information[i].Size);
}
