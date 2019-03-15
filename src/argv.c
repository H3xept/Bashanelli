/**
 * File: argv.c
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

#include <stdlib.h>
#include <string.h>

struct argv_frame{
	struct argv_frame* next;
	char** argv;
	int argc;
};

static unsigned int length;
static struct argv_frame* start;
static struct argv_frame* end;

// Initialises the argv stack
void init_argv(){
	start = NULL;
	end = NULL;
	length = 0;
}

// Pushes a new argv frame to the stack
void push_argv_frame(const char** argv, const int argc){
	struct argv_frame* frame = calloc(1, sizeof(struct argv_frame));
	frame->argv = calloc(argc, sizeof(char*));
	for(int i = 0; i < argc; i++){
		*(frame->argv + i) = calloc(strlen(*(argv + i)) + 1, sizeof(char));
		strcpy(*(frame->argv + i), *(argv + i));
	}

	frame->argc = argc;
	frame->next = NULL;
	if(end){
		end->next = frame;
	}
	else{
		start = frame;
	}
	end = frame;
	length++;

}

// Removes the top argv frame from the stack
void pop_argv_frame(){
	if(!end){
		return;
	}
	struct argv_frame* frame = end;
	struct argv_frame* current = start;
	while(current->next->next){
		current = current->next;
	}
	current->next = NULL;
	end = current;
	length--;
	for(int i = 0; i < frame->argc; i++){
		free(*(frame->argv + i));
	}
	free(frame->argv);
	free(frame);
}

// Returns the argv pointer from the current frame
char** get_current_argv(){
	return end->argv;
}

// Returns an indexed arg from the current argv frame
char* get_arg_from_current_argv(const int index){
	if(index >= (end->argc) || index < 0){
		return " ";
	}
	return *(end->argv + index);
}
