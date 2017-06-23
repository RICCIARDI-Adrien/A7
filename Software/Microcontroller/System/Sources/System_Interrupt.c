/** @file System_Interrupt.c
 * @see System_Interrupt.h for description.
 * @author Adrien RICCIARDI
 */
#include <System_Keyboard.h>
#include <xc.h>

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Entry point for all high priority interrupts (i.e. all interrupts because only high priority one are enabled). */
void __interrupt high_priority SystemInterruptHighPriority(void)
{
	// Did the keyboard sent a key ?
	if (PIR1bits.RC1IF) SystemKeyboardUARTInterruptHandler();
	
	// Has the keyboard timer overflowed ?
	if (PIR5bits.TMR6IF) SystemKeyboardTimerInterruptHandler();
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemInterruptInitialize(void)
{
	// Use only high-priority interrupts
	RCONbits.IPEN = 0; // Disable interrupts priority levels
	INTCON = 0xC0; // Enable al unmasked interrupts, enable all unmasked peripheral interrupts
}