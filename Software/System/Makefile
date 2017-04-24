PATH_BINARIES = ../Binaries
PATH_INCLUDES = Includes
PATH_OBJECTS = Objects
PATH_SOURCES = Sources

CC = xc8
CCFLAGS = --chip=18F26K22 --objdir=$(PATH_OBJECTS) --outdir=$(PATH_OBJECTS) --output=lpp -I$(PATH_INCLUDES)

SOURCES = $(PATH_SOURCES)/System.c $(PATH_SOURCES)/System_Serial_Port.c

all:
	$(CC) $(CCFLAGS) $(SOURCES) -OSystem.lpp
	@cp $(PATH_OBJECTS)/System.lpp $(PATH_BINARIES)
	
clean:
	rm -f $(PATH_OBJECTS)/* $(PATH_BINARIES)/System.lpp
	