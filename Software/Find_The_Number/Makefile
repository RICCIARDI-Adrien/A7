PATH_BINARIES = ../Binaries
PATH_OBJECTS = Objects
PATH_SOURCES = Sources

CC = xc8
CCFLAGS = --chip=18F26K22 --objdir=$(PATH_OBJECTS) --outdir=$(PATH_OBJECTS) -I../System/Includes $(PATH_BINARIES)/System.lpp

SOURCES = $(PATH_SOURCES)/Main.c

all:
	$(CC) $(CCFLAGS) $(SOURCES)
	@cp $(PATH_OBJECTS)/Find_The_Number.hex $(PATH_BINARIES)
	
clean:
	rm -f $(PATH_OBJECTS)/* $(PATH_BINARIES)/Find_The_Number.hex