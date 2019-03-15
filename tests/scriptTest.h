#ifndef __SCRIPT_TEST__
#define __SCRIPT_TEST__

void setup();
void test_read_empty_file();
void test_read_five_lines();
void test_read_non_existing_file();
void test_run_garbage_script();
void test_read_file_in_path();
void test_run_script_ls();
void test_run_script_ls_with_comments();
void test_read_in_current_dir();
void teardown();

#endif
