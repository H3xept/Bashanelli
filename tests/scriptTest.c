#include <BareBonesReadline/readline.h>
#include "macros.h"
#include "scriptTest.h"
#include "../src/script_handling.h"

void setup(){
	init_readline(NULL);
	return;
}

void test_read_empty_file(){
	FILE *fp = fopen("loltest","w");
	fclose(fp);
	unsigned int c;
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
	unsigned int c;
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
	assert_true(count > 0);
}

void test_run_script_ls() {
	FILE *fp = fopen("lstest","w");
	fputs("ls\n",fp);
	fputs("ls -a\n",fp);	
	fclose(fp);
	int ret = handle_script("./lstest");
	assert_eq_int(ret,0);
	remove("lstest");
}

void test_run_script_ls_with_comments() {
	FILE *fp = fopen("lstest","w");
	fputs("ls #lol\n",fp);
	fputs("#ls -a\n",fp);
	fputs("ls # -a\n",fp);	
	fclose(fp);
	int ret = handle_script("./lstest");
	assert_eq_int(ret,0);
	remove("lstest");
}

void test_read_in_current_dir() {
	unsigned int c;
	read_file("./README.md",&c);
	assert_true(c > 0);
}

void teardown(){
	return;
}