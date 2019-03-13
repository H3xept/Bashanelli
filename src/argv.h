#ifndef __ARGV__
#define __ARGV__

void init_argv();
void push_argv_frame(const char** argv, const int argc);
void pop_argv_frame();
char** get_current_argv();
char* get_arg_from_current_argv(const int index);

#endif
