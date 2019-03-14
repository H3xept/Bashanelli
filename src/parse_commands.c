//#include <ANSIsACurse/cursor.h>
#include <BareBonesReadline/readline.h>
#include <BareBonesReadline/tokenizer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>
#include <stdint.h>
#include <assert.h>
#include <BareBonesReadline/string_utils.h>

#include "parse_commands.h"
#include "exporting.h"
#include "aliasing.h"
#include "argv.h"

#define MAX_CMD_LEN 10000
#define MAX_ARG_AMT 50

static char *get_arg(const char *arg, const char **endptr);

char** parse_command(const char *command) {
	if(!command || !*command){
		return 0;
	}
	char *uncommented = ignore_comment(command);
	#warning TEMPORARY FIX FOR PARSE_LINE NOT LIKING NULL
	char** argv;
	if (uncommented){
		char *export_expanded = expand_exvar(uncommented);
		char *alias_expanded = expand_alias(export_expanded);
		char* cmd_whitespace = trim_whitespace(alias_expanded);
		argv = generate_argv(cmd_whitespace);
		free(uncommented);
		free(export_expanded);	
		free(alias_expanded);
		free(cmd_whitespace);		
	}
	else{
		return NULL;
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
	const char* ret = str;
	const char* end = str + strlen(str)-1;
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
	const char* nextarg = NULL;
	char** argv = calloc(MAX_ARG_AMT, sizeof(char*));
	arg = get_arg(line,&nextarg);
	while(arg != NULL) {
		*(argv + i) = calloc(strlen(arg) + 1, sizeof(char));
		strcpy(*(argv + i), arg);
		arg = get_arg(nextarg, &nextarg);
	 	i++;
	}
	*(argv + i) = NULL;
	argv = realloc(argv,(i+1)*sizeof(char*));
	if(line){
		free(line);
	}
	return argv;
}

//custom strtok for handling quoted args, ignoring spaces etc.
static char *get_arg(const char *arg, const char **endptr) {
	if (!arg || !*arg){
		// *endptr = NULL;
		return NULL;
	}

	char temp[MAX_CMD_LEN] = {0};
	int i = 0;
	int c = 0;
	int inq = 0;
	int cont = 1;
	while(cont) {
		switch(arg[i]) {
			case '\\':
				if(arg[i+1] == '\0'){
					break;
				}
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
				cont = 0;
				break;
			default:
				temp[c] = arg[i];
				c++;
				break;
		}
		i++;
	}
	char *ret = (!c) ? NULL : calloc(c + 1,sizeof(char));
	if(ret){
		strcpy(ret,temp);
	}
	while(arg[i] == ' ') {
		i++;
	}
	*endptr = (arg + i);
	return ret; 
}

char *expand_alias(const char *line){
	const char *endptr;
	char *alias = get_arg(line, &endptr);
	char *expanded = resolve_alias(alias);
	char *ret;
	if(expanded){
		size_t bodylength = (!(endptr || *endptr)) ? 0 : strlen(endptr); 
		ret = calloc(strlen(expanded) + bodylength + 2, sizeof(char));
		strcat(ret, expanded);
		if(bodylength){
			strcat(ret," ");
			strcat(ret, endptr);
		}
		return ret;
	}
	else {
		ret = calloc(strlen(line)+1, sizeof(char));
		strcpy(ret, line);
		return ret;
	}	
}

char *expand_exvar(const char *line){
	if(!line || !*line){
		return NULL;
	}
	char ln[MAX_CMD_LEN] = {0};
	strcpy(ln, line); 
	char tmp[MAX_CMD_LEN] = {0};
	char *dp;
	char *var;
	//char *current = get_arg(line, &next);
	//I'm sorry leo. . . don't look, skip the next line, it doesn't exist.
	Tokenizer* tok = new_tokenizer(ln, ' ');
	char* current = next_token(tok);
	while(current){
		dp = strchr(current, '$');
		if(dp){
			if(dp > current){
				strncat(tmp, current, (dp - current));
			}
			int arg = (int)strtol(dp + 1, NULL, 10);
			if(arg || *(dp + 1) == '0'){
				char* indexed_arg = get_arg_from_current_argv(arg);
				strcat(tmp, indexed_arg);
			}
			else{
				var = get_export_value(dp+1);
				if(var){
					strcat(tmp, var);
				}
				else{
					strcat(tmp, " ");
				}
			}
			// printf(";%x;", arg);
			
		}
		else {
			strcat(tmp, current);
		}
		strcat(tmp, " ");
		//current =  get_arg(next, &next);
		free(current);
		current = next_token(tok);
	}
	destroy_tokenizer(tok);
	char *ret = calloc(strlen(tmp) + 1, sizeof(char));
	strcpy(ret, tmp);
	return ret;
}

