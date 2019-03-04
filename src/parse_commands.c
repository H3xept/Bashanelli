#include <ANSIsACurse/cursor.h>
#include <BareBonesReadline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>
#include <stdint.h>
#define MAX_CMD_LEN 500
#define MAX_ARG_AMT 50
#define ARG_HAS_QUOTES(x) ((x!=NULL && (strchr(x,'"')!=NULL)))

#include "parse_commands.h"

//custom strtok for handling quoted args, ignoring spaces etc.
char *_get_arg(char *arg, char **endptr);

char** parse_command(char *command) {

	command = trim_whitespace(command);
	ignore_comment(command);
	if(!*command){
		return 0;
	}
	//read_line("Shellerino Diocanarino$ ");
	char** argv = generate_argv(command);
	free(command);
	return argv;
}

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
	// //int argc = count_occ(command, ' ') + 1; //multiple spaces between args?
	char *arg;
	int i = 0;
	char *nextarg;
	char** argv = calloc(MAX_ARG_AMT, sizeof(char*));
	arg = _get_arg(command,&nextarg);
	while(arg != NULL) {
	 	*(argv + i) = arg;
		arg = _get_arg(nextarg,&nextarg);
	 	i++;
	}
	*(argv + i) = NULL;
	return argv;
}

char *_get_arg(char *arg, char **endptr) {
	if (!arg || !*arg){
		*endptr = NULL;
		return NULL;
	}

	char temp[MAX_CMD_LEN] = {0};
	int i = 0;
	int c = 0;
	int inq = 0;
	int indq = 0;
	int cont = 1;
	while(cont) {
		
		switch(arg[i]) {
			case '\\':
				i++;
				temp[c] = arg[i];
				c++;
				break;
			case '"':
				inq = (inq) ? 0 : 1;
				break;
			case ' ':
				cont = (inq) ? 1 : 0;
				if(inq){
					temp[c] = arg[i];
					c++;
				}
				break;
			case '\0':
				cont = (inq) ? 1 : 0;
				break;
			default:
				temp[c] = arg[i];
				c++;
				break;
		}
		i++;
	}
	char *ret = calloc(c,sizeof(char));
	strcpy(ret,temp);
	while(arg[i] == ' ') {
		i++;
	}
	*endptr = (arg + i);
	return ret; 
}

int count_occ(char* str, char c){
	int occurences = 0;
	for(int i = 0; i < strlen(str); i++){
		occurences += *(str + i) == c;
	}
	return occurences;
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

void ignore_comment(char *line) {

	if (*line == '#'){
		*line = '\0';
	}

	char *comment = strchr(line,'#'); 
	if (comment && *(comment-1) == ' '){
		*(comment-1) = '\0';
	}
}
