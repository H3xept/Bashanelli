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

		int pip_n = 0;
		const PipelineNode** pipeline_array = (const PipelineNode**) parse_command(&pip_n, line);
		if (pipeline_array) {
			execute_pipelines(pip_n, pipeline_array);
			if (pipeline_array)
				pn_array_destroy(pip_n, (PipelineNode**) pipeline_array);
			export_history_to_file(history_filepath);
		}

		if(line) free(line);
		free(ps1);
	}

	return 0;
}
