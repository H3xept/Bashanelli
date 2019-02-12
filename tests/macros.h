#ifndef __UT_MACROS__
#define __UT_MACROS__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ANSIsACurse/characters.h>

#define _test_failed_ex_rec(line_n, expected, received) \
	c_color(C_COLOR_CODE_Red);\
	r_printf(" x - line: %d\n| Expected: %s\n| Received: %s\n", line_n, expected, received);\
	c_reset();\
	errno = -1;

#define _test_failed(line_n) \
	c_color(C_COLOR_CODE_Red);\
	printf(" x - line: %d\n", line_n);\
	c_reset();\
	errno = -1;

#define _test_passed() \
	c_color(C_COLOR_CODE_Green);\
	printf(" ✓\n", NULL);\
	c_reset();

#define run_test(test_func) \
	printf("-> %s",#test_func); \
	test_func(); \
	if (!errno) {_test_passed();}

#define assert_true_ex_rec(cond, expected, received) \
	if (!(cond)) {_test_failed_ex_rec(__LINE__, expected, received); } 

#define assert_true(cond) \
	if (!(cond)) {_test_failed(__LINE__); } 

#define assert_false(cond) \
	assert_true(!(cond))

#define assert_eq_num(int1, int2) \
	assert_true_ex_rec(int1==int2, #int1, #int2)

#define assert_eq_str(str1, str2) \
	assert_true_ex_rec(!strcmp(str1, str2), str1, str2)

#endif