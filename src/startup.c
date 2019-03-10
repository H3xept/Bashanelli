#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "execute_command.h"
#include "../dependencies/BareBonesReadline/readline.h"


static void load_profile_login();
static void load_profile_nonlogin();

void startup(const int argc, const char** argv){
	int login = **argv == '-';
	int noprofile = 0;
	int first_non_flag_arg = -1;
	for(int i = 1; i < argc; i++){
		if(!*argv[i] == '-'){
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
	}
	
	if(first_non_flag_arg > 0){
		char* cmd = parse_line(argv[first_non_flag_arg]);
		parse_and_execute_command(cmd);
		free(cmd);
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
	//exit(0);
	char* cmd = parse_line("~/.bashrc");
	parse_and_execute_command(cmd);
	free(cmd);
}
