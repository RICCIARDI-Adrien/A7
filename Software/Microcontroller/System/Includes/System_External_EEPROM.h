/** @file System_External_EEPROM.h
 * Access to a 128KB 25LC1024 EEPROM through an SPI bus.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_EXTERNAL_EEPROM_H
#define H_SYSTEM_EXTERNAL_EEPROM_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** A page size in bytes. */
#define SYSTEM_EXTERNAL_EEPROM_PAGE_SIZE 256

/** How many pages the EEPROM provides. */
#define SYSTEM_EXTERNAL_EEPROM_PAGES_COUNT 512

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the SPI module. */
void SystemExternalEEPROMInitialize(void);

/** Read SYSTEM_EXTERNAL_EEPROM_PAGE_SIZE bytes from the specified page.
 * @param Page_Number The page to read, must be in range [0; SYSTEM_EXTERNAL_EEPROM_PAGES_COUNT - 1]. No read operation is done if the provided page number is invalid.
 * @param Pointer_Buffer On output, will contain read data. Buffer must be at least SYSTEM_EXTERNAL_EEPROM_PAGE_SIZE bytes large.
 */
void SystemExternalEEPROMReadPage(unsigned short Page_Number, unsigned char *Pointer_Buffer);

/** Write SYSTEM_EXTERNAL_EEPROM_PAGE_SIZE bytes to the specified page.
 * @param Page_Number The page to write, must be in range [0; SYSTEM_EXTERNAL_EEPROM_PAGES_COUNT - 1]. No write operation is done if the provided page number is invalid.
 * @param Pointer_Buffer Data to write. Buffer must be at least SYSTEM_EXTERNAL_EEPROM_PAGE_SIZE bytes large.
 */
void SystemExternalEEPROMWritePage(unsigned short Page_Number, unsigned char *Pointer_Buffer);

#endif