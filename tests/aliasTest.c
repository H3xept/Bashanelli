#include "macros.h"
#include "parseTest.h"
#include "../src/parse_commands.h"
#include "../src/execute_command.h"
#include "../src/aliasing.h"

void setup(){
	init_aliases();
	return;
}

void test_add_aliases() {
	add_alias("aaaa","beee");
	add_alias("cea","diii");
	assert_eq_int(2,get_aliaslist_length());
}

void test_remove_first_alias() {
	add_alias("aaaa","beee");
	add_alias("cea","diii");
	remove_alias("aaaa");
	assert_eq_int(1,get_aliaslist_length());
}

void test_remove_alias() {
	add_alias("aaaa","beee");
	add_alias("cea","diii");
	remove_alias("cea");
	assert_eq_int(1,get_aliaslist_length());
}

void test_add_remove_alias() {
	remove_alias("cea");
	add_alias("aaaa","beee");
	add_alias("cea","diii");
	add_alias("dea","xyz");
	add_alias("eea","diii");
	remove_alias("cea");
	add_alias("fea","diii");
	add_alias("sea","diii");
	remove_alias("cea");
	assert_eq_int(5,get_aliaslist_length());
}


void test_remove_non_existing() {
	add_alias("aaaa","beee");
	add_alias("cea","diii");
	assert_eq_int(remove_alias("xyz"),1);
	assert_eq_int(2,get_aliaslist_length());
}

void test_replace_alias() {
	add_alias("aaaa","beee");
	add_alias("aaaa","seamonster");
	add_alias("aaaa","deamonster");
	add_alias("aaaa","eamonster");
	add_alias("aaaa","feamonster");
	assert_eq_int(1,get_aliaslist_length());
}

void test_teardown_aliases() {
	remove_alias("cea");
	add_alias("aaaa","beee");
	add_alias("cea","diii");
	add_alias("dea","xyz");
	add_alias("eea","diii");
	remove_alias("cea");
	add_alias("fea","diii");
	add_alias("sea","diii");
	remove_alias("cea");
	teardown_aliases();
	assert_eq_int(0,get_aliaslist_length());
}

void test_expanding_aliases(){
	add_alias("aa","bee");
	add_alias("cea","diii");
	char *a1 = expand_alias("aa");
	char *a2 = expand_alias("aa");
}

void teardown(){
	return;
}