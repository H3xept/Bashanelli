/**
 * File: main.c
 *
 * Description: Initialises the shell,
 * runs the main loop, and tears down on exit.
 * 
 * Date: 
 * 15/3/2019
 * 
 * Authors: 
 *	1. Leonardo Cascianelli (me.leonardocascianelli@gmail.com)
 *	2. Rory Brown (rorybrown660@gmail.com)
 *	3. Ewan Skene (ewancskene@gmail.com)
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ANSIsACurse/characters.h>
#include <BareBonesReadline/readline.h>
#include <BareBonesHistory/history.h>

#include "exporting.h"
#include "aliasing.h"
#include "parse_commands.h"
#include "execute_command.h"
#include "startup.h"
#include "generate_ps1.h"
#include "argv.h"
#include "constants.h"

// Initialises the shell and runs the main loop, getting input, parsing and executing commands.
int main(int argc, char const *argv[])
{	
	char* home_directory = getenv(HOME_ENV);
	char* history_filepath = calloc(strlen(home_directory) + strlen(HISTORY_FILE) + 2, sizeof(char));
	strcpy(history_filepath, home_directory);
	strcat(history_filepath, "/");
	strcat(history_filepath, HISTORY_FILE);

	init_argv();
	push_argv_frame(argv, argc);

	if (chdir(home_directory) != 0) {
		printf("Could not change to home directory!");
		exit(-1);
	}

	init_aliases();
	init_exports();

	add_export(PWD_ENV, home_directory);

	int is_done = 0;
	init_readline(&is_done);

	if(!file_exists(history_filepath)){
		fclose(fopen(history_filepath, "w"));
	}
	import_history_from_file(history_filepath);
	
	startup(argc, argv);

	while(!is_done) {
		char* ps1 = generate_ps1();
		char* line = read_line(ps1);
		free(ps1);

		const char** args = (const char**) parse_command(line);
		execute_command(args);

		if(args){
			int i = 0;
			while(*(args+i)) {
				free(*(args+i));
				i++;
			}
			free(args);
		}

		if(line && *line){
			free(line);
		}

		export_history_to_file(history_filepath);
	}

	return 0;
}
