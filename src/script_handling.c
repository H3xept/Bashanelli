#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>
#include <BareBonesReadline/readline.h>

#include "script_handling.h"
#include "execute_command.h"
#include "parse_commands.h"

#define MAX_CMD_LEN 500
#define DEAFAULT_LINES 50

int handle_script(char *filename) {

	unsigned int lines = 0;
	char **scrlines = 0;
	char *inpath = 0;

	inpath = search_path(filename);
	if(inpath) {
		scrlines = read_file(inpath, &lines);
	} else {
		scrlines = read_file(filename, &lines);
	}

	if(!scrlines)
		return 1;

	int c = 0;
	while(c < lines) {
		if(*scrlines[c]){
			parse_then_execute(scrlines[c]);	
		}
		c++;
	}
	free(scrlines);
	return 0;
}

void parse_then_execute(const char* command) {
	char* pcom = parse_line(command);
	char** args = parse_command(pcom);
	execute_command(args);
	if(pcom){
		free(pcom);		
	}
	if(args){
		free(args);			
	}
}

//reads file line by line and returns malloced char** 
//if (line count) *lcount < actual number of lines in the file then *lcount is increased accordingly
char **read_file(char* filepath, unsigned int *lcount) {
	
	*lcount = DEAFAULT_LINES; //default size, IK magic numbers are probably bad but whatever, it stops uninitialised and 0 lcount from breaking it.
	FILE* sfp = fopen(filepath,"r");
	if (!sfp) {
		*lcount = 0;
		return 0;
	}
	
	char **lines = malloc(*lcount*sizeof(char *));
	int c = 0;
	char *tmp = 0;
	size_t n = 0;
	errno = 0;

	while (getline(&tmp,&n,sfp) != -1 && !errno) {
		//realloc if too many lines
		if (c == *lcount) {
			*lcount *= 2;
			lines = realloc(lines,*lcount*sizeof(char *));
		}
		lines[c] = malloc(strlen(tmp)*sizeof(char)+1);
		strcpy(lines[c],tmp);
		REMOVE_NEWLINE(lines[c])
		ignore_comment(lines[c]);
		c++;
		tmp = 0;
	}
	*lcount = c;
	fclose(sfp);
	return lines;
}

//searches the PATH env variable for the filename
//returns the full path to that file if present in PATH,
//otherwise returns 0/NULL
char *search_path(char* filename) {

	FILE *fp;
	char *path, *ret, *tempath;
	path = malloc(strlen(getenv("PATH"))*sizeof(char)+1);
	strcpy(path,getenv("PATH"));

	if(!path)
		return 0;

	tempath = strtok(path,":");
	while (tempath != NULL) {
		ret = malloc(strlen(tempath) + strlen(filename) + 2);
		strcpy(ret, tempath);
		strcat(ret,"/");
		strcat(ret, filename);
		fp = fopen(ret,"r");
		if (fp) {
			fclose(fp);
			return ret;
		}
		free(ret);
		tempath = strtok(NULL,":");
	}
	free(path);
	return 0;
}
