#ifndef __ALIASING__

#define __ALIASING__

void init_aliases();
int get_aliaslist_length();
void add_alias(const char *alias, const char *command);
void print_aliaslist();

#endif