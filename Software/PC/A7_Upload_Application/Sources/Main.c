/** @file Main.c
 * Connect to the A7 (assuming it is waiting for a file to download) and send an Intel Hex file content converted to binary.
 * @author Adrien RICCIARDI
 */
#include <Hex_Parser.h>
#include <Serial_Port.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Allowed application size in bytes. */
#define MAIN_APPLICATION_MAXIMUM_SIZE 32768
/** Application base address in program memory (application code is relocated after the Programs Manager). */
#define MAIN_APPLICATION_BASE_ADDRESS 0x2000

/** Maximum amount of characters in an A7 file name. */
#define MAIN_A7_FILE_NAME_MAXIMUM_CHARACTERS_COUNT 18

/** The code sent to initiate a download. */
#define MAIN_DOWNLOAD_PROTOCOL_CODE_START_DOWNLOADING 'S'
/** The code sent to validate a protocol step. */
#define MAIN_DOWNLOAD_PROTOCOL_CODE_ACKNOWLEDGE 'A'

/** How many bytes to send at a time when transfering application data. */
#define MAIN_DOWNLOAD_PROTOCOL_DATA_CHUNK_SIZE 256

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Hold the microcontroller program memory content. */
static unsigned char Main_Microcontroller_Program_Memory[MAIN_APPLICATION_MAXIMUM_SIZE];

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Send the converted application to the A7.
 * @param String_Serial_Port_Device The serial port connected to the A7.
 * @param Application_Size How many bytes to send.
 * @param String_A7_File_Name The file the will have on the A7.
 * @return 0 if the application was successfully sent,
 * @return -1 if an error occurred.
 */
static int MainSendApplication(char *String_Serial_Port_Device, int Application_Size, char *String_A7_File_Name)
{
	TSerialPortID Serial_Port_ID;
	int Return_Value = -1, i = MAIN_A7_FILE_NAME_MAXIMUM_CHARACTERS_COUNT, Bytes_To_Send_Count;
	unsigned char *Pointer_Data_To_Send = (unsigned char *) (Main_Microcontroller_Program_Memory + MAIN_APPLICATION_BASE_ADDRESS);
	
	// Try to connect to the serial port
	if (SerialPortOpen(String_Serial_Port_Device, 115200, &Serial_Port_ID) != 0)
	{
		printf("Error : failed to initialize serial port.\n");
		goto Exit;
	}
	
	// Try to establish connection
	printf("Waiting for the A7 to connect...\n");
	// Wait for the A7 to initiate the connection
	while (SerialPortReadByte(Serial_Port_ID) != MAIN_DOWNLOAD_PROTOCOL_CODE_START_DOWNLOADING);
	// Tell the A7 that the connection is established
	SerialPortWriteByte(Serial_Port_ID, MAIN_DOWNLOAD_PROTOCOL_CODE_ACKNOWLEDGE);
	printf("Connection established.\n");
	
	// Wait a bit because the A7 displays another message, and display access is slow
	usleep(200000); // TODO determine if really useful on real circuit
	
	// Send the file name (send the string up to the maximum amount of characters, or up to the string terminating zero)
	while ((i > 0) && (*String_A7_File_Name != 0))
	{
		SerialPortWriteByte(Serial_Port_ID, *String_A7_File_Name);
		i--;
		String_A7_File_Name++;
	}
	// Send extra bytes if the name is smaller than MAIN_A7_FILE_NAME_MAXIMUM_CHARACTERS_COUNT characters (A7 expects exactly MAIN_A7_FILE_NAME_MAXIMUM_CHARACTERS_COUNT characters)
	while (i > 0)
	{
		SerialPortWriteByte(Serial_Port_ID, 0);
		i--;
	}
	
	// Send application size
	SerialPortWriteByte(Serial_Port_ID, Application_Size >> 8);
	SerialPortWriteByte(Serial_Port_ID, (unsigned char) Application_Size);
	
	// Send the application data chunk by chunk
	printf("Sending application data...\n");
	while (Application_Size > 0)
	{
		// Compute how many bytes to send
		if (Application_Size > MAIN_DOWNLOAD_PROTOCOL_DATA_CHUNK_SIZE) Bytes_To_Send_Count = MAIN_DOWNLOAD_PROTOCOL_DATA_CHUNK_SIZE;
		else Bytes_To_Send_Count = Application_Size;
		
		// Send data
		SerialPortWriteBuffer(Serial_Port_ID, Pointer_Data_To_Send, Bytes_To_Send_Count);
		
		// Wait for an acknowledge to let A7 enough time to write data to external EEPROM
		while (SerialPortReadByte(Serial_Port_ID) != MAIN_DOWNLOAD_PROTOCOL_CODE_ACKNOWLEDGE);
		
		Application_Size -= Bytes_To_Send_Count;
		Pointer_Data_To_Send += Bytes_To_Send_Count;
	}
	
	Return_Value = 0;

Exit:
	SerialPortClose(Serial_Port_ID);
	return Return_Value;
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	char *String_Serial_Port_Device, *String_Hex_File, *String_A7_File_Name;
	int Application_Size;
	unsigned int A7_File_Name_Length, i;
	
	// Check parameters
	if (argc != 4)
	{
		printf("Usage : %s Serial_Port_Device Hex_File_To_Send File_Name_On_A7\n"
			" - Serial_Port_Device : the serial port connected to the A7, like /dev/ttyS0 or /dev/ttyUSB0.\n"
			" - Hex_File_To_Send : path to the Intel Hex file to send.\n"
			" - File_Name_On_A7 : the slot in which the file will be saved will be named like this.\n", argv[0]);
		
		return EXIT_FAILURE;
	}
	// Retrieve parameters
	String_Serial_Port_Device = argv[1];
	String_Hex_File = argv[2];
	String_A7_File_Name = argv[3];
	
	// Convert hex file to binary
	printf("Converting Hex file in A7 format...\n");
	Application_Size = HexParserConvertHexToBinary(String_Hex_File, MAIN_APPLICATION_MAXIMUM_SIZE, MAIN_APPLICATION_BASE_ADDRESS, Main_Microcontroller_Program_Memory);
	if (Application_Size == -1)
	{
		printf("Error : failed to convert the provided Hex file (make sure the code does not exceeds the allowed %d bytes of program memory).\n", MAIN_APPLICATION_MAXIMUM_SIZE);
		return EXIT_FAILURE;
	}
	printf("Conversion successful (application size : %d bytes / %d maximum allowed bytes).\n", Application_Size, MAIN_APPLICATION_MAXIMUM_SIZE);
	
	// Do some sanity check on the file name that will be sent to the A7
	// Name can't be too long
	A7_File_Name_Length = strlen(String_A7_File_Name);
	if (A7_File_Name_Length > MAIN_A7_FILE_NAME_MAXIMUM_CHARACTERS_COUNT)
	{
		printf("Error : provided A7 file name is too long (maximum allowed file name length is %d characters).\n", MAIN_A7_FILE_NAME_MAXIMUM_CHARACTERS_COUNT);
		return EXIT_FAILURE;
	}
	// Only first page standard ASCII characters are allowed
	for (i = 0; i < A7_File_Name_Length; i++)
	{
		// First ASCII page holds characters from code 0 to code 127, so check if characters are higher than 127
		if (String_A7_File_Name[i] < 0) // As characters are signed and 127 is the greatest positive value, checking if character value is greater than 127 results in searching if it is negative (because "unsigned char" 128 equals "signed char" -1)
		{
			printf("Error : only ASCII characters with codes in range [0..127] are allowed.\n");
			return EXIT_FAILURE;
		}
	}
	
	// Transfer application
	if (MainSendApplication(String_Serial_Port_Device, Application_Size, String_A7_File_Name) != 0)
	{
		printf("Error : failed to send the application to the A7.\n");
		return EXIT_FAILURE;
	}

	printf("Application successfully sent.\n");

	return EXIT_SUCCESS;
}
