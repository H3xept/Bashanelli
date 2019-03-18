#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "redirection.h"
#include "pipeline_node.h"

#define REDIR_RIGHT '>'
#define REDIR_LEFT '<'
#define REDIR_PIPE '|'

#define IN_TOKEN "<"
#define OUT_TOKEN ">"
#define PIPE_TOKEN "|"

// Syntax check

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

int rd_syntax_check(const char* const command) {
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

// END Syntax check

// Parse Command

static char* get_in_filename(const char* const subcmd) {
	assert(subcmd);
	
	char* beg = strstr(subcmd, IN_TOKEN);
	if (!beg) return NULL;
	beg += 1;
	char* ret = calloc(strlen(beg)+1, sizeof(char));

	while(*beg != '\0' && *beg == ' ') beg++;
	if (*beg == '\0') return NULL;

	for (int i = 0; i < strlen(beg); i++) {
		if (*(beg+i) != ' ') i[ret] = *(beg+i);
		else break;
	}

	ret = realloc(ret, (strlen(ret)+1)*sizeof(char));
	return ret;
}

static char* get_out_filename(const char* const subcmd, OutMode* omode) {
	assert(subcmd);
	assert(omode);

	char* beg = strstr(subcmd, OUT_TOKEN);
	if (!beg) return NULL;
	beg += 1;
	if (*beg == REDIR_RIGHT) {
		beg++;
		*omode = OUT_MODE_AP;
	} else *omode = OUT_MODE_OW;

	char* ret = calloc(strlen(beg)+1, sizeof(char));

	while(*beg != '\0' && *beg == ' ') beg++;
	if (*beg == '\0') return NULL;

	for (int i = 0; i < strlen(beg); i++) {
		if (*(beg+i) != ' ') i[ret] = *(beg+i);
		else break;
	}

	ret = realloc(ret, (strlen(ret)+1)*sizeof(char));
	return ret;
}

static char* get_command(const char* const subcmd) {

	assert(subcmd);
	const char* cursor = subcmd;
	char* ret = calloc(strlen(subcmd)+1, sizeof(char));

	while(*cursor && *cursor == ' ') cursor++;
	for (int i = 0; i < strlen(cursor); i++) {
		if (!in(*(cursor+i), redir_symbols)) i[ret] = *(cursor+i);
		else break;
	}

	for (int i = strlen(ret)-1; i >= 0; i--) {
		if (i[ret] == ' ') i[ret] = '\0';
		else break;
	}

	ret = realloc(ret, (strlen(ret)+1)*sizeof(char));
	return ret;
}

static void create_node_and_append(PipelineNode** head, const char* const command) {

	OutMode omode;
	PipelineNode* node = NULL;

	char* cmd = get_command(command);
	char* in = get_in_filename(command);
	char* out = get_out_filename(command, &omode);

	node = pn_new(cmd, in, out, omode);	

	if (!(*head)) *head = node;
	else pn_append(*head, node);
}

static PipelineNode* create_pipeline(const char* const command) {
	if (!command) return NULL;
	
	PipelineNode* head = NULL;
	char* subcmd = NULL;
	const char* last = command;

	for (int i = 0; i < strlen(command); i++) {
		if (i[command] == '|' || i == (strlen(command)-1)) {
			int subcmd_len = (int)((command+i)-last)+1;

			subcmd = calloc(subcmd_len+1, sizeof(char));
			strncpy(subcmd, last, subcmd_len);
			create_node_and_append(&head, subcmd);

			free(subcmd); subcmd = 0;
			last = command+i+1;
		}
	}

	return head;
}

static int pipelines_n(const char* const command) {
	assert(command);
	int ret = 1;
	for (int i = 0; i < strlen(command); i++) {
		if (i[command] == ';') ret++;
	} return ret;
}

PipelineNode** rd_parse_command(int* nodes_n, const char* const command) {
	if (!command) return NULL;
	
	// Hack to make aliases work with all this last minute crap
	// 🤖👾
	if (!strncmp(command, "alias", 5)) {
		PipelineNode** fakearray = calloc(1, sizeof(PipelineNode*));
		0[fakearray] = pn_new((char*)command, NULL, NULL, 0);
		*nodes_n = 1;
		return fakearray;
	}

	PipelineNode** pipelines_array = NULL;
	int p_n = pipelines_n(command);
	*nodes_n = p_n;

	pipelines_array = calloc(p_n, sizeof(PipelineNode*));

	int j = 0;
	char* subcmd = NULL;
	const char* last = command;
	for (int i = 0; i < strlen(command); i++) {
		if (i[command] == ';' || i == (strlen(command)-1)) {
			int subcmd_len = (int)((command+i)-last)+1;
			subcmd = calloc(subcmd_len+1, sizeof(char));
			strncpy(subcmd, last, subcmd_len);
			
			for (int k = 0; k < strlen(subcmd); ++k) 
				if (k[subcmd] == ';') { subcmd[k] = '\0'; break; }

			PipelineNode* node = create_pipeline(subcmd);
			*(pipelines_array+j) = node;
			j++;
			free(subcmd); subcmd = 0;
			last = command+i+1;
		}
	}

	return pipelines_array;
}

// Wrappers for testing 

char* rd_test_get_in_filename(const char* const subcmd) {
	return get_in_filename(subcmd);
}

char* rd_test_get_out_filename(const char* const subcmd, OutMode* omode) {
	return get_out_filename(subcmd, omode);
}

char* rd_test_get_command(const char* const subcmd) {
	return get_command(subcmd);
}

PipelineNode* rd_test_create_pipeline(const char* const command) {
	return create_pipeline(command);
}
