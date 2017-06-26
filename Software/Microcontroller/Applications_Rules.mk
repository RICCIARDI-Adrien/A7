# Gather variables shared by all applications.
export APPLICATIONS_RULES_PATH_BINARIES = ../../Binaries

export APPLICATIONS_RULES_C_COMPILER = xc8
export APPLICATIONS_RULES_C_COMPILER_FLAGS = --chip=18F26K22 -I../System/Includes -I$(XC8_INCLUDES_PATH) $(APPLICATIONS_RULES_PATH_BINARIES)/System.lpp --codeoffset=0x2000
