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
// WARNING the following pinout is the Mikroe EasyPic v7 one, not the final circuit one
/** RST pin. */
#define SYSTEM_DISPLAY_PIN_RESET LATBbits.LATB5
/** R/W pin. */
#define SYSTEM_DISPLAY_PIN_READ_WRITE LATBbits.LATB3
/** D/I (also known as R/S) pin. */
#define SYSTEM_DISPLAY_PIN_DATA_INSTRUCTION LATBbits.LATB2
/** E pin. */
#define SYSTEM_DISPLAY_PIN_ENABLE LATBbits.LATB4
/** CS2 pin. */
#define SYSTEM_DISPLAY_PIN_CHIP_SELECT_2 LATBbits.LATB1
/** CS1 pin. */
#define SYSTEM_DISPLAY_PIN_CHIP_SELECT_1 LATBbits.LATB0

/** The busy flag bit in the status register. */
#define SYSTEM_DISPLAY_STATUS_REGISTER_BUSY_FLAG_BIT_MASK 0x80

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
	unsigned char Controller_ID, Row, Column;
	
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
	for (Controller_ID = 0; Controller_ID < 2; Controller_ID++)
	{
		// Turn off all pixels
		for (Row = 0; Row < 8; Row++)
		{
			// Set row number
			SystemDisplayWriteByte(Controller_ID, 1, 0xB8 | Row);
			
			// Make column number start from zero
			SystemDisplayWriteByte(Controller_ID, 1, 0x40);
			
			// Clear all pixels
			for (Column = 0; Column < 64; Column++) SystemDisplayWriteByte(Controller_ID, 0, 0);
		}
		
		// Turn display on
		SystemDisplayWriteByte(Controller_ID, 1, 0x3F);
	}
}
