/**
 * File: builtins.c
 *
 * Description: Contains definitions of builtins used by Bashanelli
 * Also contains functions that can be called to execute builtins by
 * name or id.
 * 
 * Date: 
 * 15/3/2019
 * 
 * Authors: 
 *	1. Leonardo Cascianelli (me.leonardocascianelli@gmail.com)
 *	2. Rory Brown (rorybrown660@gmail.com)
 *	3. Ewan Skene (ewancskene@gmail.com)
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>

#include <BareBonesHistory/history.h>

#include "builtins.h"
#include "aliasing.h"
#include "exporting.h"
#include "execute_command.h"
#include "constants.h"

#define NUM_BUILTINS 11

static int builtin_cd(const char** argv);
static int builtin_export(const char **argv);
static int builtin_unset(const char **argv);
static int builtin_alias(const char **argv);
static int builtin_unalias(const char **argv);
static int builtin_builtin(const char **argv);
static int builtin_source(const char **argv);
static int builtin_exec(const char** argv);
static int builtin_history(const char** argv);
static int builtin_phil(const char** argv);
static void builtin_exit(const char** argv);

// List of all builtin names ordered by id
static const char* const builtins_list[] = {"cd", "export", "unset", "alias", "unalias", "builtin", "source", "exec", "history", "phil", "exit"};

// Retrieves the id associated with a builtin by name
int builtin_id(const char* name){
	if(!name){
		return -1;
	}
	for(int i = 0; i < NUM_BUILTINS; i++){
		if(!strcmp(name,*(builtins_list + i))){
			return i;
		}
	}
	return -1;
}

// Executes a builtin with a given name, passing a given argv
int exec_builtin_str(const char* name, const char** argv){
	return exec_builtin_id(builtin_id(name), argv);
}

// Executes a builtin with a given id, passing a given argv
int exec_builtin_id(int id, const char** argv){
	switch(id){
		case 0:
			return builtin_cd(argv);
		case 1:
			return builtin_export(argv);
		case 2:
			return builtin_unset(argv);
		case 3:
			return builtin_alias(argv);
		case 4:
			return builtin_unalias(argv);
		case 5:
			return builtin_builtin(argv);
		case 6:
			return builtin_source(argv);
		case 7:
			return builtin_exec(argv);
		case 8:
			return builtin_history(argv);
		case 9:
			return builtin_phil(argv);
		case 10:
			builtin_exit(argv);
		default:
			printf("Builtin id %d not found.\n", id);
			return -1;
	}
}

// Changes the working directory to the given arg
// If not second arg is given then the working directory is set to $HOME
static int builtin_cd(const char** argv){

	size_t argvn = 0;
	const char** cursor = argv;
	while(*cursor) { argvn++;  cursor++;}

	if(argvn > 2){
		printf("bashanelli: cd: too many arguments\n");
		return 1;
	}

	if(!*(argv + 1)) chdir(getenv(HOME_ENV));
	else if(chdir(*(argv + 1))) {
		warn("cd: %s", *(argv + 1));
		return 1;
	}
	char working_dir[MAX_CWD_LEN];
	getcwd(working_dir, MAX_CWD_LEN);
	add_export(PWD_ENV, working_dir);
	return 0;
}

// Parses exports from argv in form a=b where a = env var name and b = value
static int builtin_export(const char** argv){
	if(!*(argv+1)){
		print_exportlist();
		return 0;
	}
	int i = 1;
	if(!strcmp(*(argv+i),"-p")){
		print_exportlist();
		i++;
	}
	const char *p = *(argv+i);
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
	return 0;
}

// Unsets an env var's value based on name from argv
static int builtin_unset(const char **argv){
	if(!*(argv+1)){
		return 1;
	}
	int i = 1;
	const char *p = *(argv+i);
	while(p){
		if(remove_export(p)) {
			warnx("unset: %s: was not found", p);
		}
		i++;
		p = *(argv+i);
	}
	return 0;
}

static int builtin_alias(const char** argv){
	if(!*(argv+1)){
		print_aliaslist();
		return 0;
	}
	int i = 1;
	if(!strcmp(*(argv+i),"-p")){
		print_aliaslist();
		i++;
	}
	const char *p = *(argv+i);
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
	return 0;
}

static int builtin_unalias(const char** argv){
if(!*(argv+1)){
		warnx("unalias: usage: unalias [-a] name [name ...]\n");
		return 1;
	}
	int i = 1;
	if(!strcmp(*(argv+i),"-a")){
		teardown_aliases();
		init_aliases();
		return 0;
	}
	const char *p = *(argv+i);
	while(p){
		if(remove_alias(p)) {
			warnx("alias: %s: was not found", p);
		}
		i++;
		p = *(argv+i);
	}
	return 0;
}

static int builtin_builtin(const char** argv){
	if(!*(argv + 1)){
		printf("builtin: usage: builtin [shell-builtin [args]]\n");
		return 1;
	}
	exec_builtin_str(*(argv + 1), argv + 1);
	return 0;
}

static int builtin_source(const char** argv){
	if(!*(argv + 1)){
		printf("source: usage: source filename [args]\n");
		return 1;
	}
	const char* full_path = file_path(*(argv + 1));
	if(full_path){
		execute_shell_script(full_path, argv + 1, NO_FD, NO_FD);
		free((char*)full_path);
		return 0;
	}
	execute_shell_script(*(argv + 1), argv + 1, NO_FD, NO_FD);
	return 0;
}

static int builtin_exec(const char** argv){
	if(!*(argv + 1)){
		printf("exec: usage: exec filename [args]\n");
		return 1;
	}
	const char* full_path = file_path(*(argv + 1));
	if(full_path){
		execute_bin(full_path, argv + 1, NO_FD, NO_FD);
		free((char*)full_path);
		return 0;
	}
	execute_bin(*(argv + 1), argv + 1, NO_FD, NO_FD);
	return 0;
}

static int builtin_history(const char** argv){
	if(*(argv + 1)){
		if(!strcmp(*(argv+1),"-c") || !strcmp(*(argv+1),"--clear")){
			clear_history();
			return 0;
		}
		printf("bashanelli: history: too many arguments\n");
		return 1;
	}
	
	char** entries = get_entire_history();
	if(!entries){
		return 0;
	}
	for(int i = 0; i < entries_n(); i++){
		printf(" %d\t%s\n", i + 1, *(entries + i));
	}
	free(entries);
	return 0;
}

static void builtin_exit(const char** argv){
	char* home_directory = getenv(HOME_ENV);
	char* history_filepath = calloc(strlen(home_directory) + strlen(HISTORY_FILE) + 2, sizeof(char));
	strcpy(history_filepath, home_directory);
	strcat(history_filepath, "/");
	strcat(history_filepath, HISTORY_FILE);
	export_history_to_file(history_filepath);
	exit(0);
}

static int builtin_phil(const char** argv) {
	printf("***********************************\n");
	printf("Hi, can we get full marks please 👾\n");
	printf("***********************************\n");
	return 0;
}
