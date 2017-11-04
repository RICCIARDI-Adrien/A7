# Make some shared variables available to application makefiles
export MAIN_MAKEFILE_PATH_BINARIES = ../../Binaries
export MAIN_MAKEFILE_C_COMPILER = xc8
export MAIN_MAKEFILE_C_COMPILER_FLAGS = --chip=18F26K22 -I../System/Includes -I$(XC8_INCLUDES_PATH) $(MAIN_MAKEFILE_PATH_BINARIES)/System.lpp --codeoffset=0x2000

# Find all directories (each application stands in a separate directory). Search only in the current directory (do not go into subdirectories), display only the file name with leading "./" removed, ignore "." file
DIRECTORIES_LIST = $(sort $(shell find . -mindepth 1 -maxdepth 1 -type d -printf "%f "))
# Remove directories that do not belong in the regular applications category
APPLICATIONS_LIST = $(filter-out Programs_Manager System,$(DIRECTORIES_LIST))

# Tell "make" to not match the rule names with the existing folders
.PHONY: $(DIRECTORIES_LIST)

# Template for an application rules. Use "eval" operator to "instantiate" the template.
# @param $(1) The application directory.
define CREATE_APPLICATION_RULES =
$(1): System
	@# Make sure objects directory is present
	@mkdir -p $(1)/Objects
	cd $(1) && $$(MAKE)

$(1)-clean:
	cd $(1) && $$(MAKE) clean

$(1)-download: $(1)
	cd ../PC/A7_Upload_Application && $(MAKE)
	../Binaries/A7_Upload_Application $(A7_APPLICATION_DOWNLOAD_SERIAL_PORT_DEVICE) ../Binaries/$(1).hex $(shell cat $(1)/Application_Name_On_A7.txt)
endef

# Compile all applications (there is no need to explicitly depend on System because all applications depend on it yet)
all: $(APPLICATIONS_LIST)
	@printf "\033[32mAll applications successfully built.\033[0m\n"

# Clean all applications and System too
clean:
	echo "TODO"

# System and Programs Manager need specific rules because they can't be downloaded and System build is slightly different
# Programs Manager must be explicitly compiled, it is not automatically compiled when executing "all" rule
Programs_Manager: System
	cd Programs_Manager && $(MAKE)

Programs_Manager-clean:
	cd Programs_Manager && $(MAKE) clean

System:
	cd System && $(MAKE)

System-clean:
	cd System && $(MAKE) clean

# Automatically create all application rules
$(foreach Application_Name,$(APPLICATIONS_LIST),$(eval $(call CREATE_APPLICATION_RULES,$(Application_Name))))
