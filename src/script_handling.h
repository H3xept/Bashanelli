
#ifndef SCRIPT_HANDLING
#define SCRIPT_HANDLING


#define REMOVE_NEWLINE(x)	{if (x[strlen(x)-1] == '\n') x[strlen(x)-1] = '\0';}
char *search_path(char* filename);
char **read_file(char* filepath, unsigned int *lcount);
int handle_script(char *filename);

#endif