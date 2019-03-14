#include <ANSIsACurse/characters.h>
#include <BareBonesReadline/readline.h>
#include <BareBonesHistory/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "exporting.h"
#include "aliasing.h"
#include "parse_commands.h"
#include "execute_command.h"
#include "startup.h"
#include "generate_ps1.h"
#include "argv.h"

#define MAX_CMD_LEN 5000
#define K_HOME_ENV "HOME"
#define K_PATH_ENV "PATH"
#define K_PS1_ENV "PS1"
#define HISTORY_FILE ".bnli_history"

int main(int argc, char const *argv[])
{	
	char* home_directory = getenv(K_HOME_ENV);
	char* history_filepath = calloc(strlen(home_directory) + strlen(HISTORY_FILE) + 2, sizeof(char));
	strcpy(history_filepath, home_directory);
	strcat(history_filepath, "/");
	strcat(history_filepath, HISTORY_FILE);

	init_argv();
	push_argv_frame(argv, argc);

	if (chdir(home_directory) != 0) {
		printf("Could not change directory!");
		exit(-1);
	}

	init_aliases();
	init_exports();

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
		const char** args = (const char**)parse_command(line);
		execute_command(args);
		if(args && *args){
			free(args);
		}
		if(line && *line){
			free(line);
		}
		export_history_to_file(history_filepath);
	}
	return 0;
}
