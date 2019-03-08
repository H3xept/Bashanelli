#ifndef __ALIASING__

#define __ALIASING__

//inits alias variables
void init_aliases();
int get_aliaslist_length();
void add_alias(const char *alias, const char *command);
int remove_alias(const char *alias);
void print_aliaslist();
int print_alias(const char *alias);
char *expand_alias(char *alias);
void teardown_aliases();


#endif
