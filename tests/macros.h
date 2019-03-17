#ifndef __UT_MACROS__
#define __UT_MACROS__

#include <ANSIsACurse/characters.h>
#include <ANSIsACurse/cursor.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

#define EXPLANATION_SEPARATOR "-------------------"
#define __explanation_separator() printf(EXPLANATION_SEPARATOR "\n")

#define run_test(test_func) {\
	printf("-> %s \n",#test_func);\
	pid_t pid = fork();\
	int status;\
	if (!pid) {\
		test_func();\
		exit(0);\
	} else {\
		waitpid(pid, &status, 0);\
		if (WEXITSTATUS(status) != 0) {\
			c_color(31);\
			c_decorate(C_DECORATION_Bold);\
			printf("Test %s failed!\n",#test_func);\
			c_color(0);\
			errno = -1;\
		} else if (WIFEXITED(status)){\
			\
		} else if (WIFSIGNALED(status)){\
			c_color(31);\
			c_decorate(C_DECORATION_Bold);\
			printf("[%s] forcefully terminated.\nSignal: %s\n",#test_func,strsignal(WTERMSIG(status)));\
			c_color(0);\
			errno = -1;\
		}\
	}\
}

#define assert_true(COND) {\
	if (!(COND)) {\
		__explanation_separator();\
		printf("Expected true condition.\n");\
		printf("Line: %d - %s\n",__LINE__, __FILE__);\
		__explanation_separator();\
		exit(-1);\
	}\
}

#define assert_eq_int(NUM1, NUM2) {\
	if ((NUM1 != NUM2)) {\
		__explanation_separator();\
		printf("Expected %d, got %d \n",NUM1,NUM2);\
		printf("Line: %d - %s\n",__LINE__, __FILE__);\
		__explanation_separator();\
		exit(-1);\
	}\
}

#define assert_eq_float(NUM1, NUM2) {\
	if ((NUM1 != NUM2)) {\
		__explanation_separator();\
		printf("Expected %f | got %f \n",NUM1,NUM2);\
		printf("Line: %d - %s\n",__LINE__, __FILE__);\
		__explanation_separator();\
		exit(-1);\
	}\
}

#define assert_eq_double(NUM1, NUM2) {\
	if ((NUM1 != NUM2)) {\
		__explanation_separator();\
		printf("Expected %ld | got %ld \n",NUM1,NUM2);\
		printf("Line: %d - %s\n",__LINE__, __FILE__);\
		__explanation_separator();\
		exit(-1);\
	}\
}

#define assert_eq_str(S1, S2) {\
	if ((S1) == NULL || (S2) == NULL) {\
		__explanation_separator();	\
		printf("One of the passed strings is NULL!\n");\
		__explanation_separator();	\
		exit(-1);\
	} else\
	if (strcmp(S1,S2)) {\
		__explanation_separator();\
		printf("Expected %s | got %s \n",S1,S2);\
		printf("Line: %d - %s\n",__LINE__, __FILE__);\
		__explanation_separator();\
		exit(-1);\
	}\
}

#define assert_null(P) {\
	if (P != NULL) {\
		__explanation_separator();\
		printf("Expected NULL\n");\
		printf("Line: %d - %s\n",__LINE__, __FILE__);\
		__explanation_separator();\
		exit(-1);\
	}\
}

#define assert_not_null(P) {\
	if (P == NULL) {\
		__explanation_separator();\
		printf("Expected NOT NULL\n");\
		printf("Line: %d - %s\n",__LINE__, __FILE__);\
		__explanation_separator();\
		exit(-1);\
	}\
}

#endif
