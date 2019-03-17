#ifndef __PARSE_COMMANDS__
#define __PARSE_COMMANDS__

//gets single command, trims outer whitespace, ignores line after #, returns alloced argv.
char** parse_command(const char *command);

//input full line of commands and pipeline/redirection symbols
//UNIMPLEMENTED
//char **seperate_into_commands(const char *command);

//gets alloced tokens from line without trimming whitespace or ignoring comments
char** generate_argv(char* command);

//returns alloced line stripping anything after and including #
char *ignore_comment(const char *line);

//returns number of occurences of char c in str.
int count_occ(const char* str, const char c);

//returns alloced str without outer whitespace
char* trim_whitespace(const char* str);

//returns alloced str with first word replaced with alias
//otherwise returns alloced copy of original str
char *expand_alias(const char *line);

//returns alloced line with $x replaced with exported variables
//returns copy of alloced line if no $s
char *expand_exvar(const char *line);


#endif
