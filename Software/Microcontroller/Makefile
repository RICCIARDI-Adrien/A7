# Make some shared variables available to application makefiles
# Following paths can be used only from the application itself
export MAIN_MAKEFILE_APPLICATION_PATH_INCLUDES = Includes
export MAIN_MAKEFILE_APPLICATION_PATH_OBJECTS = Objects
export MAIN_MAKEFILE_APPLICATION_PATH_SOURCES = Sources
export MAIN_MAKEFILE_PATH_BINARIES = ../../Binaries
export MAIN_MAKEFILE_C_COMPILER = xc8
export MAIN_MAKEFILE_C_COMPILER_FLAGS = --chip=18F26K22 --objdir=$(MAIN_MAKEFILE_APPLICATION_PATH_OBJECTS) --outdir=$(MAIN_MAKEFILE_APPLICATION_PATH_OBJECTS) -I../System/Includes -I$(XC8_INCLUDES_PATH) -I$(MAIN_MAKEFILE_APPLICATION_PATH_INCLUDES) $(MAIN_MAKEFILE_PATH_BINARIES)/System.lpp --codeoffset=0x2000

# Find all directories (each application stands in a separate directory). Search only in the current directory (do not go into subdirectories), display only the file name with leading "./" removed, ignore "." file
DIRECTORIES_LIST = $(sort $(shell find . -mindepth 1 -maxdepth 1 -type d -printf "%f "))
# Remove directories that do not belong in the regular applications category
APPLICATIONS_LIST = $(filter-out System,$(DIRECTORIES_LIST))

# Tell "make" to not match the rule names with the existing folders
.PHONY: $(DIRECTORIES_LIST)

# Template for an application rules. Use "eval" operator to "instantiate" the template.
# @param $(1) The application directory.
define CREATE_APPLICATION_RULES =
$(1): System
	@printf "\033[33mBuilding $(1)...\033[0m\n"
	@# Make sure objects directory is present
	@mkdir -p $(1)/$(MAIN_MAKEFILE_APPLICATION_PATH_OBJECTS)
	cd $(1) && $$(MAKE)

$(1)-clean:
	@# Automatically remove the objects directory
	@rm -rf $(1)/$(MAIN_MAKEFILE_APPLICATION_PATH_OBJECTS)
	cd $(1) && $$(MAKE) clean

# Programs Manager can't be downloaded as it resides in another memory area than classic applications
ifneq ("$(1)","Programs_Manager")
$(1)-download:
	cd ../PC/A7_Upload_Application && $(MAKE)
	../Binaries/A7_Upload_Application $(A7_APPLICATION_DOWNLOAD_SERIAL_PORT_DEVICE) ../Binaries/$(1).hex '$(shell cat $(1)/Application_Name_On_A7.txt)'
endif
endef

all: $(APPLICATIONS_LIST)
	@printf "\033[32mSystem and all applications successfully built.\033[0m\n"

# Clean everything (it works by modifying all words from the applications list to append "-clean" at their end)
clean: $(APPLICATIONS_LIST:=-clean) System-clean
	@printf "\033[32mSystem and all applications successfully cleaned.\033[0m\n"

# System needs specific rules because it can't be downloaded and its build process is slightly different
System:
	cd System && $(MAKE)

System-clean:
	cd System && $(MAKE) clean

# Automatically create all application rules
$(foreach Application_Name,$(APPLICATIONS_LIST),$(eval $(call CREATE_APPLICATION_RULES,$(Application_Name))))
