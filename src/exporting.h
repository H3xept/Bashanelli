#ifndef __EXPORTING__
#define __EXPORTING__

void init_exports();

//gets number of exported variables
int get_exportlist_length();

//add export to export list and evironment
void add_export(const char *name, const char *value);

//remove export from list and environment
int remove_export(const char *name);

//print all exported variables
void print_exportlist();

//print "declare -x name = 'coresponding value'"
int print_export_var(const char *name);

//returns the value of the exported variable with given name
char *get_export_value(const char *name);

//frees export list
void teardown_exports();

#endif
