
#ifndef PARSE_COMMANDS
#define PARSE_COMMANDS

#define SIGNATURE_MACH_O 0xfeedface
#define SIGNATURE_MACH_O_REVERSE 0xcefaedfe
#define SIGNATURE_ELF 0x7f454c46
#define SIGNATURE_ELF_REVERSE 0x464c457f

int is_builtin(const char* command);
int is_executable(const char* command);
int count_occ(const char* str, char c); char* _trim_whitespace(const char* str);
char** generate_argv(const char* command);
int file_exists(const char* filename);
char* trim_whitespace(const char* str);
char** parse_command(const char *command);

#endif