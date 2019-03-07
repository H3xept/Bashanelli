#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

static struct aliaslist *generate_alias_item(const char *command, const char *alias);
static struct aliaslist *is_alias(const char *alias);

struct aliaslist{
	char *com;
	char *ali;
	struct aliaslist *next;
};

static struct aliaslist *head = NULL;
static struct aliaslist *tail = NULL; 
static unsigned int length = 0;

void init_aliases() {
	head = NULL;
	tail = NULL;
	length = 0;
}

int get_aliaslist_length(){
	return length;
}

void add_alias(const char *alias, const char *command) {
	struct aliaslist *new = generate_alias_item(command, alias);
	struct aliaslist *existing = is_alias(alias);
	if(!tail){
		tail = new;
		head = new;
		length++;
	}
	else if(!existing) {
		head->next = new;
		head = new;
		length++;
	}
	else {
		existing->ali = realloc(existing->ali, (strlen(alias) + 1) * sizeof(char));
		strcpy(existing->ali, alias);
		existing->com = realloc(existing->com, (strlen(command) + 1) * sizeof(char));
		strcpy(existing->com, command);
		free(new);
	}

}

static struct aliaslist *generate_alias_item(const char *command, const char *alias) {
	char *c = calloc(strlen(command),sizeof(char));
	strcpy(c,command);
	char *a = calloc(strlen(alias),sizeof(char));
	strcpy(a,alias);
	struct aliaslist *new = calloc(1,sizeof(struct aliaslist));
	new->com = c;
	new->ali = a;
	new->next = NULL;
	return new;
}

static struct aliaslist *is_alias(const char *alias){
	struct aliaslist *current = tail;
	while(current){
		if (!strcmp(current->ali,alias)){
			return current;
		}
		current = current->next;
	}
	return NULL;
}


void print_aliaslist(){
	struct aliaslist *current = tail;
	while(current){
		printf("alias %s = '%s'\n",current->ali,current->com);
		current = current->next;
	}
}