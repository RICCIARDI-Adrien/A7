/** @file System_Flash.h
 * Write to the microcontroller flash program memory.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_FLASH_H
#define H_SYSTEM_FLASH_H

//--------------------------------------------------------------------------------------------------
// Constants
//--------------------------------------------------------------------------------------------------
/** A program memory block size in bytes. */
#define SYSTEM_FLASH_BLOCK_SIZE 64

/** Flash program memory size in blocks. */
#define SYSTEM_FLASH_BLOCKS_COUNT 1024

//--------------------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------------------
/** Erase the specified block and write the provided data.
 * @param Block_Address The block beginning address.
 * @param Pointer_Data_Buffer The data to write. The buffer must be SYSTEM_FLASH_BLOCK_SIZE bytes.
 * @note Nothing is done if the provided block address is out of memory range.
 */
void SystemFlashWriteBlock(unsigned short Block_Address, unsigned char *Pointer_Data_Buffer);

#endif
