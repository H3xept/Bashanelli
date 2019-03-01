#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>
#include <stdint.h>

#include "execute_command.h"
#include "script_handling.h"
#include "parse_commands.h"


// Does not parse or handle redirection stuff, should be handled beforehand.
// Currently takes a string containing the entire command text, which is parsed into argv.
// This functionality should probably be moved elsewhere.
void execute_command(char* command){

	char **argv = parse_command(command);
	command = *argv;
	if(file_exists(command)){
		if(is_executable(command)){
			execute_bin(command, argv);
		}
		else{
			execute_shell_script(command, argv);
		}
	}
	else{
		if(is_builtin(command)){
			execute_builtin(command, argv);
		}
		else{
			execute_bin(command, argv);
		}
	}
	char** argv_base = argv;
	while(*argv){
		free((*argv++));
	}
	free(argv_base);
}

void execute_builtin(char* command, char** argv){
	// temp
	int cid = is_builtin(command);
	switch(cid){
		case 1:
			break;
	}
}

void execute_shell_script(char* filename, char** argv){
	warn("Shell script argv support not implemented. Executing without args...");
	handle_script(filename);
}

void execute_bin(char* filename, char** argv){
	pid_t pid = fork();
	if(!pid){
		if(execvp(filename, argv) == -1){
			printf("%s: command not found\n", filename);
			fflush(stdout);
		};
		exit(0);
	}
	else{
		waitpid(pid, NULL, 0);
	}
}



