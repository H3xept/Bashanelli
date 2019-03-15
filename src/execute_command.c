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

void execute_command(const char** argv){
	if(!argv || !*argv){
		return;
	}
	if(is_builtin(argv[0])){
		execute_builtin(argv[0], argv);
	}
	else{
		const char* full_filepath = file_path(argv[0]);
		if(full_filepath){
			if(is_executable(full_filepath)){
				execute_bin(full_filepath, argv);
			}
			else{
				execute_shell_script(full_filepath, argv);
			}
			free((char*)full_filepath);
			return;
		}
		if(file_exists(argv[0])){
			if(is_executable(argv[0])){
				execute_bin(argv[0], argv);
			}
			else{
				if(is_dir(argv[0])){
					printf("bashanelli: %s: Is a directory\n", argv[0]);
					return;
				}
				execute_shell_script(argv[0], argv);
			}
		}
		else{
			printf("%s: command not found\n", argv[0]);
		}
	}
}

void parse_and_execute_command(const char* command){
	char* parsed_line = parse_line(command);
	char** argv = parse_command(parsed_line);
	execute_command((const char**)argv);
	if(argv){
		int i = 0;
		while(*(argv+i)) {
			free(*(argv+i));
			i++;
		}
		free(argv);
	}
	free(parsed_line);
}

void execute_builtin(const char* command, const char** argv){
	recent_exit_code = exec_builtin_str(command, argv);
}

void execute_shell_script(const char* filename, const char** argv){
	//printf("Shell script argv support not implemented. Executing without args...\n");
	if(!filename){
		printf("No filename given", filename);
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
	push_argv_frame(argv, i);
	handle_script(filename);
	pop_argv_frame();

}

void execute_bin(const char* filename, const char** argv) {
	pid_t pid = fork();
	if(!pid){
		recent_exit_code = execvp(filename, ((char* const *)argv));
		if(recent_exit_code == -1){
			printf("%s: command not found\n", filename);
			fflush(stdout);
		};
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
