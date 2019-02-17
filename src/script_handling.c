#include "script_handling.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int handle_script(char *filename) {


	unsigned int lines;
	char **scrlines;
	char *inpath;
	inpath = search_path(filename);
	printf("%sITSOUT",inpath );
	if(inpath) {
		scrlines = read_file(inpath, &lines);
	} else {
		scrlines = read_file(filename, &lines);
	}

	//int c = 0;
	//while(c < lines) {
		//parse_line(scrlines[c]);
		//execute_stuff(the_parsed_line)
	//	c++;
	//}

	free(scrlines);

	return 0;
}

//reads file line by line and returns malloced char** 
//if (line count) *lcount < actual number of lines in the file then *lcount is increased accordingly
char **read_file(char* filepath, unsigned int *lcount) {

	*lcount = 10; //default size, IK magic numbers are probably bad but whatever, it stops uninitialised and 0 lcount from breaking it.
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
		//expands size if > lcount lines 
		if (c == *lcount) {
			*lcount *= 2;
			lines = realloc(lines,*lcount*sizeof(char *));
		}
		lines[c] = strdup(tmp);
		REMOVE_NEWLINE(lines[c])
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
		printf("%s\n",ret );
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