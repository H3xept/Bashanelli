#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>

#include "builtins.h"
#include "aliasing.h"
#include "exporting.h"
#include "execute_command.h"

static void builtin_cd(const char** argv);
static void builtin_export(const char **argv);
static void builtin_alias(const char **argv);
static void builtin_unalias(const char **argv);
static void builtin_builtin(const char **argv);
static void builtin_source(const char **argv);
static void builtin_exec(const char** argv);

static const char* const builtins_list[] = {"cd", "export", "alias", "unalias", "builtin", "source", "exec"};

int builtin_id(const char* command){
	if(!command){
		return -1;
	}
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
		case 4:
			builtin_builtin(argv);
			break;
		case 5:
			builtin_source(argv);
			break;
		case 6:
			builtin_exec(argv);
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
	if(!*(argv+1)){
		print_exportlist();
		return;
	}
	int i = 1;
	if(!strcmp(*(argv+i),"-p")){
		print_exportlist();
		i++;
	}
	char *p = *(argv+i);
	while(p){
		char *ep = strchr(p, '=');
		if(ep) {
			char *tmp = calloc(strlen(p) - strlen(ep) + 1,sizeof(char));
			strncpy(tmp, p, strlen(p) - strlen(ep));
			add_export(tmp, ep+1);
			free(tmp);
		}
		i++;
		p = *(argv+i);
	}
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
				warnx("alias: %s: was not found", p);
			}
		}
		i++;
		p = *(argv+i);
	}
	return;
}

static void builtin_unalias(const char **argv){
if(!*(argv+1)){
		warnx("unalias: usage: unalias [-a] name [name ...]\n");
		return;
	}
	int i = 1;
	if(!strcmp(*(argv+i),"-a")){
		teardown_aliases();
		init_aliases();
		return;
	}
	char *p = *(argv+i);
	while(p){
		if(remove_alias(p)) {
			warnx("alias: %s: was not found", p);
		}
		i++;
		p = *(argv+i);
	}
	return;
}

static void builtin_builtin(const char** argv){
	if(!*(argv + 1)){
		printf("builtin: usage: builtin [shell-builtin [args]]\n");
		return;
	}
	exec_builtin_str(*(argv + 1), argv + 1);
}

static void builtin_source(const char** argv){
	if(!*(argv + 1)){
		printf("source: usage: source filename [args]\n");
		return;
	}
	char* full_path = file_path(*(argv + 1));
	if(full_path){
		execute_shell_script(full_path, argv + 1);
		free(full_path);
		return;
	}
	execute_shell_script(*(argv + 1), argv + 1);
}

static void builtin_exec(const char** argv){
	if(!*(argv + 1)){
		printf("exec: usage: exec filename [args]\n");
		return;
	}
	char* full_path = file_path(*(argv + 1));
	if(full_path){
		execute_bin(full_path, argv + 1);
		free(full_path);
		return;
	}
	execute_bin(*(argv + 1), argv + 1);
}