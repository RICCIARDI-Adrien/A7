; Relocate interrupt vectors to the application memory space

PSECT Relocated_Interrupt_Vectors,abs,ovrld ; Make the PSECT absolute to force the address to start at 0
	ORG 0x08
	goto 0x08 + CONFIGURATION_APPLICATION_BASE_ADDRESS
	
	ORG 0x18
	goto 0x18 + CONFIGURATION_APPLICATION_BASE_ADDRESS