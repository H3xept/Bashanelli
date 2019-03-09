#ifndef _EXPORTING_
#define _EXPORTING_


void init_exports();


int get_exportlist_length();

//add export to export list and evironment
void add_export(const char *name, const char *value);

//remove export from list and environment
int remove_export(const char *name);


void print_exportlist();


int print_export_var(const char *name);


void teardown_exports();

#endif
