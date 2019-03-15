#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ANSIsACurse/characters.h>
#include <BareBonesReadline/string_utils.h>

#include "exporting.h"
#include "argv.h"
#include "constants.h"

char* generate_ps1(){
	char* ps1 = get_export_value(PS1_ENV);
	if(!ps1){
		char* cwd = calloc(MAX_CWD_LEN, sizeof(char));
		getcwd(cwd, MAX_CWD_LEN);
		strcat(cwd, "$ ");
		return cwd;
	}

	char hostname[MAX_HOSTNAME_LEN];
	gethostname(hostname, MAX_HOSTNAME_LEN);
	char hostname_cut[MAX_HOSTNAME_LEN];
	for(int i = 0; i < strlen(hostname) + 1; i++){
		if(hostname[i] == '.'){
			hostname_cut[i] = '\0';
			break;
		}
		hostname_cut[i] = hostname[i];
	}

	char working_dir[MAX_CWD_LEN];
	getcwd(working_dir, MAX_CWD_LEN);

	char* home_directory = getenv(HOME_ENV);
	char working_dir_w_tilde[MAX_CWD_LEN];
	if(strstr(working_dir, home_directory) == working_dir){
		working_dir_w_tilde[0] = '~';
		for(int i = strlen(home_directory); i < strlen(working_dir) + 1; i++){
			working_dir_w_tilde[i - strlen(home_directory) + 1] = working_dir[i];
		}
	}
	else{
		strcpy(working_dir_w_tilde, working_dir);
	}

	char login[MAX_LOGIN_LEN];
	getlogin_r(login, MAX_LOGIN_LEN);

	char* shellname = *get_current_argv();


	char* ps1_h = su_replace_occurrencies_of(ps1, "\\h", hostname_cut);
	char* ps1_H = su_replace_occurrencies_of(ps1_h, "\\H", hostname);
	char* ps1_u = su_replace_occurrencies_of(ps1_H, "\\u", login);
	char* ps1_n = su_replace_occurrencies_of(ps1_u, "\\n", "\n");
	char* ps1_s = su_replace_occurrencies_of(ps1_n, "\\s", shellname);
	char* ps1_w = su_replace_occurrencies_of(ps1_s, "\\w", working_dir_w_tilde);
	char* ps1_bs = su_replace_occurrencies_of(ps1_w, "\\\\", "\\");
	char* ps1_c = su_replace_occurrencies_of(ps1_bs, "\\033", "\033");
	
	free(ps1_h);
	free(ps1_H);
	free(ps1_u);
	free(ps1_n);
	free(ps1_s);
	free(ps1_w);
	free(ps1_bs);
	if (ps1) free(ps1);
	
	return ps1_c;
}
