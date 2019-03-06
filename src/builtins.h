#ifndef __BUILTINS__
#define __BUILTINS__

#define NUM_BUILTINS 1

void init_builtins();
int builtin_id(const char* command);
void exec_builtin_str(const char* command, const char** argv);
void exec_builtin_id(const int id, const char** argv);

#endif