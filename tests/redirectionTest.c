#include "../src/redirection.h"
#include "redirectionTest.h"
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
	assert_eq_int(1,rd_syntax_check(test));
}

void testSyntaxCheck4() {
	char* test2 = "Leo < > poffpok";
	assert_eq_int(0,rd_syntax_check(test2));
}

void testSyntaxCheck5() {
	char* test2 = "Leo > poffpok";
	assert_eq_int(1,rd_syntax_check(test2));
}

void testSyntaxCheck6() {
	char* test2 = "Leo >> poffpok";
	assert_eq_int(1,rd_syntax_check(test2));
}

void testSyntaxCheck7() {
	char* test2 = "Leo > > poffpok";
	assert_eq_int(0,rd_syntax_check(test2));
}

void testSyntaxCheck8() {
	char* test = "Leo | poffpok";
	assert_eq_int(1, rd_syntax_check(test));
}

void testSyntaxCheck9() {
	char* test = "Leo >| poffpok";
	assert_eq_int(0, rd_syntax_check(test));
}

void testSyntaxCheck10() {
	char* test = "| poffpok";
	assert_eq_int(0, rd_syntax_check(test));
}

void testSyntaxCheck11() {
	char* test = "poffpok |";
	assert_eq_int(0, rd_syntax_check(test));
}

void testSyntaxCheck12() {
	char* test = "poffpok |> wepfok";
	assert_eq_int(0, rd_syntax_check(test));
}

void testSyntaxCheck13() {
	char* test = "poffpok || wepfoj";
	assert_eq_int(0, rd_syntax_check(test));
}

void testSyntaxCheck14() {
	char* test = "poffpok |  | wepfoj";
	assert_eq_int(0, rd_syntax_check(test));
}

void testSyntaxCheck15() {
	char* test = "poffpok < beebo | bobo";
	assert_eq_int(1, rd_syntax_check(test));
}

void testGetInFilename() {
	char* test = "somecoolcommand    <       filerino      ";
	assert_eq_str("filerino", rd_test_get_in_filename(test));
}

void testGetOutFilename() {
	char* test = "somecoolcommand    >  filerino  ";
	OutMode omode;
	assert_eq_str("filerino", rd_test_get_out_filename(test,&omode));
	assert_eq_int(OUT_MODE_OW, omode);
}

void testGetOutFilename2() {
	char* test = "somecoolcommand    >>  filerino  ";
	OutMode omode;
	assert_eq_str("filerino", rd_test_get_out_filename(test,&omode));
	assert_eq_int(OUT_MODE_AP, omode);
}

void testGetCommand() {
	char* test = "               commando >> filerino";
	assert_eq_str("commando", rd_test_get_command(test));
}

void testCreatePipeline() {
	char* test = "ls | head -3 > ls.txt";
	PipelineNode* head = rd_test_create_pipeline(test);
	assert_eq_str("ls", head->cmd);
	assert_not_null(head);
	assert_null(head->in);
	assert_null(head->out);
	assert_not_null(head->next);
	PipelineNode* next = head->next;
	assert_eq_str("head", next->cmd);
	assert_null(next->in);
	assert_eq_str("ls.txt", next->out);
	assert_eq_int(OUT_MODE_OW, next->omode);
	assert_null(next->next);
}

void testCreatePipelines() {
	char* test = "command1 >> lolz.txt | echo lolz.txt | cat";
	int nodes = 0;
	PipelineNode** pipelines = rd_parse_command(&nodes, test);
	assert_not_null(pipelines);
	
	PipelineNode* head = pipelines[0];
	assert_not_null(head);
	assert_eq_str("command1", head->cmd);
	assert_null(head->in);
	assert_eq_str("lolz.txt",head->out);
	assert_eq_int(OUT_MODE_AP, head->omode);
	assert_not_null(head->next);
	PipelineNode* next = head->next;
	assert_eq_str("echo", next->cmd);
	assert_null(next->in);
	assert_null(next->out);
	assert_not_null(next->next);
	next = next->next;
	assert_eq_str("cat", next->cmd);
	assert_null(next->in);
	assert_null(next->out);
	assert_null(next->next);

}
