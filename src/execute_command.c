#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <err.h>
#include <stdint.h>
#include <assert.h>

#include "execute_command.h"
#include "script_handling.h"
#include "parse_commands.h"

// Does not parse or handle redirection stuff, should be handled beforehand.
// Currently takes a string containing the entire command text, which is parsed into argv.
// This functionality should probably be moved elsewhere.
void execute_command(const char** argv){
	if(!argv || !*argv){
		return;
	}
	if(file_exists(argv[0])){
		if(is_executable(argv[0])){
			execute_bin(argv[0], argv);
		}
		else{
			execute_shell_script(argv[0], argv);
		}
	}
	else{
		if(is_builtin(argv[0])){
			execute_builtin(argv[0], argv);
		}
		else{
			execute_bin(argv[0], argv);
		}
	}
}

void execute_builtin(const char* command, const char** argv){
	// temp
	int cid = is_builtin(command);
	switch(cid){
		case 1:
			break;
	}
}

void execute_shell_script(const char* filename, const char** argv){
	printf("Shell script argv support not implemented. Executing without args...\n");
	handle_script(filename);
}

void execute_bin(const char* filename, const char** argv){
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

int is_builtin(const char* command){
	return 0;
}

int is_executable(const char* filename){
	FILE* f = fopen(filename, "rb");
	if(!f){
		return 0;
	}
	uint32_t signature = 0;
	fread(&signature, 1, sizeof(uint32_t), f);
	fclose(f);
	#ifdef __MACH__
	return signature == SIGNATURE_MACH_O
		|| signature == SIGNATURE_MACH_O_REVERSE;
	#endif
	return signature == SIGNATURE_ELF
		|| signature == SIGNATURE_ELF_REVERSE;
}


int file_exists(const char* filename){
	FILE* f = fopen(filename, "rb");

	if(!f){
		return 0;
	}
	fclose(f);
	return 1;
}
