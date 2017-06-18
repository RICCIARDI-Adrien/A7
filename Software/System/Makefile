PATH_BINARIES = ../Binaries
PATH_INCLUDES = Includes
PATH_OBJECTS = Objects
PATH_SOURCES = Sources

BINARY_NAME = System.lpp

CC = xc8
CCFLAGS = --chip=18F26K22 --objdir=$(PATH_OBJECTS) --outdir=$(PATH_OBJECTS) --output=lpp -I$(PATH_INCLUDES)

SOURCES = $(PATH_SOURCES)/System.c $(PATH_SOURCES)/System_Display.c $(PATH_SOURCES)/System_External_EEPROM.c $(PATH_SOURCES)/System_Flash.c $(PATH_SOURCES)/System_Internal_EEPROM.c $(PATH_SOURCES)/System_Interrupt.c $(PATH_SOURCES)/System_Keyboard.c $(PATH_SOURCES)/System_Random.c $(PATH_SOURCES)/System_Serial_Port.c

all:
	$(CC) $(CCFLAGS) $(SOURCES) -O$(BINARY_NAME)
	@cp $(PATH_OBJECTS)/$(BINARY_NAME) $(PATH_BINARIES)
	
clean:
	rm -f $(PATH_OBJECTS)/* $(PATH_BINARIES)/$(BINARY_NAME)
	