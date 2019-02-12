CC = gcc
CFLAGS = -Wall --pedantic --std=c99
SOURCE_FOLDER = src
OUT_FOLDER = bin
DEPENDENCIES_FOLDER = dependencies
SOURCE_FILES = $(SOURCE_FOLDER)/*.c
OUT_NAME = bashanelli

ANSI_IS_A_CURSE = ANSIsACurse
ANSI_IS_A_CURSE_LOCATION = ../$(ANSI_IS_A_CURSE)
ANSI_IS_A_CURSE_LOCATION_GIT = "https://github.com/H3xept/ANSIsACurse"

READLINE = BareBonesReadline
READLINE_LOCATION = ../$(READLINE)
READLINE_LOCATION_GIT = "https://github.com/H3xept/BareBonesReadline"

all: compile

compile: taget_dir prepare_dependencies
	@ echo "Compiling..."
	@ $(CC) -g -I$(DEPENDENCIES_FOLDER) $(CFLAGS) $(SOURCE_FILES) $(DEPENDENCIES_FOLDER)/libs/*.a -o $(OUT_FOLDER)/$(OUT_NAME)

prepare_dependencies: 
	@ if [ ! -d $(DEPENDENCIES_FOLDER)/$(ANSI_IS_A_CURSE) ]; then \
		make dep_ANSI; \
	fi;
	@ if [ ! -d $(DEPENDENCIES_FOLDER)/$(READLINE) ]; then \
		make dep_READLINE; \
	fi;

dep_ANSI:
	@ mkdir -p $(DEPENDENCIES_FOLDER)/$(ANSI_IS_A_CURSE)
	@ if [ -d $(ANSI_IS_A_CURSE_LOCATION) ]; then\
		echo "Folder found locally, running make..."; \
		echo "Making $$ANSI_IS_A_CURSE ... "; \
		make -C $(ANSI_IS_A_CURSE_LOCATION); \
		cp $(ANSI_IS_A_CURSE_LOCATION)/bin/headers/*.h $(DEPENDENCIES_FOLDER)/$(ANSI_IS_A_CURSE)/ ; \
		cp $(ANSI_IS_A_CURSE_LOCATION)/bin/libs/*.a $(DEPENDENCIES_FOLDER)/libs ; \
	else \
		echo "Fonder not found, cloning ..."; \
		git clone $(ANSI_IS_A_CURSE_LOCATION_GIT) $(ANSI_IS_A_CURSE); \
		echo "Making $$ANSI_IS_A_CURSE ..." ; \
		make -C $(ANSI_IS_A_CURSE); \
		cp $(ANSI_IS_A_CURSE)/bin/headers/*.h $(DEPENDENCIES_FOLDER)/$(ANSI_IS_A_CURSE)/ ; \
		cp $(ANSI_IS_A_CURSE)/bin/libs/*.a $(DEPENDENCIES_FOLDER)/libs ; \
		rm -rf $(ANSI_IS_A_CURSE); \
	fi;

dep_READLINE:
	@ mkdir -p $(DEPENDENCIES_FOLDER)/$(READLINE)
	@ if [ -d $(READLINE_LOCATION) ]; then\
		echo "Folder found locally, running make..."; \
		echo "Making $$READLINE ..."; \
		make -C $(READLINE_LOCATION); \
		cp $(READLINE_LOCATION)/bin/headers/*.h $(DEPENDENCIES_FOLDER)/$(READLINE)/ ; \
		cp $(READLINE_LOCATION)/bin/libs/*.a $(DEPENDENCIES_FOLDER)/libs ; \
	else \
		echo "Fonder not found, cloning ..."; \
		git clone $(READLINE_LOCATION_GIT) $(READLINE); \
		echo "Making $$READLINE ..." ; \
		make -C $(READLINE); \
		cp $(READLINE)/bin/headers/*.h $(DEPENDENCIES_FOLDER)/$(READLINE)/ ; \
		cp $(READLINE)/bin/libs/*.a $(DEPENDENCIES_FOLDER)/libs ; \
		rm -rf $(READLINE); \
	fi;

taget_dir:
	@ echo "Creating folder structure if needed..."
	@ mkdir -p $(OUT_FOLDER)
	@ mkdir -p $(DEPENDENCIES_FOLDER)
	@ mkdir -p $(DEPENDENCIES_FOLDER)/libs

test:
	@ echo "\033[1mInit testing for $(OUT_NAME)...\033[0m"
	@ sh test.sh
	@ echo "\033[32m\033[1mTesting complete!\033[0m"
	
clean:
	@ echo "Cleaning up..."
	@ rm -rf dependencies/
	@ rm -rf bin/
