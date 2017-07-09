# Find all directories (each application stands in a separate directory). Search only in the current directory (do not go into subdirectories), display only the file name with leading "./" removed, ignore "." file
DIRECTORIES_LIST = $(sort $(shell find . -mindepth 1 -maxdepth 1 -type d -printf "%f "))
# Remove the System directory as it must be compiled in an other way
APPLICATIONS_LIST = $(filter-out System,$(DIRECTORIES_LIST))

# Tell "make" to not match the rule names with the existing folders
.PHONY: $(DIRECTORIES_LIST)

# Template for an application rules. Use "eval" operator to "instantiate" the template.
# @param $(1) The application directory.
define CREATE_APPLICATION_RULES =
$(1): System
	cd $(1) && $$(MAKE)

$(1)-clean:
	cd $(1) && $$(MAKE) clean

# TODO download
endef

# Compile all applications (there is no need to explicitly depend on System because all applications depend on it yet)
all: $(APPLICATIONS_LIST)
	@printf "\033[32mAll applications successfully built.\033[0m\n"

# Clean all applications and System too
clean:
	echo "TODO"

# System needs specific rules because it can't be downloaded
System:
	cd System && $(MAKE)

System-clean:
	cd System && $(MAKE) clean

# Automatically create all application rules
$(foreach Application_Name,$(APPLICATIONS_LIST),$(eval $(call CREATE_APPLICATION_RULES,$(Application_Name))))
