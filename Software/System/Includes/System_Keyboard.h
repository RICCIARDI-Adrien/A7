/** @file System_Keyboard.h
 * Handle a PS/2 keyboard connected to EUSART module 1.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_KEYBOARD_H
#define H_SYSTEM_KEYBOARD_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the EUSART module the keyboard is connected to. */
void SystemKeyboardInitialize(void);

/** Called when EUSART 1 module receives a byte. */
void SystemKeyboardUARTInterruptHandler(void);

/** Called when the keyboard timer overflows. */
void SystemKeyboardTimerInterruptHandler(void);

/** Wait for a key to be typed (or return the last typed one if available).
 * @return The ASCII code of the last typed key.
 */
unsigned char SystemKeyboardReadCharacter(void);

#endif