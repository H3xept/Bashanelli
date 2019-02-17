#include "macros.h"
#include "scriptTest.h"
#include "../src/script_handling.h"

void setup(){
	return;
}


void test_read_file(){
	unsigned int count;
	read_file("nudes",&count);
	assert_eq_int(count,0);
}




void teardown(){
	return;
}