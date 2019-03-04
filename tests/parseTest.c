#include "macros.h"
#include "parseTest.h"
#include "../src/parse_commands.h"
#include "../src/execute_command.h"

void setup(){
	return;
}

void test_count_occ(){
	char str[] = "a b aac aifonai a oisd a";
	int i = count_occ(str, 'a');
	assert_eq_int(i,7);
}

void test_generate_argv() {
	char str[] = "ls -a";
	char **args = generate_argv(str);
	assert_eq_str(*args,"ls");
	assert_eq_str(*(args+1),"-a");
}

void test_generate_argv_with_quotes() {
	char str[] = {  '"',  'l',  's',  ' ',  '-',  'a',  '"',  ' ',  'x',  '"',  'y',  ' ',  '"',  'z',  '\0'  };
	char **args = generate_argv(str);
	assert_eq_str(*args,"ls -a");
	assert_eq_str(*(args+1),"xy z");
}

void test_generate_argv_with_quotes2() {
	char str[] = {  '"',  'l',  's',  '"', ' ',  '-',  'a',  ' ',  ' ',  '"',  'x',  'y',  ' ',  '"',  'z',  '\0'  };
	char **args = generate_argv(str);
	assert_eq_str(*args,"ls");
	assert_eq_str(*(args+1),"-a");
	assert_eq_str(*(args+2),"xy z");
}

void test_generate_argv_with_quotes_and_comment() {
	char str[] = {  '"',  '"',  'l',  's',  ' ',  'a',  ' ',  'b', ' ',  '#',  'a',  'b',  '\0'  };
	char **args = generate_argv(str);
	assert_eq_str(*args,"ls");
	assert_eq_str(*(args+1),"a");
	assert_eq_str(*(args+2),"b");
}

void test_generate_argv_with_non_comment() {
	char str[] = {  '"',  '"',  'l',  's',  ' ',  'a',  ' ',  'b',  '#',  'a',  'b',  '\0'  };
	char **args = generate_argv(str);
	assert_eq_str(*args,"ls");
	assert_eq_str(*(args+1),"a");
	assert_eq_str(*(args+2),"b#ab");
}

void test_generate_argv_one_quoted_arg() {
	char str[] = {  '"',  'l',  's',  ' ',  '-',  'a',  '"',  ' ',  'x',  'y', 'z',  ' ',  'a',  '\0'  };
	char **args = generate_argv(str);
	assert_eq_str(*args,"ls -a");
	assert_eq_str(*(args+1),"xyz");
	assert_eq_str(*(args+2),"a");
}

void test_generate_argv_backslash_spaces() {
	char str[] = {  'l',  's',  '\\',  ' ',  'a',  ' ',  'b',  '\0'  };
	char **args = generate_argv(str);
	assert_eq_str(*args,"ls a");
	assert_eq_str(*(args+1),"b");
}

void test_generate_argv_another_backslash_escape() {
	char str[] = {  'l',  's',  '\\',  'b',  'a',  ' ',  'b',  '\0'  };
	char **args = generate_argv(str);
	assert_eq_str(*args,"lsba");
	assert_eq_str(*(args+1),"b");
}

void test_file_exists() {
	FILE *fp = fopen("loltest","w");
	assert_true(file_exists("loltest"));
	remove("loltest");
}

void test_trim_outside_whitespace() {
	char str1[] = "     some words    ";
	char *str2 = trim_whitespace(str1);
	assert_eq_str(str2,"some words");
}

void teardown(){
	return;
}