#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "builtins.h"
#include "aliasing.h"


static void builtin_cd(const char** argv);
static void builtin_export(const char **argv);
static void builtin_alias(const char **argv);
static void builtin_unalias(const char **argv);

static char* builtins_list[] = {"cd", "export", "alias", "unalias"};

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
		case 1:
			builtin_export(argv);
			break;
		case 2:
			builtin_alias(argv);
			break;
		case 3:
			builtin_unalias(argv);
			break;
		default:
			printf("Builtin id %d not found.\n", id);
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

static void builtin_alias(const char **argv){
	if(!*(argv+1)){
		print_aliaslist();
		return;
	}
	int i = 1;
	if(!strcmp(*(argv+i),"-p")){
		print_aliaslist();
		i++;
	}
	char *p = *(argv+i);
	while(p){
		char *ep = strchr(p, '=');
		if(ep) {
			char *tmp = calloc(strlen(p) - strlen(ep) + 1,sizeof(char));
			strncpy(tmp, p, strlen(p) - strlen(ep));
			add_alias(tmp, ep+1);
			free(tmp);
		}
		else {
			if(print_alias(p)) {
				warnx("alias: %s: was not found :(", p);
			}
		}
		i++;
		p = *(argv+i);
	}
	return;
}

static void builtin_unalias(const char **argv){
	return;
}
