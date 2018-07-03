#include <fstream>
#include <iostream>
#include <string>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <windows.h>
#include <sstream>      // std::istringstream
#include "ProjectLib.h"

using namespace test;
//=======================================================================FUNCTIONS============================================================================
JSONNode* test::parseElements(const std::string & elements, std::string printOffset)
{
	printOffset += "\t";
	std::cout << "value:\t\t" << elements << std::endl;
	std::string subjElem = elements;
	try {
		JSONNode *node = new JSONNode;
		std::sregex_iterator end;
		std::string restOfString;
		std::regex whole_file("\\s*\\{([\\{A-Za-z0-9\":\\#\\n\\t\\s,\\{\\}\\[\\]]+)\\}\\s*");
		std::sregex_iterator whole_file_it(subjElem.begin(), subjElem.end(), whole_file);
		std::regex children("([a-z0-9\"]+)\\s*:\\s*\\s*(\\{[\\#A-Za-z0-9\"\\s*\n*\t*:\\[\\],]+?\\})\\s*,{0,1}");
		std::sregex_iterator next_child(subjElem.begin(), subjElem.end(), children);
		while (next_child != end)
		{
			auto match = *next_child;
			std::cout << "\n" + printOffset << match[1].str() << std::endl;
			JSONNode* child = parseElements(match[2].str(), printOffset);
			// see if child is empty
			if (!(*child == *(new JSONNode)))
			{
				child->key = match[1].str();
				node->child.push_back(child);
			}
			node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
			next_child++;
			restOfString = match.prefix().str() + match.suffix().str();
			std::sregex_iterator tmp = next_child;
			if (tmp == end)
			{
				subjElem = restOfString;
				next_child = std::sregex_iterator(subjElem.begin(), subjElem.end(), children);
			}
		}
		std::regex primitives("\\s*([a-zA-Z0-9\"]+)\\s*:\\s*([a-zA-Z0-9#\"]+),{0,1}\\s*");
		std::sregex_iterator next_primitive(subjElem.begin(), subjElem.end(), primitives);
		while (next_primitive != end) {
			auto match = *next_primitive;
			std::cout << "\n" + printOffset << match[1].str() << std::endl;
			JSONNode* child = parseElements(match[2].str(), printOffset);
			// todo: see if child is empty - done
			if (!(*child == *(new JSONNode)))
			{
				child->key = match[1].str();
				node->child.push_back(child);
			}
			node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
			restOfString = match.prefix().str() + match.suffix().str();
			next_primitive++;
			std::sregex_iterator tmp3 = next_primitive;
			if (tmp3 == end)
			{
				subjElem = restOfString;
				next_primitive = std::sregex_iterator(subjElem.begin(), subjElem.end(), primitives);
			}
		}
		std::regex array("([a-zA-Z0-9\"]+)\\s*:\\s*(\\[[a-zA-Z0-9\",\\s*]+\\]),{0,1}");
		std::sregex_iterator next_array(subjElem.begin(), subjElem.end(), array);
		while (next_array != end) {
			auto match = *next_array;
			std::cout << "\n" + printOffset << match[1].str() << std::endl;
			JSONNode* child = parseElements(match[2].str(), printOffset);
			// see if child is empty
			if (!(*child == *(new JSONNode)))
			{
				child->key = match[1].str();
				node->child.push_back(child);
			}
			node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
			restOfString = match.prefix().str() + match.suffix().str();
			next_array++;
			std::sregex_iterator tmp2 = next_array;
			if (tmp2 == end)
			{
				subjElem = restOfString;
				next_array = std::sregex_iterator(subjElem.begin(), subjElem.end(), array);
			}
			
		}
		if (restOfString != "")
		{
			subjElem = restOfString;
			restOfString = "";
		}
		return node;
	}
	catch (std::regex_error& e) {
		std::cout << e.what();
	}
	std::cout << "\n\nEND\n\n" << std::endl;
}

JSONNode* test::parseKeyListElements(const std::string & s, std::string printOffset)
{
	printOffset += "\t";
	std::cout << "value:\t\t" << s << std::endl;
	std::string subject = s;
	try
	{
		JSONNode *node = new JSONNode;
		JSONNode* parentNode = new JSONNode;
		std::sregex_iterator end;
		std::string restOfString;
		std::regex whole_file("\\s*\\{([\\{A-Za-z0-9\":\\#\\n\\t\\s,\\{\\}\\[\\]]+)\\}\\s*");
		/*	the whole file between the structure { }*/
		std::sregex_iterator whole_file_it(subject.begin(), subject.end(), whole_file);
		std::regex child_array("\\{\\s*([a-zA-Z0-9\"]+)\\s*:\\s*\\n*\\t*\\[\\s*([a-zA-Z\"\\[\\]#0-9\\s*:,\\{\\}]+)\\s*\\]\\s*\\}");
		/*delimits the key from an array with children between the []
		the content between [] is parsed then with the regex called subgroup (the next regex)*/
		std::sregex_iterator next_child_array(subject.begin(), subject.end(), child_array);
		while (next_child_array != end)
		{
			auto match = *next_child_array;
			std::cout << "\n" + printOffset << match[1].str() << std::endl;
			//JSONNode* parentNode = parseKeyListElements(match[2].str(), printOffset);
			//if (!(*parentNode == *(new JSONNode)))
			//{
			//	parentNode->key = match[1].str();
			//	node->parentNode.insert(std::make_pair(match[1].str(), match[2].str()));
			//}
			std::string groupValue = match[2].str();
			std::regex subgrup_child_array("\\{\\s*([\\{A-Za-z0-9\":\\#\\n\\t\\s,\\[\\]\\{\\}]+?\\})\\s*\\}");
			/* key : [{subgroup from array}] checked*/
			std::sregex_iterator next_subgrup_child_array(groupValue.begin(), groupValue.end(), subgrup_child_array);
			for (; next_subgrup_child_array != end; ++next_subgrup_child_array)
			{				
				//JSONNode *node = new JSONNode;
				auto groupMatch = *next_subgrup_child_array;
				std::cout << "\n" + printOffset << groupMatch[1].str() << std::endl;
				std::string subgrMatch = groupMatch[1].str();
				/////////////////
				std::regex children("([a-z0-9\"]+)\\s*:\\s*\\{\\s*([\\#A-Za-z0-9\"\\s*\n*\t*:\\[\\],]+?)\\s*\\},{0,1}");
				std::sregex_iterator next_child(subgrMatch.begin(), subgrMatch.end(), children);
				while (next_child != end)
				{
					auto match = *next_child;
					std::cout << "\n" + printOffset << match[1].str() << std::endl;
					JSONNode* child = parseKeyListElements(match[2].str(), printOffset);
					// see if child is empty
					if (!(*child == *(new JSONNode)))
					{
						child->key = match[1].str();
						node->child.push_back(child);
					}

					node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
					next_child++;
					restOfString = match.prefix().str() + match.suffix().str();
					std::sregex_iterator tmp = next_child;
					if (tmp == end)
					{
						subject = restOfString;
						next_child = std::sregex_iterator(subject.begin(), subject.end(), children);
					}
				}
				///////////////////////
				std::regex primitives("\\s*([a-zA-Z0-9\"]+)\\s*:\\s*([a-zA-Z0-9#\"]+),{0,1}\\s*");
				/* checked with or without ,
				"color": "yellow" */
				std::sregex_iterator next_primitive(subgrMatch.begin(), subgrMatch.end(), primitives);
				while (next_primitive != end)
				{
					auto match = *next_primitive;
					std::cout << "\n" + printOffset << match[1].str() << std::endl;
					JSONNode* child = parseKeyListElements(match[2].str(), printOffset);
					// todo: see if child is empty - done
					if (!(*child == *(new JSONNode)))
					{
						child->key = match[1].str();
						node->child.push_back(child);
					}
					node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
					restOfString = match.prefix().str() + match.suffix().str();
					next_primitive++;
					std::sregex_iterator tmp3 = next_primitive;
					if (tmp3 == end)
					{
						subject = restOfString;
						next_primitive = std::sregex_iterator(subject.begin(), subject.end(), primitives);
					}

				}
				///////////////////////
				std::regex array("([a-zA-Z0-9\"]+)\\s*:\\s*(\\[[a-zA-Z0-9\",\\s*]+\\]),{0,1}");
				/* key : array -checked with or without ,
				"hex" : ["5", "5", "7", "1"] */
				std::sregex_iterator next_array(subgrMatch.begin(), subgrMatch.end(), array);
				while (next_array != end)
				{
					auto match = *next_array;
					std::cout << "\n" + printOffset << match[1].str() << std::endl;
					JSONNode* child = parseKeyListElements(match[2].str(), printOffset);
					// see if child is empty
					if (!(*child == *(new JSONNode)))
					{
						child->key = match[1].str();
						node->child.push_back(child);
					}
					node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
					restOfString = match.prefix().str() + match.suffix().str();
					next_array++;
					std::sregex_iterator tmp2 = next_array;
					if (tmp2 == end)
					{
						subject = restOfString;
						next_array = std::sregex_iterator(subject.begin(), subject.end(), array);
					}
				}
				if (!(*node == *(new JSONNode)))
				{
					parentNode->child.push_back(node);
				}
				else
				{
					delete node;
				}
			}
			next_child_array++;
			restOfString = match.prefix().str() + match.suffix().str();
			std::sregex_iterator tmp1 = next_child_array;
			if (tmp1 == end)
			{
				subject = restOfString;
				next_child_array = std::sregex_iterator(subject.begin(), subject.end(), child_array);
			}
		}

		if (restOfString != "")
		{
			subject = restOfString;
			restOfString = "";
		}

		return parentNode;
	}
	catch (std::regex_error& e) {
		std::cout << e.what();
	}
	std::cout << "\n\nEND\n\n" << std::endl;
	return nullptr;
}

void test::deleteFunction(JSONNode *node)
{
	// Case of NULL list
	if (node == NULL) {
		return;
	}
	for (int i = 0; i < node->child.size(); i++)
	{
		deleteFunction(node->child[i]);
		delete node->child[i];
	}
	auto begin = node->child.begin();
	auto end = node->child.end();
	node->child.erase(begin, end);
}

void test::openFiles(std::string& test1, std::string& test2)
{   /* !!!!!NOT WORKING IN THIS FILE, SO USE THE PREVIOUS FILE DIRECTORY AND COPY THE .JSON FILES THERE!!!!!
	std::ifstream ifs1("C:/Users/maria.zolya/Documents/Visual Studio 2013/Projects/ProjectLib/JSONParserTest/resources/colors.json");
	std::ifstream ifs2("C:/Users/maria.zolya/Documents/Visual Studio 2013/Projects/ProjectLib/JSONParserTest/resources/colors2.json");
	*/
	std::ifstream ifs1("C:/Users/maria.zolya/Documents/Visual Studio 2013/Projects/ProjectLib/colors.json");
	std::ifstream ifs2("C:/Users/maria.zolya/Documents/Visual Studio 2013/Projects/ProjectLib/colors2.json");
	std::stringstream ss1;
	std::stringstream ss2;
	if (ifs1.is_open() || ifs2.is_open())
	{
		ss1 << ifs1.rdbuf();
		ss2 << ifs2.rdbuf();
		test1 = ss1.str();
		test2 = ss2.str();
	}
}

void test::removeKeyValDuplicates(std::map<std::string, std::string> &kV1, std::map<std::string, std::string> &kV2)
{
	if (kV1.size() == kV2.size())
	{
		if (kV1.empty())
		{
			std::cout << "Empty keyVal" << std::endl;
			return;
		}
		for (auto& pair : kV1)
		{
			auto found = kV2.find(pair.first);
			if (found != kV2.end()) //if the key of kv1 and kv2 is equal
			{
				if (kV1 == kV2)//if kv1 and kv2 is equal
				{
					kV2.erase(found);//erase kv2
				}
				if (pair.second == found->second)//if the value of kv1 is equal to kv2
				{
					kV2.erase(found);//erase kv2
				}
			}
		}
	}
	else { std::cout << "Different sizes of keyVal" << std::endl; }
}

void test::removeChildDuplicates(std::vector<JSONNode*> &C1, std::vector<JSONNode*> &C2)
{ 
	if (C1.size() == C2.size())
	{
		if (C1.empty())
		{
			std::cout << "Empty child" << std::endl;
			return;
		}
		for (int i = 0; i < C1.size(); i++)
		{
			for (int j = 0; j < C2.size(); j++)
			{
				if (C1[i] == C2[j])
				{
					delete C2[j];
				}
			}
		}
	}
	else { std::cout << "Different sizes of children" << std::endl; }
}

void test::mergeFiles(JSONNode *node1, JSONNode *node2)
{
	if (node1->child.size() == node2->child.size())
	{
		if (!(node1->child.empty()))
		{
			if (node1->child == node2->child)
			{
				std::cout << node1->child << std::endl;
				std::cout << "\nChildren are the same\n";

			}
			if (node1->keyVal.size() == node2->keyVal.size())
			{
				if (!(node1->keyVal.empty()))
				{
					if (node1->keyVal == node2->keyVal)
					{
						std::cout << node1->keyVal << std::endl;
						std::cout << "\nKeyVal are the same\n";
					}
					else
					{
						removeKeyValDuplicates(node1->keyVal, node2->keyVal);
						std::cout << node1->keyVal << node2->keyVal << std::endl;
						std::cout << "\nKeyVal has duplicates or different values\n";
					}
				}
				else
				{
					std::cout << "\nEMPTY keyVal\n";
				}
			}
			else
			{
				removeChildDuplicates(node1->child, node2->child);
				std::cout << node1->child << node2->child << std::endl;
				std::cout << "\nChildren has duplicates or different values\n";
			}
		}
		else
		{
			std::cout << "\nEMPTY keyVal\n";
		}
	}
	else
	{
		std::cout << "\nDiferent size of children\n";
	}
	////////////////
	if (node1->keyVal.size() == node2->keyVal.size())
	{
		if (!(node1->keyVal.empty()))
		{
			if (node1->keyVal == node2->keyVal)
			{
				std::cout << node1->keyVal << std::endl;
				std::cout << "\nKeyVal are the same\n";
			}
			else
			{
				removeKeyValDuplicates(node1->keyVal, node2->keyVal);
				std::cout << node1->keyVal << node2->keyVal << std::endl;
				std::cout << "\nKeyVal has duplicates or different values\n";
			}
		}
		else
		{
			std::cout << "\nEMPTY keyVal\n";
		}
	}
	else
	{
		std::cout << "\nDifferent sizes of keyVal\n";
	}
}

//===============================================================FUNCTIONS THAT PRINT IN A FILE====================================================================

//JSONNode* parseElementsInFile(std::ofstream &out_data, const std::string & elements, std::string printOffset)
//{
//	printOffset += "\t";
//	out_data << "value:\t\t" << elements << std::endl;
//	std::cout << "value:\t\t" << elements << std::endl;
//	std::string subjElem = elements;
//	try {
//		JSONNode *node = new JSONNode;
//		std::sregex_iterator end;
//		std::string restOfString;
//		std::regex whole_file("\\s*\\{([\\{A-Za-z0-9\":\\#\\n\\t\\s,\\{\\}\\[\\]]+)\\}\\s*");
//		std::sregex_iterator whole_file_it(subjElem.begin(), subjElem.end(), whole_file);
//		std::regex children("([a-z0-9\"]+)\\s*:\\s*\\s*(\\{[\\#A-Za-z0-9\"\\s*\n*\t*:\\[\\],]+?\\})\\s*,{0,1}");
//		std::sregex_iterator next_child(subjElem.begin(), subjElem.end(), children);
//		while (next_child != end)
//		{
//			auto match = *next_child;
//			out_data << "\n" + printOffset << match[1].str() << std::endl;
//			std::cout << "\n" + printOffset << match[1].str() << std::endl;
//			JSONNode* child = parseElements(match[2].str(), printOffset);
//			// see if child is empty
//			if (!(*child == *(new JSONNode)))
//			{
//				child->key = match[1].str();
//				node->child.push_back(child);
//			}
//			node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
//			next_child++;
//			restOfString = match.prefix().str() + match.suffix().str();
//			std::sregex_iterator tmp = next_child;
//			if (tmp == end)
//			{
//				subjElem = restOfString;
//				next_child = std::sregex_iterator(subjElem.begin(), subjElem.end(), children);
//			}
//		}
//		std::regex primitives("\\s*([a-zA-Z0-9\"]+)\\s*:\\s*([a-zA-Z0-9#\"]+),{0,1}\\s*");
//		std::sregex_iterator next_primitive(subjElem.begin(), subjElem.end(), primitives);
//		while (next_primitive != end) {
//			auto match = *next_primitive;
//			out_data << "\n" + printOffset << match[1].str() << std::endl;
//			std::cout << "\n" + printOffset << match[1].str() << std::endl;
//			JSONNode* child = parseElements(match[2].str(), printOffset);
//			// todo: see if child is empty - done
//			if (!(*child == *(new JSONNode)))
//			{
//				child->key = match[1].str();
//				node->child.push_back(child);
//			}
//			node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
//			restOfString = match.prefix().str() + match.suffix().str();
//			next_primitive++;
//			std::sregex_iterator tmp3 = next_primitive;
//			if (tmp3 == end)
//			{
//				subjElem = restOfString;
//				next_primitive = std::sregex_iterator(subjElem.begin(), subjElem.end(), primitives);
//			}
//		}
//		std::regex array("([a-zA-Z0-9\"]+)\\s*:\\s*(\\[[a-zA-Z0-9\",\\s*]+\\]),{0,1}");
//		std::sregex_iterator next_array(subjElem.begin(), subjElem.end(), array);
//		while (next_array != end) {
//			auto match = *next_array;
//			out_data << "\n" + printOffset << match[1].str() << std::endl;
//			std::cout << "\n" + printOffset << match[1].str() << std::endl;
//			JSONNode* child = parseElements(match[2].str(), printOffset);
//			// see if child is empty
//			if (!(*child == *(new JSONNode)))
//			{
//				child->key = match[1].str();
//				node->child.push_back(child);
//			}
//			node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
//			restOfString = match.prefix().str() + match.suffix().str();
//			next_array++;
//			std::sregex_iterator tmp2 = next_array;
//			if (tmp2 == end)
//			{
//				subjElem = restOfString;
//				next_array = std::sregex_iterator(subjElem.begin(), subjElem.end(), array);
//			}
//
//		}
//		if (restOfString != "")
//		{
//			subjElem = restOfString;
//			restOfString = "";
//		}
//		return node;
//	}
//	catch (std::regex_error& e) {
//
//		std::cout << e.what();
//	}
//	out_data << "\n\nEND\n\n" << std::endl;
//	std::cout << "\n\nEND\n\n" << std::endl;
//}
//
//JSONNode* parseKeyListElementsInFile(std::ofstream &out_data, const std::string & s, std::string printOffset)
//{
//	printOffset += "\t";
//	out_data << "value:\t\t" << s << std::endl;
//	std::cout << "value:\t\t" << s << std::endl;
//	std::string subject = s;
//	try
//	{
//		JSONNode *node = new JSONNode;
//		JSONNode* parentNode = new JSONNode;
//		std::sregex_iterator end;
//		std::string restOfString;
//		std::regex whole_file("\\s*\\{([\\{A-Za-z0-9\":\\#\\n\\t\\s,\\{\\}\\[\\]]+)\\}\\s*");
//		/*	the whole file between the structure { }*/
//		std::sregex_iterator whole_file_it(subject.begin(), subject.end(), whole_file);
//		std::regex child_array("\\{\\s*([a-zA-Z0-9\"]+)\\s*:\\s*\\n*\\t*\\[\\s*([a-zA-Z\"\\[\\]#0-9\\s*:,\\{\\}]+)\\s*\\]\\s*\\}");
//		/*delimits the key from an array with children between the []
//		the content between [] is parsed then with the regex called subgroup (the next regex)*/
//		std::sregex_iterator next_child_array(subject.begin(), subject.end(), child_array);
//		while (next_child_array != end)
//		{
//			auto match = *next_child_array;
//			out_data << "\n" + printOffset << match[1].str() << std::endl;
//			std::cout << "\n" + printOffset << match[1].str() << std::endl;
//			//JSONNode* parentNode = parseKeyListElements(match[2].str(), printOffset);
//			//if (!(*parentNode == *(new JSONNode)))
//			//{
//			//	parentNode->key = match[1].str();
//			//	node->parentNode.insert(std::make_pair(match[1].str(), match[2].str()));
//			//}
//			std::string groupValue = match[2].str();
//			std::regex subgrup_child_array("\\{\\s*([\\{A-Za-z0-9\":\\#\\n\\t\\s,\\[\\]\\{\\}]+?\\})\\s*\\}");
//			/* key : [{subgroup from array}] checked*/
//			std::sregex_iterator next_subgrup_child_array(groupValue.begin(), groupValue.end(), subgrup_child_array);
//			for (; next_subgrup_child_array != end; ++next_subgrup_child_array)
//			{
//				//JSONNode *node = new JSONNode;
//				auto groupMatch = *next_subgrup_child_array;
//				out_data << "\n" + printOffset << groupMatch[1].str() << std::endl;
//				std::cout << "\n" + printOffset << groupMatch[1].str() << std::endl;
//				std::string subgrMatch = groupMatch[1].str();
//				/////////////////
//				std::regex children("([a-z0-9\"]+)\\s*:\\s*\\{\\s*([\\#A-Za-z0-9\"\\s*\n*\t*:\\[\\],]+?)\\s*\\},{0,1}");
//				std::sregex_iterator next_child(subgrMatch.begin(), subgrMatch.end(), children);
//				while (next_child != end)
//				{
//					auto match = *next_child;
//					out_data << "\n" + printOffset << match[1].str() << std::endl;
//					std::cout << "\n" + printOffset << match[1].str() << std::endl;
//					JSONNode* child = parseKeyListElements(match[2].str(), printOffset);
//					// see if child is empty
//					if (!(*child == *(new JSONNode)))
//					{
//						child->key = match[1].str();
//						node->child.push_back(child);
//					}
//
//					node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
//					next_child++;
//					restOfString = match.prefix().str() + match.suffix().str();
//					std::sregex_iterator tmp = next_child;
//					if (tmp == end)
//					{
//						subject = restOfString;
//						next_child = std::sregex_iterator(subject.begin(), subject.end(), children);
//					}
//				}
//				///////////////////////
//				std::regex primitives("\\s*([a-zA-Z0-9\"]+)\\s*:\\s*([a-zA-Z0-9#\"]+),{0,1}\\s*");
//				/* checked with or without ,
//				"color": "yellow" */
//				std::sregex_iterator next_primitive(subgrMatch.begin(), subgrMatch.end(), primitives);
//				while (next_primitive != end)
//				{
//					auto match = *next_primitive;
//					out_data << "\n" + printOffset << match[1].str() << std::endl;
//					std::cout << "\n" + printOffset << match[1].str() << std::endl;
//					JSONNode* child = parseKeyListElements(match[2].str(), printOffset);
//					// todo: see if child is empty - done
//					if (!(*child == *(new JSONNode)))
//					{
//						child->key = match[1].str();
//						node->child.push_back(child);
//					}
//					node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
//					restOfString = match.prefix().str() + match.suffix().str();
//					next_primitive++;
//					std::sregex_iterator tmp3 = next_primitive;
//					if (tmp3 == end)
//					{
//						subject = restOfString;
//						next_primitive = std::sregex_iterator(subject.begin(), subject.end(), primitives);
//					}
//
//				}
//				///////////////////////
//				std::regex array("([a-zA-Z0-9\"]+)\\s*:\\s*(\\[[a-zA-Z0-9\",\\s*]+\\]),{0,1}");
//				/* key : array -checked with or without ,
//				"hex" : ["5", "5", "7", "1"] */
//				std::sregex_iterator next_array(subgrMatch.begin(), subgrMatch.end(), array);
//				while (next_array != end)
//				{
//					auto match = *next_array;
//					out_data << "\n" + printOffset << match[1].str() << std::endl;
//					std::cout << "\n" + printOffset << match[1].str() << std::endl;
//					JSONNode* child = parseKeyListElements(match[2].str(), printOffset);
//					// see if child is empty
//					if (!(*child == *(new JSONNode)))
//					{
//						child->key = match[1].str();
//						node->child.push_back(child);
//					}
//					node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
//					restOfString = match.prefix().str() + match.suffix().str();
//					next_array++;
//					std::sregex_iterator tmp2 = next_array;
//					if (tmp2 == end)
//					{
//						subject = restOfString;
//						next_array = std::sregex_iterator(subject.begin(), subject.end(), array);
//					}
//				}
//				if (!(*node == *(new JSONNode)))
//				{
//					parentNode->child.push_back(node);
//				}
//				else
//				{
//					delete node;
//				}
//			}
//			next_child_array++;
//			restOfString = match.prefix().str() + match.suffix().str();
//			std::sregex_iterator tmp1 = next_child_array;
//			if (tmp1 == end)
//			{
//				subject = restOfString;
//				next_child_array = std::sregex_iterator(subject.begin(), subject.end(), child_array);
//			}
//		}
//
//		if (restOfString != "")
//		{
//			subject = restOfString;
//			restOfString = "";
//		}
//
//		return parentNode;
//	}
//	catch (std::regex_error& e) {
//		out_data << e.what();
//		std::cout << e.what();
//	}
//	out_data << "\n\nEND\n\n" << std::endl;
//	std::cout << "\n\nEND\n\n" << std::endl;
//	return nullptr;
//}

void test::mergeInFile(std::ofstream &out_data, JSONNode *node1, JSONNode *node2)
{
	if (node1->child.size() == node2->child.size())
	{
		if (!(node1->child.empty()))
		{
			if (node1->child == node2->child)
			{
				removeKeyValDuplicates(node1->keyVal, node2->keyVal);
				removeChildDuplicates(node1->child, node2->child);
				std::cout << node1->child << std::endl;
				std::cout << "\nChildren are the same\n";
				out_data << node1->child << std::endl;
				out_data << "\nChildren are the same\n";

			}
			if (node1->keyVal.size() == node2->keyVal.size())
			{
				if (!(node1->keyVal.empty()))
				{
					if (node1->keyVal == node2->keyVal)
					{
						removeKeyValDuplicates(node1->keyVal, node2->keyVal);
						std::cout << node1->keyVal << std::endl;
						std::cout << "\nKeyVal are the same\n";
						out_data << node1->keyVal << std::endl;
						out_data << "\nKeyVal are the same\n";
					}
					else
					{
						removeKeyValDuplicates(node1->keyVal, node2->keyVal);
						std::cout << node1->keyVal << node2->keyVal << std::endl;
						std::cout << "\nKeyVal has duplicates or different values\n";
						out_data << node1->keyVal << node2->keyVal << std::endl;
						out_data << "\nKeyVal has duplicates or different values\n";
					}
				}
				else 
				{ 
				std::cout << "\nEMPTY keyVal\n";
				}
			}
			else
			{
				removeChildDuplicates(node1->child, node2->child);
				std::cout << node1->child << node2->child << std::endl;
				std::cout << "\nChildren has duplicates or different values\n";
				out_data << node1->child << node2->child << std::endl;
				out_data << "\nChildren has duplicates or different values\n";
			}
		}
		else 
		{
			std::cout << "\nEMPTY keyVal\n"; 
		}
	}
	else
	{
		std::cout << "\nDiferent size of children\n";
	}
	////////////////
	if (node1->keyVal.size() == node2->keyVal.size())
	{
		if (!(node1->keyVal.empty()))
		{
			if (node1->keyVal == node2->keyVal)
			{
				std::cout << node1->keyVal << std::endl;
				out_data << node1->keyVal << std::endl;
				std::cout << "\nKeyVal are the same\n";
				out_data << "\nKeyVal are the same\n";
			}
			else
			{
				removeKeyValDuplicates(node1->keyVal, node2->keyVal);
				out_data << node1->keyVal << node2->keyVal << std::endl;
				std::cout << node1->keyVal << node2->keyVal << std::endl;
				std::cout << "\nKeyVal has duplicates or different values\n";
				out_data << "\nKeyVal has duplicates or different values\n";
			}
		}
		else 
		{ 
			std::cout << "\nEMPTY keyVal\n"; 
		}
	}
	else
	{
		std::cout << "\nDifferent sizes of keyVal\n";
	}
}

//=======================================================================BOOL/OPERATORS============================================================================

bool test::operator==(const std::map<std::string, std::string> &kV1, const std::map<std::string, std::string> &kV2)
{
	if (kV1.size() == kV2.size())
	{
		//bool aux = false;
		if (kV1.empty())
		{
			//std::cout << "\nEmpty keyVal";
			return true;
		}
		else{ //std::cout << "\nKeyVal found";
		}
		for (auto& pair : kV1)
		{
			auto found = kV2.find(pair.first);
			if (found != kV2.end())
			{
				//return pair.second == found->second;
				if (pair.second == found->second)
				{
					continue;
				}
				else { return false; }
			}
			else { return false; }
		}
	}
	else { return false; }
	return true;
}

bool test::operator==(const std::map <std::string, std::vector<JSONNode*>> &kC1, const std::map <std::string, std::vector<JSONNode*>> &kC2)
{
	if (kC1.size() == kC2.size())
	{
		if (kC1.empty())
		{
			//std::cout << "\nEmpty keyChild";
			return true;
		}
		else{ //std::cout << "\nKeyChild found";
		}
		for (auto& pair : kC1)
		{
			auto found = kC2.find(pair.first);
			if (found != kC2.end()){
				//return pair.second == found->second;
				if (pair.second == found->second)
				{
					continue;
				}
				else { return false; }
			}
			else { return false; }
		}
	}
	else { return false; }
	return true;
}

bool test::operator==(const std::vector<JSONNode*> &ch1, const std::vector<JSONNode*> &ch2)
{
	if (ch1.size() == ch2.size())
	{
		if (ch1.empty())
		{
			//std::cout << "\nEmpty child";
			return true;
		}
		else
		{//std::cout << "\nChild found"; 

			for (int i = 0; i < ch1.size(); ++i)
			{
				if (*ch1[i] == *ch2[i]){
					continue;
				}
				else{ return false; }
			}
			return true;
		}
	}
	else{ return false; }
}

bool test::operator==(const JSONNode &node1, const JSONNode &node2)
{
	if (!(node1.keyVal == node2.keyVal)) { return false; }
	if (!(node1.parentNode == node2.parentNode)) { return false; }
	if (!(node1.child == node2.child)) { return false; }
	return true;
}

bool test::compare(JSONNode *node1, JSONNode *node2)
{
	if (node1->parentNode.size() == node2->parentNode.size())
	{
		for (auto it = node1->parentNode.begin(); it != node1->parentNode.end(); ++it)
		{
			for (auto childIT = it->second.begin(); childIT != it->second.end(); ++childIT)
			{
				size_t index = std::distance(it->second.begin(), childIT);
				for (auto& pair : node2->parentNode)
					compare(*childIT, *(pair.second.begin() + index));
			}
		}
		if (node1->key == node2->key)
		{
			if (node1->parentNode == node2->parentNode)
			{
				return true;
			}
			else
				return false;

			if (node1->keyVal == node2->keyVal)
			{
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}

std::ostream& test::operator<<(std::ostream& os, const std::map<std::string, std::string> &kV)
{
	for (const auto &p : kV)
	{
		os << p.first << ": ";
		os << p.second;
		os << std::endl;
	}
	return os;
}

std::ostream& test::operator<<(std::ostream& os, const std::map<std::string, std::vector<JSONNode*>> &kC)
{
	for (const auto &p : kC)
	{
		os << p.first << ": ";
		for (JSONNode *n : p.second) os << n->parentNode << ' ';
		os << std::endl;
	}
	return os;
}

std::ostream& test::operator<<(std::ostream& os, const std::vector<JSONNode*> &C)
{
	for (int i = 0; i < C.size(); ++i) {
		os << C[i]->keyVal;
		if (i != C.size() - 1)
			os << ", ";
	}
	return os;
}
