# A7
A hand-made computer based on a 8-bit PIC18F26K22 microcontroller.

## Specifications

### CPU
* 64MHz PIC18 core (up to 16MIPS).
* 3896 RAM bytes.
* 32768 16-bit instruction words.

### Display
* 128x64 STN display with white pixels on blue backlight.
* 8-bit data interface with the microcontroller.

### Input
* Standard PS/2 keyboard support.
* Connect via standard PS/2 6-pin mini DIN connector.

### External storage
* On-board 128KB EEPROM (256-byte pages, 6ms maximum to write a page).
* Connected to a fast 4MHz SPI bus (less than 600us to read a full 256-byte page).

### Communication
* Standard RS-232 DB9 connector.
* Communication settings : 115200 bauds, no parity, one stop bit.

### Power supply
* Standard power supply jack.
* Input voltage from 7V to 24V with at least 200mA.
* Polarity reversal protection.

# Hardware
Schematics and PCB have been made with Eagle 7.5.  

![Mother board schematics](https://github.com/RICCIARDI-Adrien/A7/blob/master/Hardware/Mother_Board_Schematics.png)

![Mother board PCB](https://github.com/RICCIARDI-Adrien/A7/blob/master/Hardware/Mother_Board_PCB.png)

# Software
Use Microchip XC8 compiler to build software.  
You have to add the following environment variable for the build system to find the compiler-provided include files :
```
XC8_INCLUDES_PATH
```
The variable content must be the full path to the XC8 include directory. Below is an example for a Windows machine (don't forget to add quotation marks if the path contains spaces) :
```
"C:\Program Files\Microchip\xc8\v1.42\include"
```
  
In order to download applications on the A7, you have to set the following environment variable with the serial port device used to communicate with the A7 (like /dev/ttyS0, /dev/ttyUSB0...) :
```
A7_APPLICATION_DOWNLOAD_SERIAL_PORT_DEVICE
```

# Casing
Casing panels have been drawn with LibreCAD.

![Casing panels](https://github.com/RICCIARDI-Adrien/A7/blob/master/Resources/Casing/All_Panels.png)
