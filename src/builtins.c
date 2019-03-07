#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "builtins.h"


static void builtin_cd(const char** argv);
static void builtin_export(const char **argv);

static char* builtins_list[] = {"cd", "export"};

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
	if(!*(argv + 1)){
		chdir(getenv("HOME"));
	}
	else if(chdir(*(argv + 1))) {
		warn("cd: %s", *(argv + 1));
	}
}

static void builtin_export(const char **argv){
	return;
}