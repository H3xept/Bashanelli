#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "pipeline_node.h"

PipelineNode* pn_new(char* cmd, char* in, char* out, OutMode omode) {
	assert(cmd);

	PipelineNode* pn = calloc(1, sizeof(PipelineNode));
	pn->cmd = calloc(strlen(cmd)+1, sizeof(char));
	strcpy(pn->cmd, cmd);
	
	if (in){
		pn->in = calloc(strlen(in)+1, sizeof(char));
		strcpy(pn->in, in);
	} else {
		pn->in = NULL;
	}

	if (out) {
		pn->out = calloc(strlen(out)+1, sizeof(char));
		strcpy(pn->out, out);
	} else {
		pn->out = NULL;
	}

	pn->omode = omode;
	pn->next = NULL;

	return pn;
}

void pn_add(PipelineNode* head, PipelineNode* next) {
	assert(head);
	assert(head->next == NULL);
	assert(next);
	head->next = next;
}

void pn_append(PipelineNode* head, PipelineNode* next) {
	PipelineNode* curr = head;
	while(curr) {
		if (curr->next == NULL){
			curr->next = next;
			break;
		} else {
			curr = curr->next;
		}
	}
}

void pn_destroy(PipelineNode* head) {
	PipelineNode* curr = head;
	while(curr) {
		PipelineNode* temp = curr->next;
		free(curr->cmd);
		if (curr->in) free(curr->in);
		if (curr->out) free(curr->out);
		free(curr);
		curr = temp;
	}	
}

void pn_array_destroy(int p_n, PipelineNode** array) {
	assert(array);
	for (int i = 0; i < p_n; ++i) {
		pn_destroy(array[i]);
	} free(array);
}
