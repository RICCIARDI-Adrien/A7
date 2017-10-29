/** @file AES.h
 * AES-256 implementation based on FIPS 197 (http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf).
 * @author Adrien RICCIARDI
 */
#ifndef H_AES_H
#define H_AES_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** An AES block size in bytes. */
#define AES_BLOCK_SIZE 16

/** AES-256 key size in bytes. */
#define AES_256_KEY_SIZE 32

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the AES-256 CTR context. This function must be called once to start cipher or decipher operation.
 * @param Pointer_Key The cipher key (must be AES_BLOCK_SIZE bytes long).
 * @note This function initializes the internal counter to 1.
 */
void AES256CTRInitialize(unsigned char *Pointer_Key);

/** Cipher or decipher data using Counter mode.
 * @param Pointer_Input_Buffer The data to process (buffer must be AES_BLOCK_SIZE bytes long).
 * @param Pointer_Output_Buffer On output, contain the processed data (buffer must be be AES_BLOCK_SIZE bytes long).
 * @warning AES256CTRInitialize() must have been called once before using this function, or the result will be unpredictable.
 */
void AES256CTRUpdate(unsigned char *Pointer_Input_Buffer, unsigned char *Pointer_Output_Buffer);

/** Initialize the AES-256 CBC context. This function must be called once to start cipher or decipher operation.
 * @param Pointer_Key The cipher key (must be AES_BLOCK_SIZE bytes long).
 * @note There is no need to provide an initialization vector because Explicit Initialization Vector mode is used (first ciphered block is auto-generated then discarded).
 */
void AES256CBCInitialize(unsigned char *Pointer_Key);

/** Cipher or decipher data using Cipher Block Chaining mode.
 * @param Pointer_Buffer The data to process (buffer must be AES_BLOCK_SIZE bytes long). On output, contain the processed data.
 * @warning AES256CBCInitialize() must have been called once before using this function, or the result will be unpredictable.
 * @note Provide a dummy initial block because it will be used only to generate the chaining "seed", it's data will be lost.
 */
void AES256CBCUpdate(unsigned char *Pointer_Buffer);

#endif
