#include "macros.h"
#include "scriptTest.h"
#include "../src/script_handling.h"

void setup(){
	return;
}


void test_read_empty_file(){
	return;
}

void test_read_README(){
	int c;
	read_file("README.md",&c);
	assert_true(c > 0);
}

void test_read_non_existing_file(){
	unsigned int count;
	read_file("nudes",&count);
	assert_eq_int(count,0);
}

void test_run_garbage_script(){
	int result = handle_script("oogabooga");
	assert_eq_int(result,1);
}

void test_read_file_in_path() {
	unsigned int count;
	char *filepath = search_path("ls");
	read_file(filepath,&count);
	assert_eq_int(count,466); //the number of lines in the ls file
}

void test_run_script_in_path() {
	handle_script("ls");
}

void test_read_in_current_dir() {
	int c;
	read_file("./README.md",&c);
	assert_true(c > 0);
}

void test_read_in_parent_dir() {
	int c;
	read_file("../Makefile",&c);
	assert_true(c > 0);
}

void teardown(){
	return;
}