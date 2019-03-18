#ifndef __EXECUTE_COMMAND__
#define __EXECUTE_COMMAND__

#define NO_FD -1
#define NO_INFILE -2
#define NO_OUTFILE -3

#include "pipeline_node.h"

// Executes given string as if it were a command. Currently handles its own argv parsing (move later)
void execute_command(const char** argv, int in_fd, int out_fd);

// Executes an array of pipelines
void execute_pipelines(int pipelines_n, const PipelineNode** pipelines);

//generates argv with parse_command before calling execute_command
void parse_and_execute_command(const char* command);

// Executes a given command (with args) string as if it were a builtin. (Has a big switch of builtin functions)
void execute_builtin(const char* command, const char** argv, int in_fd, int out_fd);

// Executes given command + args as if it were a shell script. Wrapper for handle_script in script_handling.c
void execute_shell_script(const char* filename, const char** argv, int in_fd, int out_fd);

// Executes given command + args as if it were a binary file (program).
void execute_bin(const char* filename, const char** argv, int in_fd, int out_fd);

int is_builtin(const char* command);
int is_executable(const char* filename);
int file_exists(const char* filename);
const char* file_path(const char* filename);
int is_dir(const char* path);

int get_exit_code();

#endif
