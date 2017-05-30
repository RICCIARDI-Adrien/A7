/** @file System_Display.c
 * @see System_Display.h for description.
 * @author Adrien RICCIARDI
 * @note Controllers access procedures are based on OpenGLCD ones.
 */
#include <System.h> // To have _XTAL_FREQ definition for delay() functions
#include <System_Display.h>
#include <xc.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** CS1 pin. */
#define SYSTEM_DISPLAY_PIN_CHIP_SELECT_1 LATBbits.LATB0
/** CS2 pin. */
#define SYSTEM_DISPLAY_PIN_CHIP_SELECT_2 LATBbits.LATB1
/** D/I (also known as R/S) pin. */
#define SYSTEM_DISPLAY_PIN_DATA_INSTRUCTION LATBbits.LATB2
/** R/W pin. */
#define SYSTEM_DISPLAY_PIN_READ_WRITE LATBbits.LATB3
/** E pin. */
#define SYSTEM_DISPLAY_PIN_ENABLE LATBbits.LATB4
/** RST pin. */
#define SYSTEM_DISPLAY_PIN_RESET LATBbits.LATB5

/** The busy flag bit in the status register. */
#define SYSTEM_DISPLAY_STATUS_REGISTER_BUSY_FLAG_BIT_MASK 0x80

/** Set the frame timer to have 20 frames per second. */
#define SYSTEM_DISPLAY_FRAME_COUNTER_PRELOAD_VALUE (65335 - 50000) // Frequency_Division_Factor = Fosc / 4 / Prescaler_Value / Requested_Frames_Per_Seconds = Fosc / 4 / 8 / 20 = 50000. As the timer can only increment, it must be preloaded with its maximum value (65535) minus the found frequency division factor, so the timer can overflow at the requested time

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Hold the display picture. All drawing operations can be done in RAM then the final picture sent to the display to save a lot of time. */
static unsigned char System_Display_Frame_Buffer[SYSTEM_DISPLAY_WIDTH * SYSTEM_DISPLAY_HEIGHT / 8];

/** The cursor used in text rendering mode horizontal location. */
static unsigned char System_Display_Text_Cursor_X = 0;
/** The cursor used in text rendering mode vertical location. */
static unsigned char System_Display_Text_Cursor_Y = 0;

/** First 128 ASCII characters sprites. */
static const unsigned char System_Display_Font_Sprites[][SYSTEM_DISPLAY_TEXT_CHARACTER_WIDTH] =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 0
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 1
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 2
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 3
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 4
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 5
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 6
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 7
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 8
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 9
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 10
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 11
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 12
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 13
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 14
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 15
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 16
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 17
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 18
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 19
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 20
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 21
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 22
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 23
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 24
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 25
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 26
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 27
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 28
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 29
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 30
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ASCII code 31
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Space
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '!' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '"' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '#' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '$' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '%' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '&' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ''' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '(' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ')' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '*' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '+' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ',' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '-' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '.' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '/' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '0' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '1' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '2' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '3' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '4' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '5' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '6' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '7' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '8' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '9' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ':' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ';' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '<' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '=' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '>' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '?' TODO
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '@' TODO
	{ 0x7C, 0x12, 0x11, 0x12, 0x7C, 0x00 }, // Uppercase 'A'
	{ 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00 }, // Uppercase 'B'
	{ 0x3E, 0x41, 0x41, 0x41, 0x41, 0x00 }, // Uppercase 'C'
	{ 0x7F, 0x41, 0x41, 0x41, 0x3E, 0x00 }, // Uppercase 'D'
	{ 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00 }, // Uppercase 'E'
	{ 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00 }, // Uppercase 'F'
	{ 0x3E, 0x41, 0x49, 0x49, 0x79, 0x00 }, // Uppercase 'G'
	{ 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00 }, // Uppercase 'H'
	{ 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00 }, // Uppercase 'I'
	{ 0x20, 0x41, 0x41, 0x3F, 0x01, 0x00 }, // Uppercase 'J'
	{ 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00 }, // Uppercase 'K'
	{ 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00 }, // Uppercase 'L'
	{ 0x7F, 0x02, 0x04, 0x02, 0x7F, 0x00 }, // Uppercase 'M'
	{ 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00 }, // Uppercase 'N'
	{ 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00 }, // Uppercase 'O'
	{ 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00 }, // Uppercase 'P'
	{ 0x3E, 0x41, 0x41, 0x21, 0x5E, 0x00 }, // Uppercase 'Q'
	{ 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00 }, // Uppercase 'R'
	{ 0x46, 0x49, 0x49, 0x49, 0x31, 0x00 }, // Uppercase 'S'
	{ 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00 }, // Uppercase 'T'
	{ 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00 }, // Uppercase 'U'
	{ 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00 }, // Uppercase 'V'
	{ 0x3F, 0x40, 0x7E, 0x40, 0x3F, 0x00 }, // Uppercase 'W'
	{ 0x63, 0x14, 0x08, 0x14, 0x63, 0x00 }, // Uppercase 'X'
	{ 0x07, 0x08, 0x70, 0x08, 0x07, 0x00 }, // Uppercase 'Y'
	{ 0x61, 0x51, 0x49, 0x45, 0x43, 0x00 }, // Uppercase 'Z'
};

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Poll the specified controller "busy flag" and exit when the flag is cleared.
 * @param Is_Second_Controller_Selected Set to 0 to select controller 1, set to 1 to select controller 2.
 * @note On output, the controller remains selected.
 */
static void SystemDisplayWaitForReadyController(unsigned char Is_Second_Controller_Selected)
{
	// Select the right controller
	if (Is_Second_Controller_Selected)
	{
		SYSTEM_DISPLAY_PIN_CHIP_SELECT_1 = 1;
		SYSTEM_DISPLAY_PIN_CHIP_SELECT_2 = 0;
	}
	else
	{
		SYSTEM_DISPLAY_PIN_CHIP_SELECT_1 = 0;
		SYSTEM_DISPLAY_PIN_CHIP_SELECT_2 = 1;
	}
	
	// Put data bus pins in input mode
	TRISA = 0xFF;
	
	// Tell that an instruction will be read
	SYSTEM_DISPLAY_PIN_READ_WRITE = 1;
	SYSTEM_DISPLAY_PIN_DATA_INSTRUCTION = 0;
	
	// Start reading
	SYSTEM_DISPLAY_PIN_ENABLE = 1;
	// Wait 375ns (at least 320ns required by datasheet)
	#asm
		nop
		nop
		nop
		nop
		nop
		nop
	#endasm
	
	// Wait for the busy flag to disappear
	while (PORTA & SYSTEM_DISPLAY_STATUS_REGISTER_BUSY_FLAG_BIT_MASK);
	
	// Stop reading
	SYSTEM_DISPLAY_PIN_ENABLE = 0;
}

/** Write a command or a byte of data to the specified controller.
 * @param Is_Second_Controller_Selected Set to 0 to select controller 1, set to 1 to select controller 2.
 * @param Is_Command Set to 0 to transmit data, set to 1 to transmit a command.
 * @param Byte The command or data value.
 */
static void SystemDisplayWriteByte(unsigned char Is_Second_Controller_Selected, unsigned char Is_Command, unsigned char Byte)
{
	// Wait for the controller to become ready
	SystemDisplayWaitForReadyController(Is_Second_Controller_Selected);
	
	// Start E cycle
	SYSTEM_DISPLAY_PIN_ENABLE = 0;
		
	// Force write mode
	SYSTEM_DISPLAY_PIN_READ_WRITE = 0;
	
	// Tell what will be sent (command or data)
	if (Is_Command) SYSTEM_DISPLAY_PIN_DATA_INSTRUCTION = 0;
	else SYSTEM_DISPLAY_PIN_DATA_INSTRUCTION = 1;
	
	// Set data to transmit
	TRISA = 0; // Put data pins in output mode
	LATA = Byte;
	
	// Wait 500ns (an instruction cycle lasts 1 / (64000000 / 4) = 62.5ns), datasheet tells to wait at least 450ns
	#asm
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
	#endasm
	
	// Do E cycle second part
	SYSTEM_DISPLAY_PIN_ENABLE = 1;
	#asm // Wait 500ns (at least 450ns required by datasheet)
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
	#endasm
		
	// Terminate E cycle
	SYSTEM_DISPLAY_PIN_ENABLE = 0;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemDisplayInitialize(void)
{
	unsigned char Controller_ID;
	
	// Configure pins as outputs
	// Set pins as digital
	ANSELA = 0;
	ANSELB = 0;
	// Configure pin direction
	TRISB &= ~0x3F; // Pins 7 and 6 are used by the serial port module
	
	// Reset controller
	SYSTEM_DISPLAY_PIN_RESET = 0;
	__delay_ms(2); // 1us should be enough according to the datasheet, use a factor 2000 to be sure
	SYSTEM_DISPLAY_PIN_RESET = 1; // Release reset
	
	// Wait for reset cycle to terminate
	__delay_ms(50); // A so huge value should be enough
	
	// Configure both controllers
	SystemDisplayClear();
	for (Controller_ID = 0; Controller_ID < 2; Controller_ID++) SystemDisplayWriteByte(Controller_ID, 1, 0x3F); // Turn display on
}

void SystemDisplayClear(void)
{
	unsigned char Controller_ID, Row, Column;
	
	// Access all controllers
	for (Controller_ID = 0; Controller_ID < 2; Controller_ID++)
	{
		// Turn off all pixels
		for (Row = 0; Row < SYSTEM_DISPLAY_HEIGHT / 8; Row++)
		{
			// Set row number
			SystemDisplayWriteByte(Controller_ID, 1, 0xB8 | Row);
			
			// Make column number start from zero
			SystemDisplayWriteByte(Controller_ID, 1, 0x40);
			
			// Clear all pixels
			for (Column = 0; Column < SYSTEM_DISPLAY_WIDTH / 2; Column++) SystemDisplayWriteByte(Controller_ID, 0, 0);
		}
	}
}

void SystemDisplayBeginRendering(void)
{
	unsigned short i;
	
	// Clear frame buffer (no need to clear the display because it will be fully overwritten by the frame buffer content)
	for (i = 0; i < sizeof(System_Display_Frame_Buffer); i++) System_Display_Frame_Buffer[i] = 0;
}

void SystemDisplayEndRendering(void)
{
	unsigned char Controller_ID, Row, Column;
	unsigned short i = 0;
	
	// Draw the row of the first controller then the row of the second to allow the frame array buffer to be read linearly
	for (Row = 0; Row < SYSTEM_DISPLAY_HEIGHT / 8; Row++) // A row is composed of 8 vertical pixels (stored in one byte)
	{
		// Access all controllers
		for (Controller_ID = 0; Controller_ID < 2; Controller_ID++)
		{
			// Set row number
			SystemDisplayWriteByte(Controller_ID, 1, 0xB8 | Row);
			
			// Make column number start from zero
			SystemDisplayWriteByte(Controller_ID, 1, 0x40);
			
			// Display the row
			for (Column = 0; Column < SYSTEM_DISPLAY_WIDTH / 2; Column++) // Each controller handle half display width
			{
				SystemDisplayWriteByte(Controller_ID, 0, System_Display_Frame_Buffer[i]);
				i++;
			}
		}
	}
}

void SystemDisplayRenderSprite(unsigned char X, unsigned char Y, const unsigned char *Pointer_Sprite_Pixels, unsigned char Width, unsigned char Height)
{
	unsigned char *Pointer_Frame_Buffer, Y_Remainder, Y_Quotient, Sprite_Pixels;
	
	// Cache Y divided by 8 operation result
	Y_Quotient = Y >> 3;
	Y_Remainder = Y - (Y_Quotient << 3); // If the remainder is 0, the sprite vertical coordinate is aligned with the frame buffer row, if not, some shifting must be done
	
	// Compute the location in the frame buffer where the sprite must be drawn
	Pointer_Frame_Buffer = (unsigned char *) (System_Display_Frame_Buffer + (Y_Quotient * SYSTEM_DISPLAY_WIDTH + X)); // Each frame buffer row contains 8 pixels rows, so divide Y by 8
	
	// A whole byte contains the pixels for 8 consecutive vertical rows, so adjust height to talk in bytes to make drawing operation easier and faster
	Height >>= 3; // Due to display module hardware, height is always a multiple of 8
	
	// XOR the sprite bits with the current one in the frame buffer
	for (Y = 0; Y < Height; Y++) // Y variable is not needed anymore, recycle it
	{
		for (X = 0; X < Width; X++) // X variable is not needed anymore, recycle it
		{
			// Sprite vertical coordinate is not aligned withe display row, sprite 8 vertical pixels must be shifted on this display row and the next one
			if (Y_Remainder != 0)
			{
				// Cache sprite pixels value
				Sprite_Pixels = *Pointer_Sprite_Pixels;
				
				// Draw the begining of the sprite row on the current frame buffer row
				*Pointer_Frame_Buffer ^= Sprite_Pixels << Y_Remainder;
				// Draw the end of the sprite row on the next frame buffer row
				*(Pointer_Frame_Buffer + SYSTEM_DISPLAY_WIDTH) ^= (Sprite_Pixels >> (8 - Y_Remainder));
			}
			// Sprite vertical coordinate is aligned with display rows, there is just need to copy the sprite content
			else *Pointer_Frame_Buffer ^= *Pointer_Sprite_Pixels;
			
			Pointer_Frame_Buffer++;
			Pointer_Sprite_Pixels++;
		}
		
		// Go to next frame buffer row
		Pointer_Frame_Buffer += SYSTEM_DISPLAY_WIDTH - Width; // "Width" bytes have been written yet, subtract them
	}
}

void SystemDisplayStartFrameTimer(void)
{
	// Configure timer 5 to overflow 50ms later
	T5CON = 0x30; // Source timer with instruction clock (Fosc/4), select a 1:8 prescaler, disable dedicated secondary oscillator circuit, use two 8-bit operations to access to the timer value
	T5GCON = 0; // Disable gating mode
	// Preload timer
	TMR5H = SYSTEM_DISPLAY_FRAME_COUNTER_PRELOAD_VALUE << 8;
	TMR5L = (unsigned char) SYSTEM_DISPLAY_FRAME_COUNTER_PRELOAD_VALUE;
	
	// Start timer
	T5CONbits.TMR5ON = 1;
}

void SystemDisplayWaitForFrameTimerEnd(void)
{
	// Wait for the timer overflow
	while (!PIR5bits.TMR5IF);
	
	// Stop timer
	T5CONbits.TMR5ON = 0;
	
	// Reset interrupt flag
	PIR5bits.TMR5IF = 0;
}

void SystemDisplaySetTextCursor(unsigned char X, unsigned char Y)
{
	// Make sure the character will be entirely displayed on the screen
	if ((X >= SYSTEM_DISPLAY_WIDTH / SYSTEM_DISPLAY_TEXT_CHARACTER_WIDTH) || (Y >= SYSTEM_DISPLAY_HEIGHT / SYSTEM_DISPLAY_TEXT_CHARACTER_HEIGHT)) return;
	
	System_Display_Text_Cursor_X = X;
	System_Display_Text_Cursor_Y = Y;
}

void SystemDisplayRenderTextCharacter(unsigned char Character)
{
	// Do nothing if the character go out the display area (clipping optimization)
	if ((System_Display_Text_Cursor_X >= SYSTEM_DISPLAY_WIDTH / SYSTEM_DISPLAY_TEXT_CHARACTER_WIDTH) || (System_Display_Text_Cursor_Y >= SYSTEM_DISPLAY_HEIGHT / SYSTEM_DISPLAY_TEXT_CHARACTER_HEIGHT)) return;

	// Render the character sprite to the frame buffer
	SystemDisplayRenderSprite(System_Display_Text_Cursor_X * SYSTEM_DISPLAY_TEXT_CHARACTER_WIDTH, System_Display_Text_Cursor_Y * SYSTEM_DISPLAY_TEXT_CHARACTER_HEIGHT, System_Display_Font_Sprites[Character], SYSTEM_DISPLAY_TEXT_CHARACTER_WIDTH, SYSTEM_DISPLAY_TEXT_CHARACTER_HEIGHT);
	
	// Move text cursor one position on the left
	System_Display_Text_Cursor_X++;
	
	// TODO go to next line when cursor reached display width
}

void SystemDisplayRenderTextString(const unsigned char *String)
{
	while (*String)
	{
		SystemDisplayRenderTextCharacter(*String);
		String++;
	}
}
