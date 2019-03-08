#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "aliasing.h"

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

int remove_alias(const char *alias) {
	if (length == 0) {
		return 1;
	}
	struct aliaslist *current = tail;
	if(!strcmp(current->ali, alias)){
		tail = current->next;
		free(current);
		length--;
		return 0;
	}
	while(current->next) {
		if(!strcmp(current->next->ali,alias)){
			struct aliaslist *temp = current->next->next;
			free(current->next);
			current->next = temp;
			length--;
			return 0;
		}
		current = current->next;
	}
	return 1;
}

void print_aliaslist(){
	if(!tail){
		printf("\n");
	}
	struct aliaslist *current = tail;
	while(current){
		printf("alias %s = '%s'\n",current->ali,current->com);
		current = current->next;
	}
}

int print_alias(const char *alias) {
	char *command = expand_alias(alias);
	if(command){
		printf("alias %s = '%s'\n", alias, command);
		free(command);
		return 0;
	}
	else {
		return 1;
	}
}

char *expand_alias(char *alias) {
	if(!alias || !*alias){
		return NULL;
	}
	char *track = calloc(2,sizeof(char));
	strcpy(track, "#");
	char *ret = NULL;
	struct aliaslist *current = is_alias(alias);
	while(current) {
		if (track && strstr(track, current->ali)) {
			ret = realloc(ret, (strlen(current->ali)+1)*sizeof(char));
			strcpy(ret, current->ali);
			return ret;
		}
		else {
			int l = (track) ? strlen(track) : 0;  
			track = realloc(track,(l+strlen(current->ali)+2)*sizeof(char));
			strcat(track,current->ali);
			strcat(track,"#");
			ret = realloc(ret, (strlen(current->ali)+1)*sizeof(char));
			strcpy(ret, current->com);
		}
		current = is_alias(current->com);
	}
	if (track) {
		free(track);		
	}

	return ret;
}

void teardown_aliases() {
	if (length == 0) {
		return;
	}
	struct aliaslist *current = tail;
	while(current) {
			struct aliaslist *temp = current->next;
			free(current);
			current = temp;
			length--;
	}
	head = NULL;
	tail = NULL;
	return;
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
	if(!alias){
		printf("nullboi\n");
		return NULL;
	}
	struct aliaslist *current = tail;
	while(current){
		if (!strcmp(current->ali,alias)){
			return current;
		}
		current = current->next;
	}
	return NULL;
}
