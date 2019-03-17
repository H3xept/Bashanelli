#ifndef __PIPELINE_NODE__
#define __PIPELINE_NODE__

typedef enum {
	OUT_MODE_OW,
	OUT_MODE_AP
} OutMode;

typedef struct _PipelineNode {
	char* cmd;
	char* in;
	char* out;
	OutMode omode;
	struct _PipelineNode* next;
} PipelineNode;

PipelineNode* pn_new(char* cmd, char* in, char* out, OutMode omode);
void pn_add(PipelineNode* head, PipelineNode* next);
void pn_append(PipelineNode* head, PipelineNode* next);
void pn_destroy(PipelineNode* head);

#endif

