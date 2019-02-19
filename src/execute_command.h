#define SIGNATURE_MACH_O 0xfeedface
#define SIGNATURE_MACH_O_REVERSE 0xcefaedfe
#define SIGNATURE_ELF 0x7f454c46
#define SIGNATURE_ELF_REVERSE 0x464c457f

void execute_command(char* command);
void execute_builtin(char* command, char** argv);
void execute_shell_script(char* filename, char** argv);
void execute_bin(char* filename, char** argv);