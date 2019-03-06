#ifndef PARSE_COMMANDS
#define PARSE_COMMANDS

//gets single command, returns argv.
char** parse_command(const char *command);

//input full line of commands and pipeline/redirection symbols
char **seperate_into_commands(const char *command);

char** generate_argv(char* command);

char *get_arg(char *arg, char **endptr);
int count_occ(const char* str, const char c);
char* trim_whitespace(const char* str);
void ignore_comment(char *line);

#endif
