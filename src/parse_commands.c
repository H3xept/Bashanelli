//#include <ANSIsACurse/cursor.h>
#include <BareBonesReadline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>
#include <stdint.h>
#include <assert.h>

#include "parse_commands.h"

#define MAX_CMD_LEN 500
#define MAX_ARG_AMT 50

static char *get_arg(char *arg, char **endptr);

char** parse_command(const char *command) {
	if(!command || !*command){
		return 0;
	}
	char* cmd_whitespace = trim_whitespace(command);
	char *uncommented = ignore_comment(cmd_whitespace);
	if(cmd_whitespace){
		free(cmd_whitespace);
	}
	#warning TEMPORARY FIX FOR PARSE_LINE NOT LIKING NULL
	char** argv;
	if (uncommented){
		char* parsed_command = parse_line(uncommented);
		if(uncommented) {
			free(uncommented);
		}
		argv = generate_argv(parsed_command);
		if(parsed_command){
			free(parsed_command);	
		}
	}
	else{
		argv = generate_argv(uncommented);
		if(uncommented){
			free(uncommented);	
		}
	}

	return argv;
}

// char **seperate_into_commands(const char *command) {
// 		if(!*command){
// 		return NULL;
// 	}
// 	int argc = count_occ(command, ' ') + 1; //multiple spaces between args?
// 	char** argv = calloc(argc + 1, sizeof(char*));
// 	char* arg = strtok(command, " ");
// 	for(int i = 0; i < argc; i++){
// 		*(argv + i) = calloc(strlen(arg) + 1, sizeof(char));
// 		strcpy(*(argv + i), arg);
// 		arg = strtok(NULL, " ");
// 	}
// 	*(argv + argc) = NULL;
// 	int i = 1;
// 	return argv;
// }

int count_occ(const char* str, const char c){
	int occurences = 0;
	for(int i = 0; i < strlen(str); i++){
		occurences += *(str + i) == c;
	}
	return occurences;
}

char* trim_whitespace(const char* str){
	if(!str || !*str){
		return NULL;
	}
	char* ret = str;
	char* end = str + strlen(str)-1;
	while(*ret == ' ' || *ret == '\n'){
		ret++;
	}
	while((*(end) == ' ' || *(end) == '\n') && end >= ret){
		end--;
	}
	if(ret > end){
		return NULL;
	}
	else if (ret == end) {
		char* ret_allocated = calloc(strlen(str)+1, sizeof(char));
		strcpy(ret_allocated, str);
		return ret_allocated;
	}
	else {
		assert(end-ret);
		char* ret_allocated = calloc((end - ret) + 2, sizeof(char));
		strncpy(ret_allocated, ret, (end - ret)+1);
		assert(ret_allocated);
		return ret_allocated;
	}

}

char *ignore_comment(const char *line) {
	if(!line || !*line){
		return NULL;
	}
	if (*line == '#'){
		return NULL;
	}
	char *comment = strchr(line,'#');
	char *ret = calloc(strlen(line)+1, sizeof(char));
	strcpy(ret, line); 
	if (comment && *(comment-1) == ' ') {
		assert(strlen(line)-strlen(comment) > 0);
		ret = realloc(ret, (strlen(line)-strlen(comment))*sizeof(char));
		*(ret+(strlen(line)-strlen(comment))) = '\0';
	}
	return ret;
}

char** generate_argv(char* command){
	if(!command || !*command){
	 	return NULL;
	}
	char *line = calloc(strlen(command) + 1,sizeof(char*));
	strcpy(line,command);
	char *arg;
	int i = 0;
	char *nextarg;
	char** argv = calloc(MAX_ARG_AMT, sizeof(char*));
	arg = get_arg(line,&nextarg);
	while(arg != NULL) {
		*(argv + i) = calloc(strlen(arg) + 1, sizeof(char));
		strcpy(*(argv + i), arg);
		arg = get_arg(nextarg,&nextarg);
	 	i++;
	}
	*(argv + i) = NULL;
	argv = realloc(argv,(i+1)*sizeof(char*));
	return argv;
}

//custom strtok for handling quoted args, ignoring spaces etc.
static char *get_arg(char *arg, char **endptr) {
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
	char *ret = (!c) ? NULL : calloc(c,sizeof(char));
	if(ret){
		strcpy(ret,temp);
	}
	while(arg[i] == ' ') {
		i++;
	}
	*endptr = (arg + i);
	return ret; 
}
