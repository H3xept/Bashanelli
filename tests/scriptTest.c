#include "macros.h"
#include "scriptTest.h"
#include "../src/script_handling.h"

void setup(){
	return;
}


void test_read_empty_file(){
	FILE *fp = fopen("loltest","w");
	fclose(fp);
	int c;
	read_file("loltest",&c);
	assert_eq_int(c,0);
	remove("loltest");
}

void test_read_five_lines(){
	FILE *fp = fopen("loltest","w");
	for(int i =0; i < 5; i++){
		fputs("lol\n",fp);
	}
	fclose(fp);
	int c;
	read_file("loltest",&c);
	remove("loltest");
	assert_true(c == 5);
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
	assert_true(count > 200);
}

void test_run_script_in_path() {
	int ret = handle_script("ls");
	assert_eq_int(ret,0);
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