#include "ProjectLib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
/*NOTES!!
YOU HAVE TO RESPECT THE ORDER OF THE ELEMENTS FOR parseElements 
1. KEY-CHILDREN
2. KEY-VAL
3. KEY-ARRAY

The program organizes the stings in alphabetical order.
*/
using namespace test;
std::ofstream out_data("MyOut.json", std::ios::app);
//all the std::cout were replaced with out_data
time_t now = time(0);
char* dt = ctime(&now);

// ========================================================TEST SIZE===========================================================================================================================


TEST_CASE("Read key val primitives, check size", "[size]"){	
	out_data << "\n======================================TEST 1 [size]==========================================\n" << dt << std::endl;
	std::string subject_keyVal("{\"color\": \"yellow\",\"category\" : \"hue\",\"heex\" : [\"55\", \"55\", \"70\", \"17\"],\"type\" : \"primary\",\"hex\" : [\"255\", \"255\", \"0\", \"1\"]}");
	out_data << "The subject:" << subject_keyVal << std::endl;
	JSONNode *testnode = test::parseElements(subject_keyVal);
	CHECK(testnode->keyVal.size() == 5);
	if (testnode->keyVal.size() == 5)
	{ 
		out_data << "\n+TEST PASSED!! 5 KeyVal was found.\n " << testnode->keyVal << std::endl;
	}
	else
	{	
		out_data << "\n-TEST FAILED!! (5 KeyVal should be found).\n " << testnode->keyVal << std::endl;
	}
	//------DONE-------
}

TEST_CASE("Read key with children of primitives, check size", "[size]"){
	out_data << "======================================TEST 2 [size]==========================================\n" << dt << std::endl;
	std::string subject_keyChildren("{\"code\":{\"heex\":[\"565\",\"55\",\"70\",\"517\"]},\"cod\":{\"hex\" : [\"57\", \"5\", \"7\", \"17\"]}}");
	out_data << "The subject:" << subject_keyChildren << std::endl;
	JSONNode *testnode = test::parseElements(subject_keyChildren);
	CHECK(testnode->keyVal.size() == 2);
	CHECK(testnode->child.size() == 2);
	if (testnode->keyVal.size() == 2)
	{
		if (testnode->child.size() == 2)
		{
			out_data << "\n+TEST PASSED!! 2 KeyVal->2 children was found\n" << testnode->keyVal << testnode->child[0]->key << ":\t" << testnode->child <<"\n"<< std::endl;
		}
		else
		{ 
			out_data << "\n-TEST FAILED!! 2 KeyVal was found (2 children should be found).\n " << testnode->keyVal << testnode->child[0]->key << ":\t" << testnode->child <<"\n"<< std::endl;
		}
	}
	else
	{
		out_data << "\n-TEST FAILED!! (2 KeyVal->2 children should be found).\n " << testnode->keyVal << testnode->child[0]->key << ":\t" << testnode->child << "\n" << std::endl;
	}
	//------DONE------- 
}

TEST_CASE("Read mix keyVal and keyChildren, check size", "[size]"){
	out_data << "======================================TEST 3 [size]==========================================\n" << dt << std::endl;
	std::string subject_mix_key_vc("{\"color\": \"yellow\",\"cod\":{\"hex\" : [\"57\", \"5\", \"7\", \"17\"]}}");
	out_data << "The subject:" << subject_mix_key_vc << std::endl;
	JSONNode *testnode = test::parseElements(subject_mix_key_vc);
	CHECK(testnode->keyVal.size() == 2);
	CHECK(testnode->child.size() == 1);
	if (testnode->keyVal.size() == 2)
	{
		if (testnode->child.size() == 1)
		{
			out_data << "\n+TEST PASSED!! 2 KeyVal->1 child was found\n" << testnode->child[0]->key << ":\t" << testnode->child << testnode->keyVal << "\n" << std::endl;
		}
		else
		{
			out_data << "\n-TEST FAILED!! 2 KeyVal was found (1 child should be found).\n " << testnode->child[0]->key << ":\t" << testnode->child << testnode->keyVal << "\n" << std::endl;
		}
	}
	else
	{ 
		out_data << "\n-TEST FAILED!! (2 KeyVal->1 child should be found).\n " << testnode->child[0]->key << ":\t" << testnode->child << testnode->keyVal << "\n" << std::endl;
	}
	//------DONE-------
}

TEST_CASE("Read the primitives from a child, check size","[size]"){
	out_data << "======================================TEST 4 [size]==========================================\n" << dt << std::endl;
	std::string subject_nr_of_primitives("{\"code\":{\"color\": \"yellow\",\"heex\":[\"565\",\"55\",\"70\",\"517\"],\"category\" : \"hue\"}}");
	out_data << "The subject:" << subject_nr_of_primitives << std::endl;
	JSONNode *testnode = test::parseElements(subject_nr_of_primitives);
	CHECK(testnode->child.size() == 1);
	if (testnode->child.size() == 1)
	{
		out_data << "\n+TEST PASSED!! 1 child was found\n" << testnode->child[0]->key << ":\t" << testnode->child << "\n" << std::endl;
	}
	else
	{
		out_data << "\n-TEST FAILED!! (1 child should be found).\n " << testnode->child[0]->key << ":\t" << testnode->child << "\n" << std::endl;
	}
	//-----DONE----------
}


// ========================================================TEST EQUALITY=====================================================================================================================


TEST_CASE("Check equality between two primitives", "[equality]"){
	out_data << "======================================TEST 1 [equality]==========================================\n" << dt << std::endl;
	std::string subject_for_equality_primitives1("{\"color\": \"yellow\",\"category\" : \"hue\"}");
	JSONNode *testnode1 = test::parseElements(subject_for_equality_primitives1);
	std::string subject_for_equality_primitives2("{\"color\": \"yellow\",\"category\" : \"hue\"}");
	JSONNode *testnode2 = test::parseElements(subject_for_equality_primitives2);
	out_data << "Subject 1:" << subject_for_equality_primitives1 << std::endl;
	out_data << "Subject 2:" << subject_for_equality_primitives2 << std::endl;
	CHECK_FALSE(!(testnode1->keyVal == testnode2->keyVal));
	if (!(testnode1->keyVal == testnode2->keyVal))
	{
		out_data << "\n+TEST FAILED!! KeyVal are different\n" << testnode1->keyVal << testnode2->keyVal << std::endl;
	}
	else
	{
		out_data << "\n+TEST PASSED!! KeyVal are the same\n" << testnode1->keyVal << testnode2->keyVal << std::endl;
	}
	//---------DONE------- it also works when you read the primitives from a file
}

TEST_CASE("Check equality between two children of primitives", "[equality]"){
	out_data << "======================================TEST 2 [equality]==========================================\n"<< dt << std::endl;
	std::string subject_keyChildren1("{\"code\":{\"one\":\"#FF0\",\"two\":\"#0F0\"}}");
	std::string subject_keyChildren2("{\"code\":{\"one\":\"#FF0\",\"twdo\":\"#0F0\"}}");
	JSONNode *testnode1 = test::parseElements(subject_keyChildren1);
	JSONNode *testnode2 = test::parseElements(subject_keyChildren2);
	out_data << "Subject 1:" << subject_keyChildren1 << std::endl;
	out_data << "Subject 2:" << subject_keyChildren2 << std::endl;
	CHECK_FALSE(testnode1->keyVal == testnode2->keyVal);
	if (!(testnode1->keyVal == testnode2->keyVal))
	{
		out_data << "\n+TEST FAILED!! KeyVal are different\n" << testnode1->keyVal << testnode2->keyVal << std::endl;
	}
	else
	{
		out_data << "\n+TEST PASSED!! KeyVal are the same\n" << testnode1->keyVal << testnode2->keyVal << std::endl;
	}
	//-------DONE-------- 	
}

TEST_CASE("Check equality between two children of arrays", "[equality]"){
	out_data << "======================================TEST 3 [equality]==========================================\n" << dt << std::endl;
	std::string subject_keyChildren1("{\"code\":{\"heex\":[\"565\",\"55\",\"70\",\"517\"],\"hed\":[\"57\", \"5\", \"4\", \"17\"]}}");
	std::string subject_keyChildren2("{\"code\":{\"heex\":[\"565\",\"55\",\"70\",\"517\"],\"hed\":[\"57\", \"5\", \"4\", \"17\"]}}");
	out_data << "Subject 1:" << subject_keyChildren1 << std::endl;
	out_data << "Subject 2:" << subject_keyChildren2 << std::endl;
	JSONNode *testnode1 = test::parseElements(subject_keyChildren1);
	JSONNode *testnode2 = test::parseElements(subject_keyChildren2);
	CHECK_FALSE(!(testnode1->keyVal == testnode2->keyVal));
	if (!(testnode1->keyVal == testnode2->keyVal))
	{
		out_data << "\n+TEST FAILED!! KeyVal are different\n" << testnode1->keyVal << testnode2->keyVal << std::endl;
	}
	else
	{
		out_data << "\n+TEST PASSED!! KeyVal are the same\n" << testnode1->keyVal << testnode2->keyVal << std::endl;
	}
	//-------DONE-------- 	
}

TEST_CASE("Reading two files and compare the content, KEY CHILD", "[equality]"){
	out_data << "======================================TEST 4 [equality]==========================================\n" << dt << std::endl;
	std::string test1;
	std::string test2;
	test::openFiles(test1, test2);
	JSONNode *testnode1 = test::parseKeyListElements(test1);
	JSONNode *testnode2 = test::parseKeyListElements(test2);
	out_data << "Subject 1:" << test1 << std::endl;
	out_data << "Subject 2:" << test2 << std::endl;
	CHECK_FALSE(!(testnode1->child == testnode2->child));
	if (!(testnode1->child == testnode2->child))
	{
		out_data << "\n+TEST FAILED!! KeyVal are different\n" << testnode1->child[0]->key << ":\t" << testnode1->child << "\n" << testnode2->child[0]->key << ":\t" << testnode2->child << std::endl;
	}
	else
	{
		out_data << "\n+TEST PASSED!! KeyVal are the same\n" << testnode1->child[0]->key << ":\t" << testnode1->child << "\n" << testnode2->child[0]->key << ":\t" << testnode2->child << std::endl;
	}
	//--------DONE------- BUT THE FILES MUST BE THE SAME
}

TEST_CASE("Reading two children and compare the content", "[equality]"){
	out_data << "======================================TEST 5 [equality]==========================================\n" << dt << std::endl;
	std::string subject_keyChildren1("{\"code\":{\"heex\":[\"565\",\"55\",\"70\",\"517\"],\"hed\":[\"57\", \"5\", \"4\", \"17\"]}}");
	std::string subject_keyChildren2("{\"code\":{\"heex\":[\"565\",\"55\",\"70\",\"517\"],\"hed\":[\"57\", \"5\", \"4\", \"17\"]}}");
	out_data << "Subject 1:" << subject_keyChildren1 << std::endl;
	out_data << "Subject 2:" << subject_keyChildren2 << std::endl;
	JSONNode *testnode1 = test::parseElements(subject_keyChildren1);
	JSONNode *testnode2 = test::parseElements(subject_keyChildren2);
	CHECK_FALSE(!(testnode1->child == testnode2->child));
	CHECK_FALSE(!(testnode1->child == testnode2->child));
	if (!(testnode1->child == testnode2->child))
	{
		out_data << "\n+TEST FAILED!! KeyVal are different\n" << testnode1->child[0]->key << ":\t" << testnode1->child << "\n" << testnode2->child[0]->key << ":\t" << testnode2->child << std::endl;
	}
	else
	{
		out_data << "\n+TEST PASSED!! KeyVal are the same\n" << testnode1->child[0]->key << ":\t" << testnode1->child << "\n" << testnode2->child[0]->key << ":\t" << testnode2->child << std::endl;
	}
	//--------DONE--------
}


// ========================================================TEST PARSE========================================================================================================================


TEST_CASE("Read key with children of primitives, check the parse", "[parse_json]"){
	out_data << "======================================TEST 1 [parse_json]==========================================\n" << dt << std::endl;
	std::string subject_keyChildren("{\"code\":{\"heex\":[\"565\",\"55\",\"70\",\"517\"]},{\"hex\":[\"57\", \"5\", \"7\", \"17\"]}}");
	out_data << "The subject is:" << subject_keyChildren << std::endl;
	JSONNode *testnode = test::parseElements(subject_keyChildren);
	out_data << "\n The parse:\n" << testnode->child[0]->key << ":\t" << testnode->child << testnode->keyVal << std::endl;
	//--------DONE-------
}

TEST_CASE("Parsing just one file, taking all elements, including comma", "[parse_json]")
{
	out_data << "======================================TEST 2 [parse_json]==========================================\n" << dt << std::endl;
	std::string test1;
	std::string test2;
	test::openFiles(test1, test2);
	out_data << "The subject is:" << test1 << std::endl;
	JSONNode *testnode1 = test::parseKeyListElements(test1);
	out_data << "\n The parse:\n" << testnode1->child[0]->key << ":\t" << testnode1->child << testnode1->keyVal << std::endl;
	testnode1;
	//--------DONE-------
}

TEST_CASE("Parsing key array[children]", "[parse_json]")
{
	out_data << "======================================TEST 3 [parse_json]==========================================\n" << dt << std::endl;
	std::string subject_keyVal("{\"color\": \"yellow\",\"category\" : \"hue\",\"heex\" : [\"55\", \"55\", \"70\", \"17\"],\"type\" : \"primary\",\"hex\" : [\"255\", \"255\", \"0\", \"1\"], \"code\":{\"heex\":[\"565\",\"55\",\"70\",\"517\"],\"hex\":[\"57\", \"5\", \"7\", \"17\"]}}");
	JSONNode *testnode = test::parseElements(subject_keyVal);
	out_data << "The subject is:" << subject_keyVal << std::endl;
	out_data << "\n The parse for children:\n" << testnode->child[0]->key << ":\t" << testnode->child << "\nThe parse for KeyVal:\n" << testnode->keyVal << std::endl;
	//--------DONE-------
}


// ========================================================TEST MERGE========================================================================================================================


TEST_CASE("Merge keyVal of []", "[merge]")
{
	out_data << "======================================TEST 1 [merge]==========================================\n" << dt << std::endl;
	std::string sub1("{\"one\" : [\"55\", \"55\", \"70\", \"17\"],\"two\" : [\"55\", \"55\", \"70\", \"17\"]}");
	std::string sub2("{\"one\" : [\"55\", \"55\", \"70\", \"17\"],\"two1\" : [\"55\", \"55\", \"70\", \"17\"]}");
	out_data << "Subject 1:" << sub1 << std::endl;
	out_data << "Subject 2:" << sub2 << std::endl;
	JSONNode *testnode = test::parseElements(sub1);
	JSONNode *testnode2 = test::parseElements(sub2);
	mergeInFile(out_data, testnode, testnode2);
	//--------DONE-------
}

TEST_CASE("Merge keyVal of strings", "[merge]")
{
	out_data << "======================================TEST 2 [merge]==========================================\n" << dt << std::endl;
	std::string sub1("{\"color\": \"green\"}");
	std::string sub2("{\"color\": \"yellow\"}");
	out_data << "Subject 1:" << sub1 << std::endl;
	out_data << "Subject 2:" << sub2 << std::endl;
	JSONNode *testnode = test::parseElements(sub1);
	JSONNode *testnode2 = test::parseElements(sub2);
	mergeInFile(out_data, testnode, testnode2);
	//--------DONE-------
}

TEST_CASE("Merge keyChildren", "[merge]")
{
	out_data << "======================================TEST 3 [merge]==========================================\n" << dt << std::endl;
	std::string sub1("{\"code\":{\"hex\":\"#FF0\",\"hex\":\"#0F0\"}}");
	std::string sub2("{\"code\":{\"hex\":\"#FF0\",\"hex\":\"#0F0\"}}");
	out_data << "Subject 1:" << sub1 << std::endl;
	out_data << "Subject 2:" << sub2 << std::endl;
	JSONNode *testnode = test::parseElements(sub1);
	JSONNode *testnode2 = test::parseElements(sub2);
	//mergeInFile(out_data, testnode, testnode2);
	//--------DONE-------
}

TEST_CASE("Merge files", "[merge]")
{
	out_data << "======================================TEST 4 [merge]==========================================\n" << dt << std::endl;
	std::string test1;
	std::string test2;
	test::openFiles(test1, test2);
	out_data << "Subject 1:" << test1 << std::endl;
	out_data << "Subject 2:" << test2 << std::endl;
	JSONNode *testnode1 = test::parseKeyListElements(test1);
	JSONNode *testnode2 = test::parseKeyListElements(test2);
	mergeInFile(out_data, testnode1, testnode2);
	//-------DONE------
}

TEST_CASE("Merge keyVal of strings with option", "[merge]")
{
	out_data << "======================================TEST 5 [merge]==========================================\n" << dt << std::endl;
	std::string sub1("{\"color\": \"green\"}");
	std::string sub2("{\"color\": \"yellow\"}");
	out_data << "Subject 1:" << sub1 << std::endl;
	out_data << "Subject 2:" << sub2 << std::endl;
	JSONNode *testnode = test::parseElements(sub1);
	JSONNode *testnode2 = test::parseElements(sub2);
	//mergeOptionFunction(testnode, testnode2, KEEP_BOTH_ELEMENTS);
	mergeElements(testnode, testnode2);
	//--------DONE-------this test does not print the result in a file!
}


// ========================================================TEST INDEX======================================================================================================


TEST_CASE("Index elements", "[index]")
{
	out_data << "======================================TEST 1 [index]==========================================\n" << dt << std::endl;
	std::string sub("{\"color\": \"green\",\"color\": \"yellow\"}");
	out_data << "Subject 1:" << sub << std::endl;
	JSONNode *testnode = test::parseElements(sub);
	out_data << "\n Indexed elements: \n" << std::endl;
	indexElementsInFile(out_data,testnode);
	//--------DONE-------
}

TEST_CASE("Index and merge elements", "[index]")
{
	out_data << "======================================TEST 2 [index]==========================================\n" << dt << std::endl;
	std::string sub("{\"color\": \"green\",\"color\": \"yellow\"}");
	std::string sub1("{\"color\": \"green\",\"type\": \"primary\",\"color\": \"blue\"}");
	out_data << "Subject 1:" << sub << std::endl;
	out_data << "Subject 2:" << sub1 << std::endl;
	JSONNode *testnode = test::parseElements(sub);
	JSONNode *testnode1 = test::parseElements(sub1);
	JSONNode *testindex= test::indexElements(testnode);
	JSONNode *testindex1 = test::indexElements(testnode1);
	//--------DONE-------
}


// ========================================================TEST ORGANIZE======================================================================================================


TEST_CASE("Structure keyVal", "[structu7re]")
{
	out_data << "======================================TEST 1 [structure]==========================================\n" << dt << std::endl;
	std::string sub("{\"color\": \"green\",\"type\": \"primary\",\"hex\": \"fff\",\"color\": \"yellow\"}");
	out_data << "Subject 1:" << sub << std::endl;
	JSONNode *test = test::structureElementsInFile(out_data,sub);
	//JSONNode *test = test::structureElements(sub);
	//--------DONE-------
}
TEST_CASE("Structure child", "[structure]")
{
	out_data << "======================================TEST 2 [structure]==========================================\n" << dt << std::endl;
	std::string subject_keyChildren("{\"color\":\"yellow\",\"code\":{\"one\":\"#FF0\",\"two\":\"#0F0\"},\"code1\":{\"one1\":\"#FF0\",\"two1\":\"#0F0\"}}");
	out_data << "Subject 1:" << subject_keyChildren << std::endl;
	JSONNode *test = test::structureElementsInFile(out_data, subject_keyChildren);
	//JSONNode *test = test::structureElements(sub);
	//--------DONE-------
}

TEST_CASE("Structure a file after parsing in another file", "[structure1]")
{
	out_data << "======================================TEST 3 [structure]==========================================\n" << dt << std::endl;
	std::string test1;
	std::string test2;
	test::openFiles(test1, test2);
	//the second file "test2" is not useful now, but is needed because of the parametres of the function "openFiles()"
	JSONNode *testnode1 = test::structureFilesinFile(out_data,test1);
	std::string tsaest1;

}
