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

void test_replace_alias() {
	add_alias("aaaa","beee");
	add_alias("aaaa","seamonster");
	add_alias("aaaa","deamonster");
	add_alias("aaaa","eamonster");
	add_alias("aaaa","feamonster");
	assert_eq_int(1,get_aliaslist_length());
}

void teardown(){
	return;
}