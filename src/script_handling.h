#ifndef __SCRIPT_HANDLING__
#define __SCRIPT_HANDLING__

#define REMOVE_NEWLINE(x)	{if (x[strlen(x)-1] == '\n') x[strlen(x)-1] = '\0';}

//searches the PATH env variable for the filename
//returns the full path to that file if present in PATH,
//otherwise returns 0/NULL
char *search_path(const char* filename);

//reads file line by line and returns malloced char** 
//if (line count) *lcount < actual number of lines in the file then *lcount is increased accordingly
char **read_file(const char* filepath, unsigned int *lcount);

//calls read_file to get argv 
//then calls parse_and_execute_command on each line delimited by newline
int handle_script(const char *filename);

#endif
