/** @file AES.h
 * AES-256 CTR implementation based on FIPS 197 (http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf).
 * @author Adrien RICCIARDI
 */
#ifndef H_AES_H
#define H_AES_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the AES-256 CTR context. This function must be called once to start cipher or decipher operation.
 * @param Pointer_Key The cipher key (must be 16-byte long).
 * @note This function initializes the internal counter to 1.
 */
void AES256CTRInitialize(unsigned char *Pointer_Key);

/** Cipher or decipher data.
 * @param Pointer_Input_Buffer The data to process (buffer must be 16-byte long).
 * @param Pointer_Output_Buffer On output, contain the processed data (buffer must be be 16-byte long).
 * @warning AES256CTRInitialize() must have been called once before using this function, or the result will be unpredictable.
 */
void AES256CTRUpdate(unsigned char *Pointer_Input_Buffer, *Pointer_Output_Buffer);

#endif
