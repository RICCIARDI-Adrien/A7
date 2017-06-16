/** Hex_Parser.c
 * @see Hex_Parser.h for description.
 * @author Adrien RICCIARDI
 */
#include <Hex_Parser.h>
#include <stdio.h>
#include <string.h>

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Replace Debug() with printf() to display debug messages. */
#define Debug(...)

/** The maximum amount of data a record can contain. */
#define HEX_PARSER_RECORD_MAXIMUM_DATA_SIZE 255

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** All the existing record types. */
typedef enum
{
	HEX_PARSER_RECORD_TYPE_DATA,
	HEX_PARSER_RECORD_TYPE_END_OF_FILE,
	HEX_PARSER_RECORD_TYPE_EXTENDED_SEGMENT_ADDRESS,
	HEX_PARSER_RECORD_TYPE_START_SEGMENT_ADDRESS,
	HEX_PARSER_RECORD_TYPE_EXTENDED_LINEAR_ADDRESS,
	HEX_PARSER_RECORD_TYPE_START_LINEAR_ADDRESS,
	HEX_PARSER_RECORD_TYPE_UNKNOWN
} THexParserRecordType;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Convert an hexadecimal character (a nibble) to its binary value.
 * @return The character binary value.
 */
static inline unsigned char HexParserConvertHexadecimalCharacterToNibble(char Character)
{
	// Cast the character in upper case if needed
	if ((Character >= 'a') && (Character <= 'f')) Character -= 32;
	
	// Convert the character
	if (Character <= '9') Character -= '0'; // The character is a digit
	else Character = Character - 'A' + 10; // The character is a letter

	return Character;
}

/** Convert an hexadecimal byte represented by two characters into the corresponding binary value.
 * @param First_Character The number's high nibble.
 * @param Second_Character The number's low nibble.
 * @return The corresponding binary value.
 */
static unsigned char HexParserConvertHexadecimalNumberToByte(char First_Character, char Second_Character)
{
	unsigned char Value;
	
	Value = (HexParserConvertHexadecimalCharacterToNibble(First_Character) << 4) & 0xF0;
	Value |= HexParserConvertHexadecimalCharacterToNibble(Second_Character) & 0x0F;
	return Value;
}

/** Parse a line from the Hex file.
 * @param String_Hexadecimal_Line A line read from the hex file.
 * @param Pointer_Data_Size On output, contain the data size.
 * @param Pointer_Load_Offset On output, contain the data load offset.
 * @param Pointer_Data On output, contain the record data converted to binary.
 * @return The record type.
 */
static THexParserRecordType HexParserParseLine(char *String_Hexadecimal_Line, int *Pointer_Data_Size, int *Pointer_Load_Offset, unsigned char *Pointer_Data)
{
	THexParserRecordType Record_Type;
	int i;
	
	Debug("[%s] Parsing line %s", __func__, String_Hexadecimal_Line); // No new line at the end of this string as the read line adds it
	
	// Bypass the record mark (':')
	String_Hexadecimal_Line++;
	
	// Extract the record size
	*Pointer_Data_Size = HexParserConvertHexadecimalNumberToByte(String_Hexadecimal_Line[0], String_Hexadecimal_Line[1]);
	String_Hexadecimal_Line += 2;
	Debug("[%s] Record data size = %d\n", __func__, *Pointer_Data_Size);
	
	// Extract the load offset
	*Pointer_Load_Offset = HexParserConvertHexadecimalNumberToByte(String_Hexadecimal_Line[0], String_Hexadecimal_Line[1]) << 8;
	*Pointer_Load_Offset |= HexParserConvertHexadecimalNumberToByte(String_Hexadecimal_Line[2], String_Hexadecimal_Line[3]);
	String_Hexadecimal_Line += 4;
	Debug("[%s] Record load offset = 0x%04X\n", __func__, *Pointer_Load_Offset);
	
	// Extract the record type
	Record_Type = HexParserConvertHexadecimalNumberToByte(String_Hexadecimal_Line[0], String_Hexadecimal_Line[1]);
	String_Hexadecimal_Line += 2;
	Debug("[%s] Record type = %d\n", __func__, Record_Type);
	
	// Convert the data if there are provided
	Debug("[%s] Data : ", __func__);
	for (i = 0; i < *Pointer_Data_Size; i++)
	{
		*Pointer_Data = HexParserConvertHexadecimalNumberToByte(String_Hexadecimal_Line[0], String_Hexadecimal_Line[1]);
		Debug("0x%02X ", *Pointer_Data);
		Pointer_Data++;
		String_Hexadecimal_Line += 2;
	}
	Debug("\n");
	
	return Record_Type;	
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int HexParserConvertHexToBinary(char *String_Hex_File, int Microcontroller_Memory_Size, unsigned char *Pointer_Microcontroller_Memory)
{
	FILE *File_Hex;
	THexParserRecordType Record_Type;
	char String_Record_Line[HEX_PARSER_RECORD_MAXIMUM_DATA_SIZE * 2 + 10]; // Can contain a full record represented by hexadecimal characters
	int Current_Line_Number = 0, Data_Size, Load_Offset, Current_Address, Highest_Address = 0, Return_Value = -1;
	unsigned char Data[HEX_PARSER_RECORD_MAXIMUM_DATA_SIZE];
	unsigned short Current_Address_High_Word = 0; // Set by an Extended Linear Address Record
	
	// Try to open the Hex file
	File_Hex = fopen(String_Hex_File, "rb");
	if (File_Hex == NULL)
	{
		printf("Error : could not open the Hex file.\n");
		return -1;
	}
	
	// Initialize the memory representation with the erased flash value
	memset(Pointer_Microcontroller_Memory, 0xFF, Microcontroller_Memory_Size);
	
	// Process all records
	while (fgets(String_Record_Line, sizeof(String_Record_Line), File_Hex) != NULL)
	{
		Current_Line_Number++;
		Debug("[%s] Processing line %d\n", __func__, Current_Line_Number);
		
		// Get the record type
		Record_Type = HexParserParseLine(String_Record_Line, &Data_Size, &Load_Offset, Data);
		switch (Record_Type)
		{
			case HEX_PARSER_RECORD_TYPE_DATA:
				Debug("[%s] Record type : data\n", __func__);
				
				// Compute the instruction address
				Current_Address = (Current_Address_High_Word << 16) | Load_Offset;
				Debug("[%s] Instruction address : 0x%08X.\n", __func__, Current_Address);
				
				// Put the instruction at the right memory place only if the instruction is in the firmware space (not the configuration registers location)
				if (Current_Address < Microcontroller_Memory_Size) memcpy(&Pointer_Microcontroller_Memory[Current_Address], Data, Data_Size);
				
				// Find the highest address used in the program, so we can omit the remaining memory and shorten the output file (in order to shorten the flashing time)
				Data_Size += Current_Address; // Compute the last address of this record into Data_Size (recycling the variable)
				if ((Current_Address < Microcontroller_Memory_Size) && (Data_Size > Highest_Address)) Highest_Address = Data_Size;
				break;
				
			case HEX_PARSER_RECORD_TYPE_END_OF_FILE:
				Debug("[%s] Record type : end of file\n", __func__);
				break;

			// Allow to define the upper bits of a 32-bit Linear Base Address
			case HEX_PARSER_RECORD_TYPE_EXTENDED_LINEAR_ADDRESS:
				Debug("[%s] Record type : extended linear address\n", __func__);

				Current_Address_High_Word = (Data[0] << 8) | Data[1];
				Debug("[%s] Current address changed to 0x%08X.\n", __func__, Current_Address_High_Word << 16);
				break;

			default:
				printf("The line %d contains an unrecognized record (0x%02X). Aborting conversion.\n", Current_Line_Number, Record_Type);
				goto Exit;
		}
		Debug("\n");
	}
	
	// Return the firmware size
	Return_Value = Highest_Address;
	
Exit:
	fclose(File_Hex);
	return Return_Value;
}
