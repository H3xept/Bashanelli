#include <unistd.h>
#include <ANSIsACurse/characters.h>

#define MAX_CWD_LEN 5000
#define MAX_LOGIN_LEN 5000
#define MAX_HOSTNAME_LEN 5000

char* generate_ps1(){
	char* cwd[MAX_CWD_LEN];
	getcwd(cwd, MAX_CWD_LEN);
	char* login[MAX_LOGIN_LEN];
	getlogin_r(login, MAX_LOGIN_LEN);
	char* hostname[MAX_HOSTNAME_LEN];
	gethostname(hostname, MAX_HOSTNAME_LEN);

	char* login_hostname[MAX_LOGIN_LEN + MAX_HOSTNAME_LEN + 1];
	sprintf(login_hostname, "%s@%s", login, hostname);

	char* login_hostname_color = c_color_string(login_hostname, C_COLOR_CODE_Green);
	char* working_dir = c_color_string(cwd, C_COLOR_CODE_Blue);

	char* ps1[MAX_HOSTNAME_LEN + MAX_LOGIN_LEN + MAX_CWD_LEN];

	char* working_dir_bold = c_decorate_string(working_dir, C_DECORATION_Bold);
	sprintf(ps1, "%s:%s$ ", login_hostname_color, working_dir_bold);
	char* bold_ps1 = c_decorate_string(ps1, C_DECORATION_Bold);
	free(login_hostname_color);
	free(working_dir);
	free(working_dir_bold);

	return bold_ps1;
}