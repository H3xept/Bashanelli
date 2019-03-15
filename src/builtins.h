/**
 * File: builtins.h
 *
 * Description: Contains definitions of builtins used by Bashanelli
 * Also contains functions that can be called to execute builtins by
 * name or id.
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

#ifndef __BUILTINS__
#define __BUILTINS__

// Retrieves the id associated with a builtin by name
int builtin_id(const char* name);

// Executes a builtin with a given name, passing a given argv
void exec_builtin_str(const char* name, const char** argv);

// Executes a builtin with a given id, passing a given argv
void exec_builtin_id(const int id, const char** argv);

#endif
