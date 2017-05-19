/** @file System_Display.h
 * Manage the 128x64 monochrome graphic display.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_DISPLAY_H
#define H_SYSTEM_DISPLAY_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Reset and configure the graphic display for operations. */
void SystemDisplayInitialize(void);

/** Clear the display. */
void SystemDisplayClear(void);

#endif