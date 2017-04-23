PATH_OBJECTS = Objects
PATH_SOURCES = Sources

CC = xc8
CCFLAGS = --chip=18F26K22 --objdir=$(PATH_OBJECTS) --outdir=$(PATH_OBJECTS) 
#--output=lib

SOURCES = $(PATH_SOURCES)/System.c

all:
	$(CC) $(CCFLAGS) $(SOURCES) -OSystem.lib
	
clean:
	rm -f $(PATH_OBJECTS)/*
	