
#ifndef PARSE_COMMANDS
#define PARSE_COMMANDS

int count_occ( char* str, char c);


char** generate_argv( char* command);

char* trim_whitespace( char* str);

//gets single command 
char** parse_command( char *command);

//input full line of commands and pipeline/redirection symbols
char **seperate_into_commands( char *command);

void ignore_comment(char *line);

#endif