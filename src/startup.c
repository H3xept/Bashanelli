#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <BareBonesReadline/readline.h>

#include "parse_commands.h"
#include "execute_command.h"

#define BNLI_PROFILE ".bnli_profile"
#define K_HOME_ENV "HOME"


static void load_profile_login();
static void load_profile_nonlogin();
static void load_profile_bnli();

void startup(const int argc, const char** argv){
	int login = **argv == '-';
	int noprofile = 0;
	int first_non_flag_arg = -1;
	for(int i = 1; i < argc; i++){
		if(!(*argv[i] == '-')){
			first_non_flag_arg = i;
		}
		else{
			if(!strcmp(argv[i], "-l") || !strcmp(argv[i], "--login")){
				login = 1;
			}
			else if(!strcmp(argv[i], "--noprofile")){
				noprofile = 1;
			}
		}
	}
	if(!noprofile){
		if(login){
			load_profile_login();
		}
		else{
			load_profile_nonlogin();
		}
		load_profile_bnli();
	}
	
	if(first_non_flag_arg > 0){
		char* cmd = parse_line(argv[first_non_flag_arg]);
		// parse_and_execute_command(cmd);
		const char **argv = (const char**)parse_command(cmd);
		execute_shell_script(cmd, argv);
		free(cmd);
		if(argv){
			int i = 0;
			while(*(argv+i)) {
				free((char*)*(argv+i));
				i++;
			}
			free(argv);
		}
	}
}

static void load_profile_login(){
	#warning Profile loading disabled.
	return;
	parse_and_execute_command("/etc/profile");
	char* cmd = parse_line("~/.bash_profile");
	parse_and_execute_command(cmd);
	free(cmd);
	cmd = parse_line("~/.bash_login");
	parse_and_execute_command(cmd);
	free(cmd);
	cmd = parse_line("~/.profile");
	parse_and_execute_command(cmd);
	free(cmd);
	cmd = parse_line("~/.bashrc");
	parse_and_execute_command(cmd);
	free(cmd);
}

static void load_profile_nonlogin(){
	return;
	parse_and_execute_command("/etc/bash.bashrc");
	char* cmd = parse_line("~/.bashrc");
	parse_and_execute_command(cmd);
	free(cmd);
}

static void load_profile_bnli(){
	char* home_directory = getenv(K_HOME_ENV);
	char* profile_location = calloc(strlen(home_directory) + strlen(BNLI_PROFILE) + 2, sizeof(char));
	strcpy(profile_location, home_directory);
	strcat(profile_location, "/");
	strcat(profile_location, BNLI_PROFILE);
	char** argv = parse_command(profile_location);
	execute_shell_script(profile_location, (const char**)argv);
	if(argv){
		int i = 0;
		while(*(argv+i)) {
			free((char*)*(argv+i));
			i++;
		}
		free((char**)argv);
	}
	free(profile_location);
}
