
#ifndef PARSE_COMMANDS
#define PARSE_COMMANDS

#define SIGNATURE_MACH_O 0xfeedface
#define SIGNATURE_MACH_O_REVERSE 0xcefaedfe
#define SIGNATURE_ELF 0x7f454c46
#define SIGNATURE_ELF_REVERSE 0x464c457f


int is_builtin( char* command);


int is_executable( char* command);


int count_occ( char* str, char c);


char** generate_argv( char* command);


int file_exists( char* filename);


char* trim_whitespace( char* str);

//gets single command 
char** parse_command( char *command);

//input full line of commands and pipeline/redirection symbols
char **seperate_into_commands( char *command);

#endif