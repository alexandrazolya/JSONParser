#include "ProjectLib.h"
#include <string>
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
/*NOTES!!
YOU HAVE TO RESPECT THE ORDER OF THE ELEMENTS FOR parseElements 
1. KEY-CHILDREN
2. KEY-VAL
3. KEY-ARRAY
*/
using namespace test;
// ========================================================TEST SIZE===========================================================================================================================
TEST_CASE("Read key val primitives, check size", "[size]"){
	std::string subject_keyVal("{\"color\": \"yellow\",\"category\" : \"hue\",\"heex\" : [\"55\", \"55\", \"70\", \"17\"],\"type\" : \"primary\",\"hex\" : [\"255\", \"255\", \"0\", \"1\"]}");
	JSONNode *testnode = test::parseElements(subject_keyVal);
	CHECK(testnode->keyVal.size() == 5);

	//------DONE-------
}

TEST_CASE("Read key with children of primitives, check size", "[size]"){
	std::string subject_keyChildren("{\"code\":{\"heex\":[\"565\",\"55\",\"70\",\"517\"]},\"cod\":{\"hex\" : [\"57\", \"5\", \"7\", \"17\"]}}");
	JSONNode *testnode = test::parseElements(subject_keyChildren);
	CHECK(testnode->keyVal.size() == 2);
	//------DONE------- there are 2 structures with children but it doesn't matter how many children there are
}

TEST_CASE("Read mix keyVal and keyChildren, check size", "[size]"){
	std::string subject_mix_key_vc("{\"color\": \"yellow\",\"cod\":{\"hex\" : [\"57\", \"5\", \"7\", \"17\"]}}");
	JSONNode *testnode = test::parseElements(subject_mix_key_vc);
	CHECK(testnode->keyVal.size() == 2);
	//------DONE-------
}

TEST_CASE("Read the primitives from a child, check size","[size]"){
	std::string subject_nr_of_primitives("{\"code\":{\"color\": \"yellow\",\"heex\":[\"565\",\"55\",\"70\",\"517\"],\"category\" : \"hue\"}}");
	JSONNode *testnode = test::parseElements(subject_nr_of_primitives);
	CHECK(testnode->child.size() == 1);

	//-----DONE BUT IT SHOWS JUST THE KEY AND THE REST OF STRING IS THE VALUE, IT DOESN'T GET INTO THE STRING TO COUNT THE PRIMITIVES----------
}

// ========================================================TEST EQUALITY=====================================================================================================================

TEST_CASE("Check equality between two primitives", "[equality]"){
	std::string subject_for_equality_primitives1("{\"color\": \"yellow\",\"category\" : \"hue\"}");
	JSONNode *testnode1 = test::parseKeyListElements(subject_for_equality_primitives1);
	std::string subject_for_equality_primitives2("{\"color\": \"yellow\",\"category\" : \"hue\"}");
	JSONNode *testnode2 = test::parseKeyListElements(subject_for_equality_primitives2);
	CHECK_FALSE(!(testnode1->keyVal == testnode2->keyVal));
	//---------DONE------- it also works when you read the primitives from a file
}

TEST_CASE("Check equality between two children of primitives", "[equality]"){
	std::string subject_keyChildren1("{\"code\":{\"hex\":\"#FF0\",\"hex\":\"#0F0\"}}");
	std::string subject_keyChildren2("{\"code\":{\"hex\":\"#FF0\",\"hex\":\"#0F0\"}}");
	JSONNode *testnode1 = test::parseElements(subject_keyChildren1);
	JSONNode *testnode2 = test::parseElements(subject_keyChildren2);
	CHECK_FALSE(!(testnode1->keyVal == testnode2->keyVal));
	//-------DONE-------- 	
}

TEST_CASE("Check equality between two children of arrays", "[equality]"){
	std::string subject_keyChildren1("{\"code\":{\"heex\":[\"565\",\"55\",\"70\",\"517\"],\"hed\":[\"57\", \"5\", \"4\", \"17\"]}}");
	std::string subject_keyChildren2("{\"code\":{\"heex\":[\"565\",\"55\",\"70\",\"517\"],\"hed\":[\"57\", \"5\", \"4\", \"17\"]}}");
	JSONNode *testnode1 = test::parseElements(subject_keyChildren1);
	JSONNode *testnode2 = test::parseElements(subject_keyChildren2);
	CHECK_FALSE(!(testnode1->keyVal == testnode2->keyVal));
	//-------DONE-------- 	
}

TEST_CASE("Reading two files and compare the content", "[equality]"){
	std::string test1;
	std::string test2;
	test::openFiles(test1, test2);
	JSONNode *testnode1 = test::parseKeyListElements(test1);
	JSONNode *testnode2 = test::parseKeyListElements(test2);
	CHECK_FALSE(!(testnode1->keyVal == testnode2->keyVal));
	//--------DONE------- BUT THE FILES MUST BE THE SAME
}

// ========================================================TEST PARSE========================================================================================================================

TEST_CASE("Read key with children of primitives, check the parse", "[parse_json]"){
	std::string subject_keyChildren("{\"code\":{\"heex\":[\"565\",\"55\",\"70\",\"517\"]},{\"hex\":[\"57\", \"5\", \"7\", \"17\"]}}");
	JSONNode *testnode = test::parseElements(subject_keyChildren);
	//--------DONE-------
}

TEST_CASE("Parsing just one file, taking all elements, including comma", "[parse_json]")
{
	std::string test1;
	std::string test2;
	test::openFiles(test1, test2);
	JSONNode *testnode1 = test::parseKeyListElements(test1);
	testnode1;
	//--------DONE-------
}

TEST_CASE("Parsing key array[children]", "[parse_json]")
{
	std::string subject_keyVal("{\"color\": \"yellow\",\"category\" : \"hue\",\"heex\" : [\"55\", \"55\", \"70\", \"17\"],\"type\" : \"primary\",\"hex\" : [\"255\", \"255\", \"0\", \"1\"], \"code\":{\"heex\":[\"565\",\"55\",\"70\",\"517\"],\"hex\":[\"57\", \"5\", \"7\", \"17\"]}}");
	JSONNode *testnode = test::parseElements(subject_keyVal);
	//--------DONE-------
}

// ========================================================TEST MERGE========================================================================================================================

TEST_CASE("Merge keyVal of []", "[merge]")
{
	std::string sub1("{\"one1\" : [\"55\", \"55\", \"70\", \"17\"],\"two\" : [\"55\", \"55\", \"70\", \"17\"]}");
	std::string sub2("{\"one\" : [\"55\", \"55\", \"70\", \"17\"],\"two1\" : [\"55\", \"55\", \"70\", \"17\"]}");
	JSONNode *testnode = test::parseElements(sub1);
	JSONNode *testnode2 = test::parseElements(sub2);
	mergeFiles(testnode, testnode2);
	//--------DONE-------
}

TEST_CASE("Merge keyVal of strings", "[merge]")
{
	std::string sub1("{\"color\": \"green\"}");
	std::string sub2("{\"color\": \"yellow\"}");
	JSONNode *testnode = test::parseElements(sub1);
	JSONNode *testnode2 = test::parseElements(sub2);
	mergeFiles(testnode, testnode2);
	//--------DONE-------
}

TEST_CASE("Merge keyChildren", "[merge]")
{
	std::string sub1("{\"code\":{\"hdex\":\"#FF0\",\"hex\":\"#0F0\"}}");
	std::string sub2("{\"code\":{\"hex\":\"#FF0\",\"hex\":\"#0F0\"}}");
	JSONNode *testnode = test::parseElements(sub1);
	JSONNode *testnode2 = test::parseElements(sub2);
	mergeFiles(testnode, testnode2);
	//--------DONE-------
}

TEST_CASE("Merge files", "[merge1]")
{
	std::string test1;
	std::string test2;
	test::openFiles(test1, test2);
	JSONNode *testnode1 = test::parseKeyListElements(test1);
	JSONNode *testnode2 = test::parseKeyListElements(test2);
	mergeFiles(testnode1, testnode2);
	//-------LOADING------
}