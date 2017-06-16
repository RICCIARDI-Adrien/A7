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
void MenuShowFiles(unsigned char *String_Menu_Title);

/** Download a file from the serial port and store it on the external EEPROM. */
void MenuDownloadFile(void);

#endif
