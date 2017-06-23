/** @file System_Interrupt.h
 * Gather all interrupt vectors and redirect peripheral interrupts to their handlers.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_INTERRUPT_H
#define H_SYSTEM_INTERRUPT_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize microcontroller interrupts module and enable them. */
void SystemInterruptInitialize(void);

#endif