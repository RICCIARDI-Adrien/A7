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

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Reset and configure the graphic display for operations. */
void SystemDisplayInitialize(void);

/** Clear the display. */
void SystemDisplayClear(void);

/** Clear the internal frame buffer. This is the first rendering function that should be called in the rendering loop. */
void SystemDisplayBeginRendering(void);
/** Send the frame buffer content to the display. This is the last rendering function called in the rendering loop. */
void SystemDisplayEndRendering(void);

/** Render a sprite in the frame buffer at the specified location.
 * @param X Sprite leftmost coordinate (must be in the display area).
 * @param Y Sprite topmost coordinate (must be in the display area).
 * @param Pointer_Sprite_Pixels Byte array of sprite pixels. This array must be arranged in the display module native format : a byte represents 8 consecutive vertical pixels, bit 7 is the upper pixel and bit 0 is the downer pixel.
 * @param Width Sprite width in pixels. Value can be from 1 to the display width.
 * @param Height Height must be a multiple of 8 pixels (due to how the display module is working). Value can be from 8 to the display height.
 */
void SystemDisplayRenderSprite(unsigned char X, unsigned char Y, const unsigned char *Pointer_Sprite_Pixels, unsigned char Width, unsigned char Height);

#endif