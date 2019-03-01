#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>
#include <stdint.h>
#define MAX_ARG_AMT 50

#include "parse_commands.h"

char** parse_command(char *command) {

	command = trim_whitespace(command);
	if(!*command){
		return 0;
	}

	char** argv = generate_argv(command);
	free(command);
	return argv;
}


	//seperate on ; recursively parse?
	//seperate into comm & args ("" and '' to contain literal args)
char **seperate_into_commands(char *command) {
		if(!*command){
		return NULL;
	}
	int argc = count_occ(command, ' ') + 1; //multiple spaces between args?
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

char** generate_argv(char* command){
	if(!*command){
		return NULL;
	}
	//int argc = count_occ(command, ' ') + 1; //multiple spaces between args?


	char *arg;
	int i = 0;
	char** argv = calloc(MAX_ARG_AMT, sizeof(char*));
	if(strchr(command,'"') && (strchr(command,'"') < strchr(command,' '))){
		
	} else {
		arg = strtok(command, " ");
	}

	while(arg != NULL){
		*(argv + i) = calloc(strlen(arg) + 1, sizeof(char));
		strcpy(*(argv + i), arg);
		arg = strtok(NULL, " ");
		i++;
	}
	*(argv + i) = NULL;
	//int i = 1;
	return argv;
}

char *get_quoted_arg(char *arg, char *endptr) {

}

int is_builtin(char* command){
	return 0;
}

int is_executable(char* filename){
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

int count_occ(char* str, char c){
	int occurences = 0;
	for(int i = 0; i < strlen(str); i++){
		occurences += *(str + i) == c;
	}
	return occurences;
}

int file_exists(char* filename){
	FILE* f = fopen(filename, "rb");
	if(!f){
		return 0;
	}
	fclose(f);
	return 1;
}

char* trim_whitespace(char* str){
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
