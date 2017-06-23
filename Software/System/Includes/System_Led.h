/** @file System_Led.h
 * Control the front panel led.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_LED_H
#define H_SYSTEM_LED_H

#include <xc.h>

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Configure led pin as output. */
#define SystemLedInitialize() \
{ \
	LATCbits.LATC0 = 0; \
	TRISCbits.TRISC0 = 0; \
}	

/** Turn led on. */
#define SystemLedOn() LATCbits.LATC0 = 1

/** Turn led off. */
#define SystemLedOff() LATCbits.LATC0 = 0

#endif
