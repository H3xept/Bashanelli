#include <stdio.h>
#include <string.h>

#include "builtins.h"


static void builtin_cd(const char** argv);

static char* builtins_list[] = {"cd"};

int builtin_id(const char* command){
	for(int i = 0; i < NUM_BUILTINS; i++){
		if(!strcmp(command,*(builtins_list + i))){
			return i;
		}
	}
	return -1;
}

void exec_builtin_str(const char* command, const char** argv){
	exec_builtin_id(builtin_id(command), argv);
}

void exec_builtin_id(const int id, const char** argv){
	switch(id){
		case 0:
			builtin_cd(argv);
			break;
		default:
			printf("Builtin id %d not found.", id);
	}
}

static void builtin_cd(const char** argv){
	chdir(*(argv + 1));
}