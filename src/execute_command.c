#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <BareBonesReadline/readline.h>

#include "execute_command.h"
#include "script_handling.h"
#include "parse_commands.h"
#include "builtins.h"
#include "argv.h"
#include "constants.h"

#define SIGNATURE_MACH_O 0xfeedface
#define SIGNATURE_MACH_O_64 0xfeedfacf
#define SIGNATURE_MACH_O_REVERSE 0xcefaedfe
#define SIGNATURE_MACH_O_64_REVERSE 0xcffaedfe

#define SIGNATURE_ELF 0x7f454c46
#define SIGNATURE_ELF_REVERSE 0x464c457f

static int recent_exit_code = 0;

static int execute_pipeline_node(const PipelineNode* node, int in_fd) {
	
	int exec_in_fd = in_fd;
	int exec_out_fd = NO_FD;
	int ret_in_fd = NO_FD;

	FILE* in_file = NULL;
	FILE* out_file = NULL;

	if(in_fd == NO_FD) {
		if(node->in) {
			in_file = fopen(node->in, "r");
			if(!in_file) return NO_INFILE;
			exec_in_fd = fileno(in_file);
		}
	}

	if(node->out){
		if (node->omode == OUT_MODE_OW)
			out_file = fopen(node->out, "w");
		else 
			out_file = fopen(node->out, "wa");

		if(!out_file) return NO_OUTFILE;
		exec_out_fd = fileno(out_file);
	} else if (node->next) {
		int fd[2];
		pipe(fd);
		exec_out_fd = fd[1];
		ret_in_fd = fd[0];
	}

	char** argv = generate_argv(node->cmd);
	printf("Command: %s\n",node->cmd);
	execute_command((const char**)argv, exec_in_fd, exec_out_fd);

	return ret_in_fd;

}

static void execute_pipeline(const PipelineNode* pipeline) {
	const PipelineNode* current = pipeline;
	int fd = NO_FD;
 	do {
 		fd = execute_pipeline_node(current, fd);
 		current = current->next;
 	}
 	while(current);
}

void execute_pipelines(int pipelines_n, const PipelineNode** pipelines) {
	for (int i = 0; i < pipelines_n; ++i) {
		execute_pipeline(pipelines[i]);
	}
}

void execute_command(const char** argv, int in_fd, int out_fd) {
	if(!argv || !*argv) return;

	if(is_builtin(argv[0])){
		execute_builtin(argv[0], argv, in_fd, out_fd);
	}
	else{
		const char* full_filepath = file_path(argv[0]);
		if(full_filepath){
			if(is_executable(full_filepath)){
				printf("INFD: %d | OUTFD: %d\n",in_fd, out_fd);
				execute_bin(full_filepath, argv, in_fd, out_fd);
			}
			else{
				execute_shell_script(full_filepath, argv, in_fd, out_fd);
			}
			free((char*)full_filepath);
			return;
		}
		if(file_exists(argv[0])){
			if(is_executable(argv[0])){
				execute_bin(argv[0], argv, in_fd, out_fd);
			}
			else{
				if(is_dir(argv[0])){
					printf("bashanelli: %s: Is a directory\n", argv[0]);
					return;
				}
				execute_shell_script(argv[0], argv, in_fd, out_fd);
			}
		}
		else{
			printf("%s: command not found\n", argv[0]);
		}
	}
}

void parse_and_execute_command(const char* command){
	const char* parsed_line = parse_line((char*) command);
	int p_n;
	const PipelineNode** pipelines = (const PipelineNode**) parse_command(&p_n, parsed_line);
	execute_pipelines(p_n, pipelines);
	
	#pragma message("Free me:C")

	free((char*)parsed_line);
}

void execute_builtin(const char* command, const char** argv, int in_fd, int out_fd) {

	pid_t pid = vfork();

	if(!pid){

		int s_stdout = dup(STDOUT_FILENO);
		int s_stdin = dup(STDIN_FILENO);

		if (in_fd != NO_FD) dup2(in_fd, 0);
		if (out_fd != NO_FD) dup2(out_fd, 1);

		recent_exit_code = exec_builtin_str(command, argv);

		dup2(s_stdin, 0);
		dup2(s_stdout, 1);

		exit(0);
	}
	else{
		waitpid(pid, NULL, 0);
	}

}

void execute_shell_script(const char* filename, const char** argv, int in_fd, int out_fd) {
	if(!filename){
		printf("No filename given");
		recent_exit_code = -1;
		return;
	}
	if(!file_exists(filename)){
		printf("No such file: %s\n", filename);
		recent_exit_code = -1;
		return;
	}
	if(is_executable(filename)){
		printf("Cannot run executable file as script: %s\n", filename);
		recent_exit_code = -1;
		return;
	}
	int i = 0;
	while(*(argv + i)){
		i++;
	}
	pid_t pid = vfork();
	if(!pid){
		
		int s_stdout = dup(STDOUT_FILENO);
		int s_stdin = dup(STDIN_FILENO);

		if (in_fd != NO_FD) dup2(in_fd, 0);
		if (out_fd != NO_FD) dup2(out_fd, 1);

		push_argv_frame(argv, i);
		handle_script(filename);
		pop_argv_frame();

		dup2(s_stdin, 0);
		dup2(s_stdout, 1);

		exit(0);
	}
	else{
		waitpid(pid, NULL, 0);
	}

}

void execute_bin(const char* filename, const char** argv, int in_fd, int out_fd)  {
	pid_t pid = vfork();
	if(!pid){

		int s_stdout = dup(STDOUT_FILENO);
		int s_stdin = dup(STDIN_FILENO);

		if (in_fd != NO_FD) dup2(in_fd, 0);
		if (out_fd != NO_FD) dup2(out_fd, 1);

		recent_exit_code = execvp(filename, ((char* const *)argv));
		if(recent_exit_code == -1){
			printf("%s: program not found\n", filename);
			fflush(stdout);
		};

		dup2(s_stdin, 0);
		dup2(s_stdout, 1);

		exit(0);
	}
	else{
		waitpid(pid, NULL, 0);
	}
}

int is_builtin(const char* const command){
	return builtin_id(command) >= 0;
}

int is_executable(const char* const filename){
	FILE* f = fopen(filename, "rb");
	if(!f){
		return 0;
	}
	uint32_t signature = 0;
	fread(&signature, 1, sizeof(uint32_t), f);
	fclose(f);
	#ifdef __MACH__
	return signature == SIGNATURE_MACH_O
		|| signature == SIGNATURE_MACH_O_REVERSE
		|| signature == SIGNATURE_MACH_O_64
		|| signature == SIGNATURE_MACH_O_64_REVERSE;
	#endif
	return signature == SIGNATURE_ELF
		|| signature == SIGNATURE_ELF_REVERSE;
}


int file_exists(const char* const filename){
	FILE* f = fopen(filename, "rb");
	if(!f) return 0;
	fclose(f);
	return 1;
}

int is_dir(const char* path){
	struct stat st;
	stat(path, &st);
	return !S_ISREG(st.st_mode);
}

int get_exit_code(){
	return recent_exit_code;
}

// Returns the full path of a file present in a directory specified by the PATH env variable.
const char* file_path(const char* filename){
	for(int i = 0; i < strlen(filename); i++){
		if(*(filename + i) == '/'){
			return NULL;
		}
	}

	char* path_list = getenv(PATH_ENV);
	char* path_pointer = path_list;
	if(path_pointer){
		do{	
			int i = 0;
			while(*(path_pointer + i) != ':' && *(path_pointer + i) != '\0'){
				i++;
			}
			char* path = calloc(i + strlen(filename) + strlen(path_pointer) + 1, sizeof(char));
			if(*(path_pointer + i) == ':'){
				*(path_pointer + i) = '\0';
				strcpy(path, path_pointer);
				*(path_pointer + i) = ':';
			}
			else{
				strcpy(path, path_pointer);
			}
			path_pointer++;
			*(path + i) = '/';
			*(path + i + 1) = '\0';
			strcat(path, filename);
			if(file_exists(path)){
				return path;
			}
			free(path);
			path_pointer = strchr(path_pointer, ':');
			if(!path_pointer){
				break;
			}
			path_pointer++;
		}
		while(*path_pointer);
	}
	return NULL;
}
