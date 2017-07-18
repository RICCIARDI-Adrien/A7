/** @file System_Keyboard.c
 * See System_Keyboard.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h> // Needed for the clock frequency constant
#include <System_Keyboard.h>
#include <xc.h>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The last received character. */
static unsigned char System_Keyboard_Received_Character;
/** Tell if a new unread key is available. */
static volatile unsigned char System_Keyboard_Is_Key_Available = 0;

/** Maintain a bit map telling which "game" keys are pressed and which are released. */
static volatile unsigned char System_Keyboard_Game_Keys_State_Bit_Mask = 0;

/** Characters issued when no modifier key is pressed (AZERTY mapping). */
static unsigned char System_Keyboard_Lowercase_Characters[] =
{
	0, // Offset 0
	SYSTEM_KEYBOARD_KEY_CODE_F9, // Offset 0x01
	0, // Offset 0x02
	SYSTEM_KEYBOARD_KEY_CODE_F5, // Offset 0x03
	SYSTEM_KEYBOARD_KEY_CODE_F3, // Offset 0x04
	SYSTEM_KEYBOARD_KEY_CODE_F1, // Offset 0x05
	SYSTEM_KEYBOARD_KEY_CODE_F2, // Offset 0x06
	SYSTEM_KEYBOARD_KEY_CODE_F12, // Offset 0x07
	0, // Offset 0x08
	SYSTEM_KEYBOARD_KEY_CODE_F10, // Offset 0x09
	SYSTEM_KEYBOARD_KEY_CODE_F8, // Offset 0x0A
	SYSTEM_KEYBOARD_KEY_CODE_F6, // Offset 0x0B
	SYSTEM_KEYBOARD_KEY_CODE_F4, // Offset 0x0C
	'\t', // Offset 0x0D
	'`', // Offset 0x0E
	0, // Offset 0x0F
	0, // Offset 0x10
	SYSTEM_KEYBOARD_KEY_CODE_LEFT_ALT, // 0x11
	0, // 0x12
	0, // Offset 0x13
	SYSTEM_KEYBOARD_KEY_CODE_LEFT_CONTROL, // Offset 0x14
	'a', // Offset 0x15
	'&', // Offset 0x16
	0, // Offset 0x17
	0, // Offset 0x18
	0, // Offset 0x19
	'w', // Offset 0x1A
	's', // Offset 0x1B
	'q', // Offset 0x1C
	'z', // Offset 0x1D
	'\202', // Offset 0x1E
	0, // Offset 0x1F (TODO E0, no confict)
	0, // Offset 0x20
	'c', // Offset 0x21
	'x', // Offset 0x22
	'd', // Offset 0x23
	'e', // Offset 0x24
	'\'', // Offset 0x25
	'\"', // Offset 0x26
	0, // Offset 0x27 (TODO E0, no conflict)
	0, // Offset 0x28
	' ', // Offset 0x29
	'v', // Offset 0x2A
	'f', // Offset 0x2B
	't', // Offset 0x2C
	'r', // Offset 0x2D
	'(', // Offset 0x2E
	0, // Offset 0x2F (TODO E0, no conflict, DO NOT MAP (mouse right-click menu key)
	0, // Offset 0x30
	'n', // Offset 0x31
	'b', // Offset 0x32
	'h', // Offset 0x33
	'g', // Offset 0x34
	'y', // Offset 0x35
	'-', // Offset 0x36
	0, // Offset 0x37
	0, // Offset 0x38
	0, // Offset 0x39
	',', // Offset 0x3A
	'j', // Offset 0x3B
	'u', // Offset 0x3C
	'\212', // Offset 0x3D
	'_', // Offset 0x3E
	0, // Offset 0x3F
	0, // Offset 0x40
	';', // Offset 0x41
	'k', // Offset 0x42
	'i', // Offset 0x43
	'o', // Offset 0x44
	'\205', // Offset 0x45
	'\135', // Offset 0x46
	0, // Offset 0x47
	0, // Offset 0x48
	':', // Offset 0x49
	'!', // Offset 0x4A
	'l', // Offset 0x4B
	'm', // Offset 0x4C
	'p', // Offset 0x4D
	')', // Offset 0x4E
	0, // Offset 0x4F
	0, // Offset 0x50
	0, // Offset 0x51
	'\227', // Offset 0x52
	0, // Offset 0x53
	'^', // Offset 0x54
	'=', // Offset 0x55
	0, // Offset 0x56
	0, // Offset 0x57
	SYSTEM_KEYBOARD_KEY_CODE_CAPITALS_LOCK, // Offset 0x58
	0, // Offset 0x59 (TODO right shift)
	'\n', // Offset 0x5A
	'$', // Offset 0x5B
	0, // Offset 0x5C
	'*', // Offset 0x5D
	0, // Offset 0x5E
	0, // Offset 0x5F
	0, // Offset 0x60
	0, // Offset 0x61
	0, // Offset 0x62
	0, // Offset 0x63
	0, // Offset 0x64
	0, // Offset 0x65
	'\b', // Offset 0x66
	0, // Offset 0x67
	0, // Offset 0x68
	'1', // Offset 0x69 (TODO also END)
	0, // Offset 0x6A
	'4', // Offset 0x6B (TODO also left arrow)
	'7', // Offset 0x6C (TODO also HOME)
	0, // Offset 0x6D
	0, // Offset 0x6E
	0, // Offset 0x6F
	'0', // Offset 0x70 (TODO also INSERT)
	'.', // Offset 0x71 (TODO also DELETE)
	'2', // Offset 0x72 (TODO also down arrow)
	'5', // Offset 0x73
	'6', // Offset 0x74 (TODO also right arrow)
	'8', // Offset 0x75 (TODO also up arrow)
	SYSTEM_KEYBOARD_KEY_CODE_ESCAPE, // Offset 0x76
	0, // Offset 0x77
	SYSTEM_KEYBOARD_KEY_CODE_F11, // Offset 0x78
	'+', // Offset 0x79
	'3', // Offset 0x7A (TODO also PAGE DOWN)
	'-', // Offset 0x7B
	'*', // Offset 0x7C
	'9', // Offset 0x7D (TODO also PAGE UP)
	0, // Offset 0x7E
	0 // Offset 0x7F
};

/** Characters issued when shift is pressed or CAPS LOCK is set (AZERTY mapping). */
static unsigned char System_Keyboard_Uppercase_Characters[] =
{
	0, // Offset 0
	SYSTEM_KEYBOARD_KEY_CODE_F9, // Offset 0x01
	0, // Offset 0x02
	SYSTEM_KEYBOARD_KEY_CODE_F5, // Offset 0x03
	SYSTEM_KEYBOARD_KEY_CODE_F3, // Offset 0x04
	SYSTEM_KEYBOARD_KEY_CODE_F1, // Offset 0x05
	SYSTEM_KEYBOARD_KEY_CODE_F2, // Offset 0x06
	SYSTEM_KEYBOARD_KEY_CODE_F12, // Offset 0x07
	0, // Offset 0x08
	SYSTEM_KEYBOARD_KEY_CODE_F10, // Offset 0x09
	SYSTEM_KEYBOARD_KEY_CODE_F8, // Offset 0x0A
	SYSTEM_KEYBOARD_KEY_CODE_F6, // Offset 0x0B
	SYSTEM_KEYBOARD_KEY_CODE_F4, // Offset 0x0C
	'\t', // Offset 0x0D
	'`', // Offset 0x0E
	0, // Offset 0x0F
	0, // Offset 0x10
	SYSTEM_KEYBOARD_KEY_CODE_LEFT_ALT, // 0x11
	0, // 0x12
	0, // Offset 0x13
	SYSTEM_KEYBOARD_KEY_CODE_LEFT_CONTROL, // Offset 0x14
	'A', // Offset 0x15
	'1', // Offset 0x16
	0, // Offset 0x17
	0, // Offset 0x18
	0, // Offset 0x19
	'W', // Offset 0x1A
	'S', // Offset 0x1B
	'Q', // Offset 0x1C
	'Z', // Offset 0x1D
	'2', // Offset 0x1E
	0, // Offset 0x1F (TODO E0, no confict)
	0, // Offset 0x20
	'C', // Offset 0x21
	'X', // Offset 0x22
	'D', // Offset 0x23
	'E', // Offset 0x24
	'4', // Offset 0x25
	'3', // Offset 0x26
	0, // Offset 0x27 (TODO E0, no conflict)
	0, // Offset 0x28
	' ', // Offset 0x29
	'V', // Offset 0x2A
	'F', // Offset 0x2B
	'T', // Offset 0x2C
	'R', // Offset 0x2D
	'5', // Offset 0x2E
	0, // Offset 0x2F (TODO E0, no conflict, DO NOT MAP (mouse right-click menu key)
	0, // Offset 0x30
	'N', // Offset 0x31
	'B', // Offset 0x32
	'H', // Offset 0x33
	'G', // Offset 0x34
	'Y', // Offset 0x35
	'6', // Offset 0x36
	0, // Offset 0x37
	0, // Offset 0x38
	0, // Offset 0x39
	'?', // Offset 0x3A
	'J', // Offset 0x3B
	'U', // Offset 0x3C
	'7', // Offset 0x3D
	'8', // Offset 0x3E
	0, // Offset 0x3F
	0, // Offset 0x40
	'.', // Offset 0x41
	'K', // Offset 0x42
	'I', // Offset 0x43
	'O', // Offset 0x44
	'0', // Offset 0x45
	'9', // Offset 0x46
	0, // Offset 0x47
	0, // Offset 0x48
	'/', // Offset 0x49
	'\025', // Offset 0x4A
	'L', // Offset 0x4B
	'M', // Offset 0x4C
	'P', // Offset 0x4D
	'\247', // Offset 0x4E
	0, // Offset 0x4F
	0, // Offset 0x50
	0, // Offset 0x51
	'%', // Offset 0x52
	0, // Offset 0x53
	0, // Offset 0x54
	'+', // Offset 0x55
	0, // Offset 0x56
	0, // Offset 0x57
	SYSTEM_KEYBOARD_KEY_CODE_CAPITALS_LOCK, // Offset 0x58
	0, // Offset 0x59 (TODO right shift)
	'\n', // Offset 0x5A
	'\234', // Offset 0x5B
	0, // Offset 0x5C
	'\346', // Offset 0x5D
	0, // Offset 0x5E
	0, // Offset 0x5F
	0, // Offset 0x60
	0, // Offset 0x61
	0, // Offset 0x62
	0, // Offset 0x63
	0, // Offset 0x64
	0, // Offset 0x65
	'\b', // Offset 0x66
	0, // Offset 0x67
	0, // Offset 0x68
	'1', // Offset 0x69 (TODO also END)
	0, // Offset 0x6A
	'4', // Offset 0x6B (TODO also left arrow)
	'7', // Offset 0x6C (TODO also HOME)
	0, // Offset 0x6D
	0, // Offset 0x6E
	0, // Offset 0x6F
	'0', // Offset 0x70 (TODO also INSERT)
	'.', // Offset 0x71 (TODO also DELETE)
	'2', // Offset 0x72 (TODO also down arrow)
	'5', // Offset 0x73
	'6', // Offset 0x74 (TODO also right arrow)
	'8', // Offset 0x75 (TODO also up arrow)
	SYSTEM_KEYBOARD_KEY_CODE_ESCAPE, // Offset 0x76
	0, // Offset 0x77
	SYSTEM_KEYBOARD_KEY_CODE_F11, // Offset 0x78
	'+', // Offset 0x79
	'3', // Offset 0x7A (TODO also PAGE DOWN)
	'-', // Offset 0x7B
	'*', // Offset 0x7C
	'9', // Offset 0x7D (TODO also PAGE UP)
	0, // Offset 0x7E
	0 // Offset 0x7F
};

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemKeyboardInitialize(void)
{
	// Configure UART for synchronous operation
	TXSTA1 = 0x50; // Select slave mode, select 9-bit transmission (may be useful if something must be sent to the keyboard), disable transmit (for now), select synchronous mode
	RCSTA1 = 0xD0; // Enable serial port, select 9-bit reception, enable continuous reception
	BAUDCON1 = 0; // Data is not inverted (logic '0' is 0V and logic '1' is Vcc), sample data on clock falling edge
	
	// Configure UART pins
	// Set pins as digital
	ANSELCbits.ANSC7 = 0;
	ANSELCbits.ANSC6 = 0;
	// Disable pin output driver (set them as input)
	TRISCbits.TRISC7 = 1;
	TRISCbits.TRISC6 = 1;
	
	// Enable interrupts
	PIE1bits.RC1IE = 1; // UART reception interrupt
	PIE5bits.TMR6IE = 1; // Timer interrupt
}

void SystemKeyboardUARTInterruptHandler(void)
{
	static unsigned char Is_Break_Code_Received = 0, Is_Extended_Code_Received = 0, Is_Left_Shift_Pressed = 0, Is_Right_Shift_Pressed = 0;
	unsigned char Received_Byte, Is_Most_Significant_Bit_Set;
	
	// Immediately stop UART reception to avoid shifting the remaining bit, which would corrupt the next received byte
	RCSTA1bits.CREN = 0;
	
	// Get the byte (and clear the reception interrupt in the same time)
	Is_Most_Significant_Bit_Set = RCSTA1 & 0x01; // Get RX9D bit
	Received_Byte = RCREG1;
	
	// Most significant bit is the 9th one
	Received_Byte >>= 1; // Discard less significant bit (parity bit)
	if (Is_Most_Significant_Bit_Set) Received_Byte |= 0x80; // Put 9th bit in place of bit 7
	
	// Start a timer that will re-enable UART reception when the last bits of data have been transmitted (so they can't be shifted in the UART shift register). // Keyboard minimum transmission frequency seems to be 10KHz (but most likely > 12KHz), so each bit takes 1/10000 = 100us to be transmitted. As there is one superfluous data bit and the stop bit to discard, making the timer overflow in 256us should be enough
	TMR6 = 0; // Initialize timer (TMR6 register value must match PR6 register value to generate an overflow, but PR6 is automatically initialized to 256, so no need to set its value)
	T6CON = 0x07; // Select a 1:1 postscaler, enable timer, select a 1:16 prescaler (timer overflow time = ((Fosc / 4) / Prescaler) / 256) = 256us
	
	// Is it the extended key code ?
	if (Received_Byte == 0xE0)
	{
		Is_Extended_Code_Received = 1;
		return;
	}
	
	// Is it the break code ?
	if (Received_Byte == 0xF0)
	{
		Is_Break_Code_Received = 1;
		return;
	}
	
	// Handle key release
	if (Is_Break_Code_Received)
	{
		// Keys using extended code
		if (Is_Extended_Code_Received)
		{
			switch (Received_Byte)
			{
				// Left arrow game key
				case 0x6B:
					System_Keyboard_Game_Keys_State_Bit_Mask &= ~SYSTEM_KEYBOARD_GAME_KEY_STATE_BIT_MASK_LEFT_ARROW;
					break;
					
				// Down arrow game key
				case 0x72:
					System_Keyboard_Game_Keys_State_Bit_Mask &= ~SYSTEM_KEYBOARD_GAME_KEY_STATE_BIT_MASK_DOWN_ARROW;
					break;
					
				// Right arrow game key
				case 0x74:
					System_Keyboard_Game_Keys_State_Bit_Mask &= ~SYSTEM_KEYBOARD_GAME_KEY_STATE_BIT_MASK_RIGHT_ARROW;
					break;
					
				// Up arrow game key
				case 0x75:
					System_Keyboard_Game_Keys_State_Bit_Mask &= ~SYSTEM_KEYBOARD_GAME_KEY_STATE_BIT_MASK_UP_ARROW;
					break;
					
				default:
					break;
			}
		}
		// Keys using normal code
		else
		{
			switch (Received_Byte)
			{
				// Left shift
				case 0x12:
					Is_Left_Shift_Pressed = 0;
					break;
					
				// Space game key
				case 0x29:
					System_Keyboard_Game_Keys_State_Bit_Mask &= ~SYSTEM_KEYBOARD_GAME_KEY_STATE_BIT_MASK_SPACE;
					break;
					
				// Right shift
				case 0x59:
					Is_Right_Shift_Pressed = 0;
					break;
					
				default:
					break;
			}
		}
		
		Is_Break_Code_Received = 0;
		Is_Extended_Code_Received = 0; // Also reset extended code flag in case it was an extended key break code that was just discarded
		return;
	}
	
	// Handle key press
	// Keep only some extended characters (avoid loosing a whole dedicated table space in RAM)
	if (Is_Extended_Code_Received)
	{
		Is_Extended_Code_Received = 0;
		
		switch (Received_Byte)
		{
			// Key pad '/'
			case 0x4A:
				System_Keyboard_Received_Character = '/';
				System_Keyboard_Is_Key_Available = 1;
				break;
				
			// Key pad enter
			case 0x5A:
				System_Keyboard_Received_Character = '\n';
				System_Keyboard_Is_Key_Available = 1;
				break;
				
			// Handle game keys in the same time
			// Left arrow
			case 0x6B:
				System_Keyboard_Game_Keys_State_Bit_Mask |= SYSTEM_KEYBOARD_GAME_KEY_STATE_BIT_MASK_LEFT_ARROW;
				break;
				
			// Down arrow
			case 0x72:
				System_Keyboard_Game_Keys_State_Bit_Mask |= SYSTEM_KEYBOARD_GAME_KEY_STATE_BIT_MASK_DOWN_ARROW;
				break;
				
			// Right arrow
			case 0x74:
				System_Keyboard_Game_Keys_State_Bit_Mask |= SYSTEM_KEYBOARD_GAME_KEY_STATE_BIT_MASK_RIGHT_ARROW;
				break;
				
			// Up arrow
			case 0x75:
				System_Keyboard_Game_Keys_State_Bit_Mask |= SYSTEM_KEYBOARD_GAME_KEY_STATE_BIT_MASK_UP_ARROW;
				break;
				
			default:
				break;
		}
	}
	// This is a normal code, get its translation from the table
	else if (Received_Byte < sizeof(System_Keyboard_Lowercase_Characters)) // Make sure the key is not out of the table bounds (all tables have same size)
	{
		// Handle special control characters
		if (Received_Byte == 0x12) Is_Left_Shift_Pressed = 1;
		else if (Received_Byte == 0x59) Is_Right_Shift_Pressed = 1;
		else
		{
			// Select the right case size
			if (Is_Left_Shift_Pressed || Is_Right_Shift_Pressed) System_Keyboard_Received_Character = System_Keyboard_Uppercase_Characters[Received_Byte];
			else System_Keyboard_Received_Character = System_Keyboard_Lowercase_Characters[Received_Byte];
			System_Keyboard_Is_Key_Available = 1;
		}
		
		// Handle normal game keys
		// Space
		if (Received_Byte == 0x29) System_Keyboard_Game_Keys_State_Bit_Mask |= SYSTEM_KEYBOARD_GAME_KEY_STATE_BIT_MASK_SPACE;
	}
}

void SystemKeyboardTimerInterruptHandler(void)
{
	// Stop the timer
	T6CON = 0;
	
	// Re-enable UART reception
	RCSTA1bits.CREN = 1;
	
	// Reset interrupt flag
	PIR5bits.TMR6IF = 0;
}

unsigned char SystemKeyboardReadCharacter(void)
{
	// Wait for a key to be received
	while (!System_Keyboard_Is_Key_Available);
	System_Keyboard_Is_Key_Available = 0;
	
	return System_Keyboard_Received_Character;
}

unsigned char SystemKeyboardReadCharacterNoInterrupt(void)
{
	// Enable UART
	RCSTA1bits.SPEN = 1;
	
	// Get bytes from the keyboard until a full character has been received
	do
	{
		// Wait for a byte to be received by the UART
		while (!PIR1bits.RC1IF);
		
		// Handle the received byte
		SystemKeyboardUARTInterruptHandler();
		
		// Re-enable UART reception when the whole data frame has been sent by the keyboard (in order to discard the latest frame bits)
		__delay_us(256);
		RCSTA1bits.CREN = 1;
	} while (!System_Keyboard_Is_Key_Available);
	
	// Disable UART to avoid receiving data while not polling for, which would lead to an UART overflow
	RCSTA1bits.SPEN = 0;
	
	// Tell that the character has been retrieved
	System_Keyboard_Is_Key_Available = 0;
	
	return System_Keyboard_Received_Character;
}

unsigned char SystemKeyboardReadGameKeysState(void)
{
	return System_Keyboard_Game_Keys_State_Bit_Mask;
}

unsigned char SystemKeyboardIsKeyAvailable(void)
{
	return System_Keyboard_Is_Key_Available;
}
