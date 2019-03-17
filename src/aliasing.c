/**
 * File: aliasing.c
 *
 * Description: Handles command aliasing
 * 
 * Date: 
 * 15/3/2019
 * 
 * Authors: 
 *	1. Leonardo Cascianelli (me.leonardocascianelli@gmail.com)
 *	2. Rory Brown (rorybrown660@gmail.com)
 *	3. Ewan Skene (ewancskene@gmail.com)
 * 
 */

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "aliasing.h"

static struct alias_entry *generate_alias_item(const char *command, const char *alias);
static struct alias_entry *get_alias_entry(const char *alias);
static int sa_contains(char **string_array, char *search_string);

struct alias_entry{
	char *com;
	char *ali;
	struct alias_entry *next;
};

static struct alias_entry *head = NULL;
static struct alias_entry *tail = NULL; 
static unsigned int length = 0;

// inits alias variables
void init_aliases() {
	head = NULL;
	tail = NULL;
	length = 0;
}

// gets number of aliases set
int get_aliaslist_length(){
	return length;
}

// set new alias
// e.g alias [alias]=[command]
void add_alias(const char *alias, const char *command) {
	struct alias_entry *new = generate_alias_item(command, alias);
	struct alias_entry *existing = get_alias_entry(alias);
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
		char *tempali = existing->ali;
		existing->ali = calloc((strlen(alias) + 1), sizeof(char));
		strcpy(existing->ali, alias);
		free(tempali);
		char *tempcom = existing->com;
		existing->com = calloc((strlen(command) + 1), sizeof(char));
		strcpy(existing->com, command);
		free(tempcom);
		free(new);
	}
}

// remove alias from list
int remove_alias(const char *alias) {
	if (length == 0) {
		return 1;
	}
	struct alias_entry *current = tail;
	if(!strcmp(current->ali, alias)){
		tail = current->next;
		free(current->ali);
		free(current->com);
		free(current);
		length--;
		return 0;
	}
	while(current->next) {
		if(!strcmp(current->next->ali,alias)){
			struct alias_entry *temp = current->next->next;
			free(current->next->ali);
			free(current->next->com);
			free(current->next);
			current->next = temp;
			length--;
			return 0;
		}
		current = current->next;
	}
	return 1;
}

// prints a list of all alias and direct translations
// e.g. given 'a=b' 'b=c' 'c=a'
// out: alias a = 'b' \n ... etc
void print_aliaslist(){
	if(!tail){
		printf("No aliases found\n");
	}
	struct alias_entry *current = tail;
	while(current){
		printf("alias %s = '%s'\n",current->ali,current->com);
		current = current->next;
	}
}

// prints direct translation of alias
// e.g. given 'a=b' 'b=c' 'c=a'
// prints alias a = 'b'
int print_alias(const char *alias) {
	struct alias_entry *tmp = get_alias_entry(alias);
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

// gets the true value of an alias after recursing
// (terminates if loop detected)
// e.g. given 'a=b' 'b=c' 'c=a'
// when input alias is 'a', return would be 'a'
char *resolve_alias(char *alias) {
	if(!alias || !*alias){
		return NULL;
	}
	char **hist = calloc(1,sizeof(char*));
	size_t r = 0;
	char *ret = NULL;
	struct alias_entry *current = get_alias_entry(alias);
	while(current) {
		if (sa_contains(hist, current->ali)) {//checknull
			ret = realloc(ret, (strlen(current->ali)+1)*sizeof(char));
			strcpy(ret, current->ali);
			return ret;
		}
		else {
			*(hist+r) = calloc(strlen(current->ali)+1,sizeof(char)); 
			strcpy(*(hist+r),current->ali);
			r++;
			hist = realloc(hist, (r+1)*sizeof(char*)); //keep null terminated
			memset(hist+r, 0x0, sizeof(char*));
			ret = realloc(ret, (strlen(current->com)+1)*sizeof(char));
			strcpy(ret, current->com);
		}
		current = get_alias_entry(current->com);
	}
	if (hist) {
		free(hist);	
		hist = 0;	
	}
	return ret;
}

// clear and free all aliases
void teardown_aliases() {
	if (length == 0) {
		return;
	}
	struct alias_entry *current = tail;
	while(current) {
			struct alias_entry *temp = current->next;
			free(current);
			current = temp;
			length--;
	}
	head = NULL;
	tail = NULL;
	return;
}

// Allocs sting space for alias list item
static struct alias_entry *generate_alias_item(const char *command, const char *alias) {
	char *c = calloc(strlen(command) + 1,sizeof(char));
	strcpy(c,command);
	char *a = calloc(strlen(alias) + 1,sizeof(char));
	strcpy(a,alias);
	struct alias_entry *new = calloc(1,sizeof(struct alias_entry));
	new->com = c;
	new->ali = a;
	new->next = NULL;
	return new;
}

// Returns an entry specified by name via string alias
// Returns NULL if no entry is found
static struct alias_entry *get_alias_entry(const char *alias){
	if(!alias){
		return NULL;
	}
	struct alias_entry *current = tail;
	while(current){
		if (!strcmp(current->ali,alias)){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

// Returns true if search_string exists inside string_array
static int sa_contains(char **string_array, char *search_string){
	assert(search_string && *search_string);

	int i = 0;
	while(*(string_array+i)) {
		if(!strcmp(*(string_array+i),search_string)){
			return 1;
		}
		i++;
	}
	return 0;
}
