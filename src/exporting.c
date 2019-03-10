#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "exporting.h"

static struct exportlist *generate_export(const char *value, const char *name);
static struct exportlist *is_export(const char *name);
static void retrieve_environ();

static struct exportlist *head = NULL;
static struct exportlist *tail = NULL; 
static unsigned int length = 0;

struct exportlist{
	char *val;
	char *name;
	struct exportlist *next;
};

void init_exports() {
	head = NULL;
	tail = NULL;
	length = 0;
	retrieve_environ();
}

int get_exportlist_length(){
	return length;
}

void add_export(const char *name, const char *value) {
	struct exportlist *new = generate_export(value, name);
	struct exportlist *existing = is_export(name);
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
		char *tempali = existing->name;
		existing->name = calloc((strlen(name) + 1), sizeof(char));
		strcpy(existing->name, name);
		free(tempali);
		char *tempcom = existing->val;
		existing->val = calloc((strlen(value) + 1), sizeof(char));
		strcpy(existing->val, value);
		free(tempcom);
		free(new);
	}
	setenv(name, value, 1);
}

int remove_export(const char *name) {
	if (length == 0) {
		return 1;
	}
	struct exportlist *current = tail;
	if(!strcmp(current->name, name)){
		tail = current->next;
		free(current->name);
		free(current->val);
		free(current);
		length--;
		unsetenv(name);
		return 0;
	}
	while(current->next) {
		if(!strcmp(current->next->name,name)){
			struct exportlist *temp = current->next->next;
			free(current->next->name);
			free(current->next->val);
			free(current->next);
			current->next = temp;
			length--;
			unsetenv(name);
			return 0;
		}
		current = current->next;
	}
	return 1;
}

void print_exportlist(){
	if(!tail){
		printf("\n");
	}
	struct exportlist *current = tail;
	while(current){
		printf("declare -x %s='%s'\n",current->name,current->val);
		current = current->next;
	}
}

int print_export_var(const char *name) {
	struct exportlist *tmp = getenv(name);
	char *value = NULL;
	if(tmp){
		value = tmp->val;		
	}
	if(value){
		printf("declare -x  %s='%s'\n", name, value);
		return 0;
	}
	else {
		return 1;
	}
}

void teardown_exports() {
	if (length == 0) {
		return;
	}
	struct exportlist *current = tail;
	while(current) {
			struct exportlist *temp = current->next;
			free(current);
			current = temp;
			length--;
	}
	head = NULL;
	tail = NULL;
	return;
}

char *get_export_value(const char *name){
	struct exportlist *e = is_export(name);
	if(e){
		char *ret = calloc( strlen(e->val) + 1, sizeof(char) );
		strcpy(ret, e->val);
		return ret;
	}
	return NULL;
}

static struct exportlist *is_export(const char *name){
	if(!name){
		return NULL;
	}
	struct exportlist *current = tail;
	while(current){
		if (!strcmp(current->name,name)){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

static struct exportlist *generate_export(const char *value, const char *name) {
	char *c = calloc(strlen(value),sizeof(char));
	strcpy(c,value);
	char *a = calloc(strlen(name),sizeof(char));
	strcpy(a,name);
	struct exportlist *new = calloc(1,sizeof(struct exportlist));
	new->val = c;
	new->name = a;
	new->next = NULL;
	return new;
}

static void retrieve_environ(){
	int i = 0;
	char *p = *(__environ+i);
	while(p){
		char *ep = strchr(p, '=');
		if(ep) {
			char *tmp = calloc(strlen(p) - strlen(ep) + 1,sizeof(char));
			strncpy(tmp, p, strlen(p) - strlen(ep));
			add_export(tmp, ep+1);
			free(tmp);
		}
		i++;
		p = *(__environ+i);
	}
}
