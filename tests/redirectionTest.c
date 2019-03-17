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

void testSyntaxCheck3() {
	char* test = "  Leo is a nice    >      person  ";
	assert_eq_int(1,redirection_syntax_check(test));
}

void testSyntaxCheck4() {
	char* test2 = "Leo < > poffpok";
	assert_eq_int(0,redirection_syntax_check(test2));
}

void testSyntaxCheck5() {
	char* test2 = "Leo > poffpok";
	assert_eq_int(1,redirection_syntax_check(test2));
}

void testSyntaxCheck6() {
	char* test2 = "Leo >> poffpok";
	assert_eq_int(1,redirection_syntax_check(test2));
}

void testSyntaxCheck7() {
	char* test2 = "Leo > > poffpok";
	assert_eq_int(0,redirection_syntax_check(test2));
}

void testSyntaxCheck8() {
	char* test = "Leo | poffpok";
	assert_eq_int(1, redirection_syntax_check(test));
}

void testSyntaxCheck9() {
	char* test = "Leo >| poffpok";
	assert_eq_int(0, redirection_syntax_check(test));
}

void testSyntaxCheck10() {
	char* test = "| poffpok";
	assert_eq_int(0, redirection_syntax_check(test));
}

void testSyntaxCheck11() {
	char* test = "poffpok |";
	assert_eq_int(0, redirection_syntax_check(test));
}

void testSyntaxCheck12() {
	char* test = "poffpok |> wepfok";
	assert_eq_int(0, redirection_syntax_check(test));
}

void testSyntaxCheck13() {
	char* test = "poffpok || wepfoj";
	assert_eq_int(0, redirection_syntax_check(test));
}

void testSyntaxCheck14() {
	char* test = "poffpok |  | wepfoj";
	assert_eq_int(0, redirection_syntax_check(test));
}

void testSyntaxCheck15() {
	char* test = "poffpok < beebo | bobo";
	assert_eq_int(1, redirection_syntax_check(test));
}
