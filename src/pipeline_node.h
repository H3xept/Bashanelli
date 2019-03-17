#ifndef __PIPELINE_NODE__
#define __PIPELINE_NODE__

typedef struct _PipelineNode {
	char* cmd;
	char* in;
	char* out;
	struct _PipelineNode* next;
} PipelineNode;

PipelineNode* pn_new(char* cmd, char* in, char* out);
void pn_add(PipelineNode* head, PipelineNode* next);
void pn_append(PipelineNode* head, PipelineNode* next);
void pn_destroy(PipelineNode* head);

#endif

