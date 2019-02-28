#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>
#include <stdint.h>

#include "execute_command.h"
#include "script_handling.h"

int _is_builtin(const char* command);
int _is_executable(const char* command);
int _count_occ(const char* str, char c);
char* _trim_whitespace(const char* str);
char** _generate_argv(const char* command);
int _file_exists(const char* filename);

// Does not parse or handle redirection stuff, should be handled beforehand.
// Currently takes a string containing the entire command text, which is parsed into argv.
// This functionality should probably be moved elsewhere.
void execute_command(const char* command){
	//added by ewan - it previously died when nullptr passed in
	if(!command){
		return;
	}
	command = _trim_whitespace(command);
	if(!*command){
		return;
	}
	char** argv = _generate_argv(command);
	free(command);
	command = *argv;
	if(_file_exists(command)){
		if(_is_executable(command)){
			execute_bin(command, argv);
		}
		else{
			execute_shell_script(command, argv);
		}
	}
	else{
		if(_is_builtin(command)){
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

void execute_builtin(const char* command, const char** argv){
	// temp
	int cid = _is_builtin(command);
	switch(cid){
		case 1:
			break;
	}
}

void execute_shell_script(const char* filename, const char** argv){
	warn("Shell script argv support not implemented. Executing without args...");
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

char** _generate_argv(const char* command){
	if(!*command){
		return NULL;
	}
	int argc = _count_occ(command, ' ') + 1;
	char** argv = calloc(argc + 1, sizeof(char*));
	char* arg = strtok(command, " ");
	for(int i = 0; i < argc; i++){
		*(argv + i) = calloc(strlen(arg) + 1, sizeof(char));
		strcpy(*(argv + i), arg);
		arg = strtok(NULL, " ");
	}
	*(argv + argc) = NULL;
	int i = 1;
	return argv;
}

int _is_builtin(const char* command){
	return 0;
}

int _is_executable(const char* filename){
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

int _count_occ(const char* str, const char c){
	int occurences = 0;
	for(int i = 0; i < strlen(str); i++){
		occurences += *(str + i) == c;
	}
	return occurences;
}

int _file_exists(const char* filename){
	FILE* f = fopen(filename, "rb");
	if(!f){
		return 0;
	}
	fclose(f);
	return 1;
}

char* _trim_whitespace(const char* str){
	char* ret = str;
	while(*ret == ' ' || *ret == '\n'){
		ret++;
	}
	while(*(ret + strlen(ret) - 1) == ' ' || *(ret + strlen(ret) - 1) == '\n'){
		*(ret + strlen(ret) - 1) = '\0';
	}
	char* ret_allocated = calloc(strlen(ret) + 1, sizeof(char));
	strcpy(ret_allocated, ret);
	return ret_allocated;
}
