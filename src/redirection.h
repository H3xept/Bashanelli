#ifndef __REDIRECTION__
#define __REDIRECTION__

#include "pipeline_node.h"

int rd_syntax_check(const char* const command);
PipelineNode** rd_parse_command(int* nodes_n, const char* const command);

// Wrappers
char* rd_test_get_in_filename(const char* const subcmd);
char* rd_test_get_out_filename(const char* const subcmd, OutMode* omode);
char* rd_test_get_command(const char* const subcmd);
PipelineNode* rd_test_create_pipeline(const char* const command);

#endif

