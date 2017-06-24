include ../Applications_Rules.mk

PATH_INCLUDES = Includes
PATH_OBJECTS = Objects
PATH_SOURCES = Sources

BINARY_NAME = Programs_Manager.hex
# Application relocation address in flash memory
APPLICATION_BASE_ADDRESS = 0x2000
# Classic APPLICATIONS_RULES_C_COMPILER_FLAGS can't be used because it contains the code base address redefinition
# -P option allows assembly files to be parsed by the C preprocessor
CCFLAGS = --chip=18F26K22 --objdir=$(PATH_OBJECTS) --outdir=$(PATH_OBJECTS) -I../System/Includes -I$(PATH_INCLUDES) $(APPLICATIONS_RULES_PATH_BINARIES)/System.lpp -DCONFIGURATION_APPLICATION_BASE_ADDRESS=$(APPLICATION_BASE_ADDRESS) -P

SOURCES = $(PATH_SOURCES)/File_System.c $(PATH_SOURCES)/Interrupt_Vector_Relocation.asm $(PATH_SOURCES)/Main.c $(PATH_SOURCES)/Menu.c $(PATH_SOURCES)/Menu_Delete_File.c $(PATH_SOURCES)/Menu_Download_File.c $(PATH_SOURCES)/Menu_Start_Program.c

all:
	$(APPLICATIONS_RULES_C_COMPILER) $(CCFLAGS) $(SOURCES) -O$(BINARY_NAME)
	@cp $(PATH_OBJECTS)/$(BINARY_NAME) $(APPLICATIONS_RULES_PATH_BINARIES)
	
clean:
	rm -f $(PATH_OBJECTS)/* $(APPLICATIONS_RULES_PATH_BINARIES)/$(BINARY_NAME)