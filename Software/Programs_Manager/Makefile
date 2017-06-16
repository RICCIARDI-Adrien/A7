PATH_BINARIES = ../Binaries
PATH_INCLUDES = Includes
PATH_OBJECTS = Objects
PATH_SOURCES = Sources

BINARY_NAME = Programs_Manager.hex
# Program relocation address in flash memory
STARTING_ADDRESS = 0xE000

CC = xc8
CCFLAGS = --chip=18F26K22 --objdir=$(PATH_OBJECTS) --outdir=$(PATH_OBJECTS) -I../System/Includes -I$(PATH_INCLUDES) $(PATH_BINARIES)/System.lpp --codeoffset=$(STARTING_ADDRESS) -DSTARTING_ADDRESS=$(STARTING_ADDRESS)

SOURCES = $(PATH_SOURCES)/File_System.c $(PATH_SOURCES)/Main.c $(PATH_SOURCES)/Menu.c $(PATH_SOURCES)/Menu_Download_File.c

all:
	$(CC) $(CCFLAGS) $(SOURCES) -O$(BINARY_NAME)
	@cp $(PATH_OBJECTS)/$(BINARY_NAME) $(PATH_BINARIES)
	
clean:
	rm -f $(PATH_OBJECTS)/* $(PATH_BINARIES)/$(BINARY_NAME)