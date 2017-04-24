/** @file System_Keyboard.c
 * See System_Keyboard.h for description.
 * @author Adrien RICCIARDI
 */
#include <System_Keyboard.h>
#include <xc.h>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The last received key make code. */
static unsigned char System_Keyboard_Last_Make_Code;
/** Tell if a new unread key is available. */
static volatile unsigned char System_Keyboard_Is_Key_Available = 0;

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
	TRISCbits.RC7 = 1;
	TRISCbits.RC6 = 1;	
	
	// Enable interrupts 
	PIE1bits.RC1IE = 1; // UART reception interrupt
	PIE5bits.TMR6IE = 1; // Timer interrupt
}

void SystemKeyboardUARTInterruptHandler(void)
{
	static unsigned char Is_Break_Code_Received = 0; // Tell if the 0xF0 break code has been previously received or not
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
	
	// Is it the break code ?
	if (Received_Byte == 0xF0)
	{
		Is_Break_Code_Received = 1;
		return;
	}
	
	// If the last received key was the break code, do not take the key into account
	if (Is_Break_Code_Received)
	{
		Is_Break_Code_Received = 0;
		return;
	}
	
	// Keep the make code (the corresponding ASCII symbol will be retrieved when the user wants to read a key, saving some cycles in the interrupt)
	System_Keyboard_Last_Make_Code = Received_Byte;
	System_Keyboard_Is_Key_Available = 1;
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
	
	// TODO return corresponding table entry
	return System_Keyboard_Last_Make_Code;
}