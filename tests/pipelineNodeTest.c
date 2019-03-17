#include "pipelineNodeTest.h"
#include "../src/pipeline_node.h"
#include "macros.h"

void setup() {
	return;
}

void teardown() {
	return;
}

void testCreate() {
	PipelineNode* head = pn_new("echo Hello", NULL, NULL, 0);
	assert_not_null(head);
}

void testAdd() {
	PipelineNode* head = pn_new("echo Hello", NULL, NULL, 0);
	PipelineNode* next = pn_new("cat file", NULL, NULL, 0);
	pn_add(head, next);
	assert_not_null(head->next);
}

void testAppend() {
	PipelineNode* head = pn_new("echo Hello", NULL, NULL, 0);
	PipelineNode* next = pn_new("cat file", NULL, NULL, 0);
	PipelineNode* next2 = pn_new("cat file2", NULL, NULL, 0);
	pn_append(head, next);
	pn_append(head, next2);
	assert_not_null(head->next);
	assert_not_null(head->next->next);
	assert_eq_str(next->cmd, head->next->cmd);
	assert_eq_str(next2->cmd, head->next->next->cmd);
}

void testDestroy() {
	PipelineNode* head = pn_new("echo Hello", NULL, NULL, 0);
	pn_destroy(head);
}
