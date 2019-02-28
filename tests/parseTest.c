#include "macros.h"
#include "parseTest.h"
#include "../src/parse_commands.h"


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

void test_file_exists(){
	FILE *fp = fopen("loltest","w");
	assert_true(file_exists("loltest"));
	remove("loltest");
}

void teardown(){
	return;
}