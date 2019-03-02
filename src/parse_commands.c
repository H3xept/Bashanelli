#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>
#include <stdint.h>
#define MAX_CMD_LEN 500
#define MAX_ARG_AMT 50
#define ARG_HAS_QUOTES(x) (  x!=NULL && strchr(x,'"') && ( (strchr(x,'"') < strchr(x,' ')) && (strchr(x,'"') < strchr(x,'\0')) )  )

#include "parse_commands.h"

//custom strtok for handling quoted args, ignoring spaces etc.
char *_get_quoted_arg(char *arg, char **endptr);

char *_get_unquoted_arg(char *arg, char **endptr);

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
	char *nextarg;
	char** argv = calloc(MAX_ARG_AMT, sizeof(char*));
	if(ARG_HAS_QUOTES(command)){
		arg = _get_quoted_arg(command, &nextarg);
	} else {
		arg = _get_unquoted_arg(command, &nextarg);
	}

	while(arg != NULL) {
		//*(argv + i) = calloc(strlen(arg) + 1, sizeof(char));
		*(argv + i) = arg;
		//strcpy(*(argv + i), arg);
		if(ARG_HAS_QUOTES(nextarg)){
			arg = _get_quoted_arg(nextarg, &nextarg);
		} else {
			arg = _get_unquoted_arg(nextarg, &nextarg);
		}
		i++;
	}
	*(argv + i) = NULL;
	//int i = 1;
	return argv;
}

char *_get_quoted_arg(char *arg, char **endptr) {
	if (!ARG_HAS_QUOTES(arg)){
		*endptr = NULL;
		return NULL;
	}
	char temp[MAX_CMD_LEN] = {0};
	int quotes = 0;
	int i = 0;
	int c = 0;
	while(quotes < 2 || !(arg[i] == ' ' || arg[i] == '\0')) {
		if(arg[i] != '"'){
			temp[c] = arg[i];
			c++;
		} else {
			quotes++;
		}
		i++;
	}
	while(arg[i] == ' ') {
		i++;
	}
	char *ret = calloc(c,sizeof(char));
	strcpy(ret,temp);
	*endptr = (arg + i);
	return ret; 
}

char *_get_unquoted_arg(char *arg, char **endptr) {
	if (!arg || !*arg){
		*endptr = NULL;
		return NULL;
	}

	char temp[MAX_CMD_LEN] = {0};
	int i = 0;
	while(!(arg[i] == ' ' || arg[i] == '\0')) {
		temp[i] = arg[i];
		i++;
	}
	char *ret = calloc(i+1,sizeof(char));
	strcpy(ret,temp);
	while(arg[i] == ' ') {
		i++;
	}
	*endptr = (arg + i);
	return ret; 
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
