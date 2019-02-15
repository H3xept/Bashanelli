#include <ANSIsACurse/characters.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_CMD_LEN 500
#define K_HOME_ENV "HOME"
#define K_PATH_ENV "PATH"
#define K_PS1_ENV "PS1"

char* read_history() {
	return "ls\ncd \"lolz\"";
}

int main(int argc, char const *argv[])
{
	char buffer[MAX_CMD_LEN];
	char* home_directory = getenv(K_HOME_ENV);
	char* env_path = getenv(K_PATH_ENV);
	char* history = NULL;

	if (chdir(home_directory) != 0) {
		// DIE
		printf("Could not change directory!");
		exit(-1);
	}

	history = read_history();

	while(1) {
		printf("%s","Shellerino Diocanarino$ ");
		getchar();
	}

	printf("Env: %s",home_directory);
	return 0;
}
