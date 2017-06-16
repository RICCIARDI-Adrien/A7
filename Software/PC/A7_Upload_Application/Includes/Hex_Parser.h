/** @file Hex_Parser.h
 * Extract useful values from an Intel Hex file record.
 * @author Adrien RICCIARDI
 * @note Only data and end-of-file records are supported.
 */
#ifndef H_HEX_PARSER_H
#define H_HEX_PARSER_H

//-------------------------------------------------------------------------------------------------
// Function
//-------------------------------------------------------------------------------------------------
/** Parse the hex file to put all instructions in a microcontroller memory representation.
 * @param String_Hex_File The firmware hex file.
 * @param Microcontroller_Memory_Size The microcontroller program memory size in bytes. 
 * @param Pointer_Microcontroller_Memory On output, contain the firmware binary instructions as they would be in the microcontroller memory. Make sure that this buffer is at least Microcontroller_Memory_Size large.
 * @return The firmware size in bytes in case of success,
 * @return -1 if an error occurred.
 */
int HexParserConvertHexToBinary(char *String_Hex_File, int Microcontroller_Memory_Size, unsigned char *Pointer_Microcontroller_Memory);

#endif
