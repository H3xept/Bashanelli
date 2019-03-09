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
static int srec(const char **hist, char *alias);

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
	struct aliaslist *tmp = is_alias(alias);
	char *command = NULL;
	if(tmp){
		command = tmp->com;		
	}
	if(command){
		printf("alias %s = '%s'\n", alias, command);
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
	char **hist = calloc(1,sizeof(char*));
	int r = 0;
	char *ret = NULL;
	struct aliaslist *current = is_alias(alias);
	while(current) {
		if (srec(hist, current->ali)) {//checknull
			ret = realloc(ret, (strlen(current->ali)+1)*sizeof(char));
			strcpy(ret, current->ali);
			return ret;
		}
		else {
			*(hist+r) = calloc(strlen(current->ali)+1,sizeof(char)); 
			strcpy(*(hist+r),current->ali);
			r++;
			hist = realloc(hist, r*sizeof(char*));
			*(hist+r) = NULL;
			ret = realloc(ret, (strlen(current->ali)+1)*sizeof(char));
			strcpy(ret, current->com);
		}
		current = is_alias(current->com);
	}
	if (hist) {
		free(hist);		
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

static int srec(const char **hist, char *alias){
	int i = 0;
	while(*(hist+i)) {
		if(!strcmp(*(hist+i),alias)){
			return 1;
		}
	}
	return 0;
}
