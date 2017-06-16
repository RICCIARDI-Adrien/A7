/** @file File_System.h
 * Files are stored on two memories : internal EEPROM holds file informations (name, size), external EEPROM contains file data.
 * @author Adrien RICCIARDI
 */
#ifndef H_FILE_SYSTEM_H
#define H_FILE_SYSTEM_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How many files are stored on the external EEPROM. */
#define FILE_SYSTEM_MAXIMUM_FILES_COUNT 4
/** Maximum file size in bytes (this is the size of an external EEPROM file "slot"). */
#define FILE_SYSTEM_MAXIMUM_FILE_SIZE 32768

/** File name string size in bytes. */
#define FILE_SYSTEM_FILE_NAME_SIZE 18

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** A file information. */
typedef __pack struct
{
	unsigned char String_Name[FILE_SYSTEM_FILE_NAME_SIZE]; //<! File name. If the file is FILE_SYSTEM_FILE_NAME_SIZE long then the string terminating zero must be removed. If the first byte is 0xFF (like EEPROM erased value), consider the file as empty.
	unsigned short Size; //<! File size in bytes.
} TFileSystemFileInformation;

//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------
/** Cache file system information. */
extern TFileSystemFileInformation File_System_Files_Information[FILE_SYSTEM_MAXIMUM_FILES_COUNT];

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Load files information from the internal EEPROM and store them in the File_System_Files_Information public variable. */
void FileSystemLoad(void);

/** Save the current file system information on the internal EEPROM. */
void FileSystemStore(void);

#endif
