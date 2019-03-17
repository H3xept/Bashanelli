#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "redirection.h"

#define REDIR_RIGHT '>'
#define REDIR_LEFT '<'
#define REDIR_PIPE '|'

static char redir_symbols[] = {REDIR_RIGHT, REDIR_LEFT, REDIR_PIPE, '\0'};

static unsigned int in(const char c, const char* const c_set) {
	for (int i = 0; i < strlen(c_set); i++) {
		if (c_set[i] == c) return 1;
	} 
	return 0;
}

static int check_redir_left(const char* const command, int i) {
	if (strlen(command)-1 == i) return 0;

	for (int j = i+1; j < strlen(command); j++) {
		if (in(*(command+j), redir_symbols)) return 0;
		else if(*(command+j) != ' ') return 1;
	} return 0;
}

static int check_redir_right(const char* const command, int i) {
	if (strlen(command)-1 == i) return 0;

	for (int j = i+1; j < strlen(command); j++) {
		if (in(*(command+j), redir_symbols)) {
			if (!(*(command+j) == REDIR_RIGHT && j == i+1)) return 0;
		} 
		else if(*(command+j) != ' ') return 1;

	} return 0;
}

static int check_redir_pipe(const char* const command, int i) {
	if (i == 0) return 0;

	int state = 0;

	for (int j = i-1; j > 0; j--) {
		if (in(*(command+j), redir_symbols)) return 0;
		else if(*(command+j) != ' ') { state = 1; break; }
	}

	if (!state) return 0;

	for (int j = i+1; j < strlen(command); j++) {
		if (in(*(command+j), redir_symbols)) return 0;
		else if(*(command+j) != ' ') return 1;
	} return 0;
}

int redirection_syntax_check(const char* const command) {
	assert(command);
	int ret = 1;
	for (int i = 0; i < strlen(command); i++) {
		if (i[command] == REDIR_RIGHT) {
			ret = check_redir_right(command, i);
		} else if (i[command] == REDIR_LEFT) {
			ret = check_redir_left(command, i);
		} else if (i[command] == REDIR_PIPE) {
			ret = check_redir_pipe(command, i);
		} if (!ret) return ret;
	} 
	return ret;
}
