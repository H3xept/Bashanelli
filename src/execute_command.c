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
#include "builtins.h"

void execute_command(const char** argv){
	if(!argv || !*argv){
		return;
	}
	if(is_builtin(argv[0])){
		execute_builtin(argv[0], argv);
	}
	else{
		char* full_filepath = file_path(*argv);
		if(full_filepath){
			if(is_executable(full_filepath)){
				execute_bin(full_filepath, argv);
			}
			else{
				execute_shell_script(full_filepath, argv);
			}
			free(full_filepath);
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
			printf("%s: command not found\n", argv[0]);
		}
	}
}

void parse_and_execute_command(const char* command){
	char **argv = parse_command(command);
	execute_command(argv);
	if(argv ){
		int i = 0;
		while(*(argv+i)) {
			free(*(argv+i));
			i++;
		}
		free(argv);
	}
}

void execute_builtin(const char* command, const char** argv){
	exec_builtin_str(command, argv);
}

void execute_shell_script(const char* filename, const char** argv){
	//printf("Shell script argv support not implemented. Executing without args...\n");
	if(!filename){
		return;
	}
	if(!file_exists(filename)){
		printf("No such file: %s\n", filename);
		return;
	}
	if(is_executable(filename)){
		printf("Cannot run executable file as script: %s\n", filename);
		return;
	}
	pid_t pid = fork();
	if(!pid){
		handle_script(filename);
		exit(0);
	}
	else{
		waitpid(pid, NULL, 0);
	}
		
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
	return builtin_id(command) >= 0;
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
	char* t = calloc(100, sizeof(char));
	if(!f){
		return 0;
	}
	fclose(f);
	return 1;
}

// Returns the full path of a file present in a directory specified by the PATH env variable.
const char* file_path(const char* filename){
	for(int i = 0; i < strlen(filename); i++){
		if(*(filename + i) == '/'){
			return NULL;
		}
	}

	char* path_list = getenv("PATH");
	char* path_pointer = path_list;
	if(path_pointer){
		do{	
			int i = 0;
			while(*(path_pointer + i) != ':' && *(path_pointer + i) != '\0'){
				i++;
			}
			char* path = calloc(i + strlen(filename) + 1, sizeof(char));
			if(*(path_pointer + i) == ':'){
				*(path_pointer + i) = '\0';
				strcpy(path, path_pointer);
				*(path_pointer + i) = ':';
			}
			else{
				strcpy(path, path_pointer);
			}
			path_pointer++;
			*(path + i) = '/';
			*(path + i + 1) = '\0';
			strcat(path, filename);
			if(file_exists(path)){
				return path;
			}
			free(path);
			path_pointer = strchr(path_pointer, ':');
			if(!path_pointer){
				break;
			}
			path_pointer++;
		}
		while(*path_pointer);
	}
	return NULL;
}