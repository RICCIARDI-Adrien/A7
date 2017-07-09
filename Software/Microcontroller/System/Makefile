PATH_OBJECTS = Objects
PATH_SOURCES = Sources

BINARY_NAME = System.lpp
MAIN_MAKEFILE_C_COMPILER_FLAGS += --objdir=$(PATH_OBJECTS) --outdir=$(PATH_OBJECTS) --output=lpp

SOURCES = $(PATH_SOURCES)/System.c $(PATH_SOURCES)/System_Display.c $(PATH_SOURCES)/System_External_EEPROM.c $(PATH_SOURCES)/System_Flash.c $(PATH_SOURCES)/System_Internal_EEPROM.c $(PATH_SOURCES)/System_Interrupt.c $(PATH_SOURCES)/System_Keyboard.c $(PATH_SOURCES)/System_Random.c $(PATH_SOURCES)/System_Serial_Port.c

all:
	$(MAIN_MAKEFILE_C_COMPILER) $(MAIN_MAKEFILE_C_COMPILER_FLAGS) $(SOURCES) -O$(BINARY_NAME)
	@cp $(PATH_OBJECTS)/$(BINARY_NAME) $(MAIN_MAKEFILE_PATH_BINARIES)
	
clean:
	rm -f $(PATH_OBJECTS)/* $(MAIN_MAKEFILE_PATH_BINARIES)/$(BINARY_NAME)
	