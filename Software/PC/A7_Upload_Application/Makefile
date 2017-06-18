PATH_BINARIES = ../../Binaries

BINARY = $(PATH_BINARIES)/A7_Upload_Application

# Automatically list all source files
SOURCES = $(shell find . -iname "*.c")

CC = gcc
CCFLAGS = -W -Wall -IHex_Parser/Includes -ISerial_Port_Library/Includes

all:
	$(CC) $(CCFLAGS) $(SOURCES) -o $(BINARY)

clean:
	rm -f $(BINARY) $(BINARY).exe
	