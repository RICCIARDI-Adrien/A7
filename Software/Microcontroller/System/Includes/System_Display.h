/** @file System_Display.h
 * Manage the 128x64 monochrome graphic display.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_DISPLAY_H
#define H_SYSTEM_DISPLAY_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Display width in pixels. */
#define SYSTEM_DISPLAY_WIDTH 128
/** Display height in pixels. */
#define SYSTEM_DISPLAY_HEIGHT 64

/** Text character width in pixels. */
#define SYSTEM_DISPLAY_TEXT_CHARACTER_WIDTH 6
/** Text character height in pixels. */
#define SYSTEM_DISPLAY_TEXT_CHARACTER_HEIGHT 8

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Reset and configure the graphic display for operations. */
void SystemDisplayInitialize(void);

/** Clear the internal frame buffer and return the text cursor to home position. */
void SystemDisplayClear(void);

/** Send the frame buffer content to the display. */
void SystemDisplayUpdate(void);

/** Render a sprite in the frame buffer at the specified location.
 * @param X Sprite leftmost coordinate (must be in the display area).
 * @param Y Sprite topmost coordinate (must be in the display area).
 * @param Pointer_Sprite_Pixels Byte array of sprite pixels. This array must be arranged in the display module native format : a byte represents 8 consecutive vertical pixels, bit 7 is the downer pixel and bit 0 is the upper pixel.
 * @param Width Sprite width in pixels. Value can be from 1 to the display width.
 * @param Height Sprite height in pixels. Value can be from 1 to the display height.
 */
void SystemDisplayRenderSprite(unsigned char X, unsigned char Y, const unsigned char *Pointer_Sprite_Pixels, unsigned char Width, unsigned char Height);

/** Turn a specific pixel on or off.
 * @param X The pixel X coordinate, in range [0; SYSTEM_DISPLAY_WIDTH[.
 * @param Y The pixel Y coordinate, in rage [0; SYSTEM_DISPLAY_HEIGHT[.
 * @param Is_Lighted Set to 1 to light the pixel, set to 0 to turn the pixel off.
 * @note This function does nothing if the specified coordinates are out of display bounds.
 */
void SystemDisplaySetPixelState(unsigned char X, unsigned char Y, unsigned char Is_Lighted);

/** Start a timer lasting a frame time. */
void SystemDisplayStartFrameTimer(void);
/** Wait for the frame timer to elapse. This timer allows to make all frames duration equal, even if the frame computation time varies. 
 * @note This function immediately returns if the frame lasted too long (i.e. system can't keep up).
 */
void SystemDisplayWaitForFrameTimerEnd(void);

// TODO allow to display text everywhere on the display (not only in fixed slots) ?
/** Set the coordinates of the cursor used to render text in the frame buffer.
 * @param X Horizontal cursor coordinate. Must be in range [0; 20] (maximum value is equal to the following integer division : SYSTEM_DISPLAY_WIDTH / SYSTEM_DISPLAY_TEXT_CHARACTER_WIDTH).
 * @param Y Vertical cursor coordinate. Must be in range [0; 7] (maximum value is equal to the following integer division : SYSTEM_DISPLAY_HEIGHT / SYSTEM_DISPLAY_TEXT_CHARACTER_HEIGHT).
 * @note The cursor location is not updated if the provided coordinates are out of the display bounds.
 */
void SystemDisplaySetTextCursor(unsigned char X, unsigned char Y);
/** Render an ASCII character at the current text cursor location and increment the cursor horizontal coordinate.
 * @param Character The character to display ASCII code.
 * @note The character is not rendered if the cursor coordinates are out of cursor bounds.
 */
void SystemDisplayRenderTextCharacter(unsigned char Character);
/** Render an ASCII string at the current text cursor location and increment the cursor horizontal coordinate.
 * @param String The string to render.
 * @note Characters are not rendered if the cursor coordinates are out of cursor bounds.
 */
void SystemDisplayRenderTextString(const unsigned char *String);

#endif
