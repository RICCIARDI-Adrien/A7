/** @file System_Keyboard.h
 * Handle a PS/2 keyboard connected to EUSART module 1.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_KEYBOARD_H
#define H_SYSTEM_KEYBOARD_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
// Use never displayed ASCII codes to represent some special keyboard keys
/** "F1" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_F1 254
/** "F2" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_F2 253
/** "F3" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_F3 252
/** "F4" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_F4 251
/** "F5" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_F5 250
/** "F6" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_F6 249
/** "F7" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_F7 248
/** "F8" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_F8 247
/** "F9" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_F9 246
/** "F10" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_F10 245
/** "F11" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_F11 244
/** "F12" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_F12 243
/** "Left alt" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_LEFT_ALT 242
/** "Left shift" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_LEFT_SHIFT 241 // TODO really useful for application ?
/** "Left control" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_LEFT_CONTROL 240
/** "Caps lock" key virtual key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_CAPITALS_LOCK 239

/** "Escape" key code. */
#define SYSTEM_KEYBOARD_KEY_CODE_ESCAPE 27

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

/** Wait for a key to be typed without needing interrupts to be enabled to work (this function discards all previous typed keys).
 * @warning This function is specially designed for use by the program manager application only (program manager application can't use interrupts because the interrupt entry points in the flash memory are dedicated to the other applications it runs).
 * @warning This function can't be used with keyboard UART or timer interrupts enabled.
 * @return The ASCII code of the last typed key.
 */
unsigned char SystemKeyboardReadCharacterNoInterrupt(void);

#endif
