/** @file Menu.h
 * Gather functions common to all menus and menus entry points.
 * @author Adrien RICCIARDI
 */
#ifndef H_MENU_H
#define H_MENU_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Display the files on the screen.
 * @param String_Menu_Title The text to display on the first line.
 */
void MenuShowFiles(const unsigned char *String_Menu_Title);

/** Load a program from the external EEPROM into the program memory and start it. */
void MenuStartProgram(void);

/** Download a file from the serial port and store it on the external EEPROM. */
void MenuDownloadFile(void);

/** Mark a file system file slot as empty. */
void MenuDeleteFile(void);

#endif
