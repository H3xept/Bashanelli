#ifndef __EXECUTE_COMMAND__

#define __EXECUTE_COMMAND__
#define SIGNATURE_MACH_O 0xfeedface
#define SIGNATURE_MACH_O_REVERSE 0xcefaedfe
#define SIGNATURE_ELF 0x7f454c46
#define SIGNATURE_ELF_REVERSE 0x464c457f

// Executes given string as if it were a command. Currently handles its own argv parsing (move later)
void execute_command(const char* command);

// Executes a given command (with args) string as if it were a builtin. (Has a big switch of builtin functions)
void execute_builtin(const char* command, const char** argv);

// Executes given command + args as if it were a shell script. Wrapper for handle_script in script_handling.c
void execute_shell_script(const char* filename, const char** argv);

// Executes given command + args as if it were a binary file (program).
void execute_bin(const char* filename, const char** argv);

#endif