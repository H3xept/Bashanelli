#include "redirectionTest.h"
#include "../src/redirection.h"
#include "macros.h"

void setup(){
	return;
}

void teardown(){
	return;
}

void testFirstPass() {
	char* test = "echo \"Hello world\" >> file.txt | cat << file.txt ; echo \"Done\"";

}

void testSyntaxCheck1() {
	// <> is not a valid redirection symbol
	char* test = "pofwefoewpofk <> wpeifpweif";
	// assert_null(redirection_parse_command())
}

void testSyntaxCheck2() {
	// << is not a valid redirection symbol
	char* test = "pofwefoewpofk << wpeifpweif";
}
