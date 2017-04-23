/** @file System.h
 * Contain system-specific and program entry point routines.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_H
#define H_SYSTEM_H

// Provide all needed includes
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