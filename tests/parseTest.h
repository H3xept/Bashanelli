#ifndef __CParse_Test__
#define __CParse_Test__

void setup();
void test_count_occ();
void test_generate_argv();
void test_generate_argv_with_quotes();
void test_generate_argv_with_quotes2();
void test_generate_argv_with_comment();
void test_generate_argv_with_non_comment();
void test_generate_argv_one_quoted_arg();
void test_generate_argv_backslash_spaces();
void test_generate_argv_another_backslash_escape();
void test_generate_argv_multiple_escaped_spaces();
void test_file_exists();
void test_trim_outside_whitespace();
void test_trim_trailing_whitespace();
void test_trim_leading_whitespace();
void test_trim_only_whitespace();
void test_parse_nulls1();
void test_parse_nulls2();
void test_parse_nulls3();
void test_generate_argv_null();
void teardown();

#endif
