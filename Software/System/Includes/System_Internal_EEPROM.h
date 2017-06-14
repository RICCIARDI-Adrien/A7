/** @file System_Internal_EEPROM.h
 * Allow reading and writing the internal 1024 byte EEPROM.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_INTERNAL_EEPROM_H
#define H_SYSTEM_INTERNAL_EEPROM_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** EEPROM size in bytes. */
#define SYSTEM_INTERNAL_EEPROM_SIZE 1024

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Read a byte from the internal EEPROM.
 * @param Address Where to read the byte.
 * @return The read byte.
 * @note Function does nothing if the provided address is invalid (returned value is 0).
 */
unsigned char SystemInternalEEPROMReadByte(unsigned short Address);

/** Write a byte to the internal EEPROM.
 * @param Address Where to write the byte.
 * @param Byte The data to write.
 * @note Function does nothing if the provided address is invalid.
 */
void SystemInternalEEPROMWriteByte(unsigned short Address, unsigned char Byte);

#endif
