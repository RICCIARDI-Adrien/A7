/** @file System.h
 * Contain system-specific constants and program entry point routines. This is the only file an applications must include to get access to all system features.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_H
#define H_SYSTEM_H

// Provide all needed includes
#include <System_Display.h>
#include <System_External_EEPROM.h>
#include <System_Keyboard.h>
#include <System_Random.h>
#include <System_Serial_Port.h>
#include <xc.h> // Provide microcontroller definitions

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Oscillator frequency in Hz (needed by software delay routines). */
#define _XTAL_FREQ 64000000

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize all system peripherals. This function must be the first call of every application. */
void SystemInitialize(void);

#endif