#ifndef __EXECUTE_COMMAND__

#define __EXECUTE_COMMAND__


// Executes given string as if it were a command. Currently handles its own argv parsing (move later)
void execute_command( char* command);

// Executes a given command (with args) string as if it were a builtin. (Has a big switch of builtin functions)
void execute_builtin( char* command,  char** argv);

// Executes given command + args as if it were a shell script. Wrapper for handle_script in script_handling.c
void execute_shell_script( char* filename,  char** argv);

// Executes given command + args as if it were a binary file (program).
void execute_bin( char* filename,  char** argv);

int is_builtin( char* command);

int is_executable( char* command);

int file_exists( char* filename);

#endif