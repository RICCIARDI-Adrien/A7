/** @file System.c
 * @see System.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include <System_Display.h>
#include <System_Interrupt.h>
#include <System_Keyboard.h>
#include <System_Led.h>
#include <System_Random.h>
#include <System_Serial_Port.h>
#include <xc.h>

//-------------------------------------------------------------------------------------------------
// Microcontroller configuration
//-------------------------------------------------------------------------------------------------
// CONFIG1H register
#pragma config FOSC = INTIO67, PLLCFG = ON, PRICLKEN = ON, FCMEN = OFF, IESO = OFF // Use internal oscillator, multiply oscillator frequency by 4 by enabling the PLL, enable primary clock, disable fail-safe clock monitor, disable oscillator switchover mode
// CONFIG2L register
#pragma config PWRTEN = ON, BOREN = SBORDIS, BORV = 285 // Enable power up timer, enable brown-out reset in hardware only (so it can't be disabled by software), set highest value for brown-out voltage (2.85V)
// CONFIG2H register
#pragma config WDTEN = OFF // Disable watchdog timer
// CONFIG3H register
#pragma config PBADEN = OFF, HFOFST = OFF, MCLRE = EXTMCLR // Port B pin 5..0 are configured as digital I/O on reset, wait for the oscillator to become stable before starting executing code, enable MCLR pin
// CONFIG4L register
#pragma config STVREN = ON, LVP = OFF, XINST = OFF, DEBUG = OFF // Reset on stack underflow or overflow, disable single supply ICSP, disable extended instruction set, disable background debug
// CONFIG5L register
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF // Disable all code protections
// CONFIG5H register
#pragma config CPB = OFF, CPD = OFF // Disable boot block code protection, disable data EEPROM code protection
// CONFIG6L register
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF // Disable all write protections
// CONFIG6H register
#pragma config WRTC = OFF, WRTB = OFF, WRTD = OFF // Disable configuration registers write protection, disable boot block write protection, disable data EEPROM write protection
// CONFIG7L register
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF // Disable all table read protections
// CONFIG7H register
#pragma config EBTRB = OFF // Disable boot block table read protection

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemInitialize(void)
{
	// Reinitialize peripherals to reset them
	SystemKeyboardInitialize();
	SystemSerialPortInitialize();
	SystemDisplayInitialize();
	SystemExternalEEPROMInitialize();
	SystemLedInitialize();
	SystemRandomInitialize(); // Initialize random module at the end to loose as much as possible random time initializing other devices, so the random seed will be better
	
	// Initialize and enable interrupts
	SystemInterruptInitialize();
}

void SystemInitializeProgramsManager(void)
{
	// Start timers before everything else to make them count with a non-stable input clock, so the timer values are random
	T4CON = 0x04; // Start a fast timer (set a 1:1 prescaler and 1:1 postscaler)
	T0CON = 0xC7; // Start a slow timer (enable timer, configure it as 8 bits, use Fosc/4 as clock source, set a 1:256 prescaler)
	
	// Set oscillator frequency to 64MHz
	OSCCON = 0x78; // Core enters sleep mode when issuing a SLEEP instruction, select 16MHz frequency for high frequency internal oscillator, device is running from primary clock (set as "internal oscillator" in configuration registers)
	while (!OSCCONbits.HFIOFS); // Wait for the internal oscillator to stabilize
	OSCCON2 = 0x04; // Turn off secondary oscillator, enable primary oscillator drive circuit
	OSCTUNEbits.PLLEN = 1; // Enable 4x PLL
	
	// Initialize peripherals needed by the Programs Manager
	SystemKeyboardInitialize();
	SystemSerialPortInitialize();
	SystemDisplayInitialize();
	SystemExternalEEPROMInitialize();
	SystemLedInitialize();
}

void SystemExitProgram(void)
{
	// Disable interrupts to avoid perturbing the Programs Manager boot
	INTCONbits.GIE = 0;
	
	// Go to the Programs Manager entry point by resetting the core
	#asm
		reset
	#endasm
}
