/**
 * File: argv.h
 *
 * Description: Manages a stack of argv frames, so the shell
 * can restore a previous argv after execution of a shell script.
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

#ifndef __ARGV__
#define __ARGV__

// Initialises the argv stack
void init_argv();

// Pushes a new argv frame to the stack
void push_argv_frame(const char** argv, const int argc);

// Removes the top argv frame from the stack
void pop_argv_frame();

// Returns the argv pointer from the current frame
char** get_current_argv();

// Returns an indexed arg from the current argv frame
char* get_arg_from_current_argv(const int index);

#endif
