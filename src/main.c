#include <ANSIsACurse/characters.h>
#include <BareBonesReadline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "exporting.h"
#include "aliasing.h"
#include "parse_commands.h"
#include "execute_command.h"
#include "startup.h"

#define MAX_CMD_LEN 5000
#define MAX_CWD_LEN 5000
#define K_HOME_ENV "HOME"
#define K_PATH_ENV "PATH"
#define K_PS1_ENV "PS1"

char* read_history() {
	return "ls\ncd \"lolz\"";
}

int main(int argc, char const *argv[])
{	
	char buffer[MAX_CMD_LEN];
	char* home_directory = getenv(K_HOME_ENV);
	char* env_path = getenv(K_PATH_ENV);
	char* history = NULL;

	if (chdir(home_directory) != 0) {
		// DIE
		printf("Could not change directory!");
		exit(-1);
	}

	history = read_history();
	init_aliases();
	init_exports();

	startup(argc, argv);
	int is_done = 0;
	init_readline(&is_done);

	char* cwd[MAX_CWD_LEN];
	while(!is_done) {
		getcwd(cwd, MAX_CWD_LEN);
		char* prompt[MAX_CWD_LEN + 1];
		sprintf(prompt, "%s>", cwd);
		char* line = read_line(prompt);
		char** args = parse_command(line);
		execute_command(args);
		if(args && *args){
			free(args);
		}
		if(line && *line){
			free(line);
		}
	}

	printf("Env: %s\n",home_directory);
	return 0;
}
