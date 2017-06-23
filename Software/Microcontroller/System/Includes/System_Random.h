/** @File System_Random.h
 * Generate pseudo-random numbers.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_RANDOM_H
#define H_SYSTEM_RANDOM_H

//--------------------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------------------
/** Initialize the random numbers generator with a random value coming from a non-initialized register. */
void SystemRandomInitialize(void);

/** Generate a pseudo-random number.
 * @Return The pseudo-random number.
 */
unsigned char SystemRandomGetNumber(void);

#endif