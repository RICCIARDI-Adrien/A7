/** @file System_Serial_Port.h
 * Provide a full-duplex 115200 8N1 serial port using EUSART module 2.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_SERIAL_PORT_H
#define H_SYSTEM_SERIAL_PORT_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the EUSART module for required operations. */
void SystemSerialPortInitialize(void);

/** Send a single byte of data through the serial port.
 * @param Data The byte to send.
 */
void SystemSerialPortWriteByte(unsigned char Data);

/** Send an ASCIIZ string through the serial port.
 * @param String The string to send.
 */
void SystemSerialPortWriteString(const unsigned char *String);

#endif