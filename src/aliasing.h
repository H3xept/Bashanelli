/**
 * File: aliasing.h
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

#ifndef __ALIASING__
#define __ALIASING__

// inits alias variables
void init_aliases();

// gets number of aliases set
int get_aliaslist_length();

// set new alias
// e.g alias [alias]=[command]
void add_alias(const char *alias, const char *command);

// remove alias from list
int remove_alias(const char *alias);

// prints a list of all alias and direct translations
// e.g. given 'a=b' 'b=c' 'c=a'
// out: alias a = 'b' \n ... etc
void print_aliaslist();

// prints direct translation of alias
// e.g. given 'a=b' 'b=c' 'c=a'
// prints alias a = 'b'
int print_alias(const char *alias);

// gets the true value of an alias after recursing
// (terminates if loop detected)
// e.g. given 'a=b' 'b=c' 'c=a'
// when input alias is 'a', return would be 'a'
char *resolve_alias(char *alias);

// clear and free all aliases
void teardown_aliases();

#endif
