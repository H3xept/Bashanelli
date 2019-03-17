#ifndef __REDIRECTION__
#define __REDIRECTION__

typedef struct _PipelineNode {
	char* cmd;
	
} PipelineNode;

int redirection_syntax_check(const char* const command);

#endif

