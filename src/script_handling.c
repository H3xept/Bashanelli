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
#define DEFAULT_LINES 50

int handle_script(const char *filename) {

	unsigned int lines = 0;
	char **scrlines = 0;
	char *inpath = 0;

	int notinpath = 0;
	for(int i = 0; i < strlen(filename); i++){
		if(*(filename + i) == '/'){
			notinpath = 1;
			break;
		}
	}
	if(!notinpath){
			inpath = search_path(filename);
		if(inpath) {
			scrlines = read_file(inpath, &lines);
		} else {
			scrlines = read_file(filename, &lines);
		}
	}
	else{
		scrlines = read_file(filename, &lines);
	}

	if(!scrlines)
		return 1;

	int c = 0;
	while(c < lines) {
		if(*scrlines[c]){
			char* line = calloc(strlen(scrlines[c]) + 1, sizeof(char));
			strcpy(line, scrlines[c]);
			parse_and_execute_command(line);
			free(line);	
		}
		c++;
	}
	int i = 0;
	while(scrlines[i]){
		free(scrlines[i]);
		i++;
	}
	if(scrlines){
		free(scrlines);
	}
	return 0;
}

char **read_file(const char* filepath, unsigned int *lcount) {
	
	*lcount = DEFAULT_LINES; //default size, IK magic numbers are probably bad but whatever, it stops uninitialised and 0 lcount from breaking it.
	FILE* sfp = fopen(filepath,"r");
	if (!sfp) {
		*lcount = 0;
		return 0;
	}
	
	char **lines = calloc(*lcount, sizeof(char *));
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
		//REMOVE_NEWLINE(lines[c])
		//ignore_comment(lines[c]);
		c++;
		char *oldtmp = tmp;
		tmp = 0;
		free(oldtmp);
	}
	*lcount = c;
	fclose(sfp);
	return lines;
}

char *search_path(const char* filename) {

	FILE *fp;
	char *path, *ret, *tempath;
	path = malloc((strlen(getenv("PATH"))+1)*sizeof(char));
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
