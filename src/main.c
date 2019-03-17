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
#include <string.h>

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

#define die(MSG,ERR) { if (MSG) { puts(MSG); } exit(ERR); }
#define true_or_die(COND, MSG, ERR) { if(!(COND)) die(MSG, ERR); }

static void free_args(char** args) {
	if(!args) return;
	int i = 0;
	while(*(args+i)) {
		free((char*)*(args+i));
		i++;
	} free(args);
}

// Initialises the shell and runs the main loop, getting input, parsing and executing commands.
int main(int argc, char const *argv[])
{	
	char* home_directory = getenv(HOME_ENV);
	char* history_filepath = calloc(strlen(home_directory) + strlen(HISTORY_FILE) + 2, sizeof(char));
	strcpy(history_filepath, home_directory);
	strcat(history_filepath, "/");
	strcat(history_filepath, HISTORY_FILE);

	int is_done = 0;

	init_readline(&is_done);
	init_argv();
	init_aliases();
	init_exports();

	push_argv_frame(argv, argc);

	true_or_die(!chdir(home_directory), "Could not change to home directory!", -1);

	add_export(PWD_ENV, home_directory);

	if(!file_exists(history_filepath)){
		fclose(fopen(history_filepath, "w"));
	}

	import_history_from_file(history_filepath);
	
	startup(argc, argv);

	while(!is_done) {
		char* ps1 = generate_ps1();
		char* line = read_line(ps1);

		const char** args = (const char**) parse_command(line);
		execute_command(args);

		free_args((char**)args);

		export_history_to_file(history_filepath);
		
		if(line) free(line);
		free(ps1);
	}

	return 0;
}
