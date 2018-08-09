#include <fstream>
#include <istream>
#include <iostream>
#include <string>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <windows.h>
#include <sstream>      /* std::istringstream*/
#include "ProjectLib.h"

using namespace test;
/*=======================================================================FUNCTIONS===============================================================================*/
JSONNode* test::parseElements(const std::string & elements, std::string printOffset)
{
	printOffset += "\t";
	std::string subjElem = elements;
	try {
		JSONNode *subgrChildren = new JSONNode;
		std::sregex_iterator end;
		std::string restOfString;

		std::regex whole_file("\\s*\\{([\\{A-Za-z0-9\":\\#\\n\\t\\s,\\{\\}\\[\\]]+)\\}\\s*"); /*regex for whole file*/
		std::sregex_iterator whole_file_it(subjElem.begin(), subjElem.end(), whole_file);

		std::regex children("([a-z0-9\"]+)\\s*:\\s*\\s*(\\{[\\#A-Za-z0-9\"\\s*\n*\t*:\\[\\],]+?\\})\\s*,{0,1}"); /*regex for key:{ values} children*/
		std::sregex_iterator next_child(subjElem.begin(), subjElem.end(), children);

		/*iterate throw the children*/
		while (next_child != end)
		{
			auto match = *next_child;
			std::cout << "\n" + printOffset << match[1].str() << " :" << std::endl;
			std::cout << "\n" + printOffset << match[2].str() << std::endl;
			JSONNode* child = parseElements(match[2].str(), printOffset);
			if (!(*child == *(new JSONNode)))  /* see if child is empty*/
			{
				child->key = match[1].str();
				subgrChildren->child.push_back(child);
			}
			/*node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));*/
			next_child++;
			restOfString = match.prefix().str() + match.suffix().str();
			std::sregex_iterator tmp = next_child;
			if (tmp == end)
			{
				subjElem = restOfString;
				next_child = std::sregex_iterator(subjElem.begin(), subjElem.end(), children);
			}
			else
			{
			}
		}

		std::regex primitives("\\s*([a-zA-Z0-9\"]+)\\s*:\\s*([a-zA-Z0-9#\"]+),{0,1}\\s*"); /*regex for key:val primitives*/
		std::sregex_iterator next_primitive(subjElem.begin(), subjElem.end(), primitives);

		/*iterate throw primitives*/
		while (next_primitive != end) {
			auto match = *next_primitive;
			std::cout << "\n\t" << match[1].str() << " :" << std::endl;
			std::cout << "\n\t\t" << match[2].str() << std::endl;
			JSONNode* keyVal = parseElements(match[2].str(), printOffset);
			std::string val;
			if (subgrChildren->keyVal.count(match[1].str()) > 0)
			{
				auto found = subgrChildren->keyVal.find(match[1].str());
				if (found != subgrChildren->keyVal.end())
				{
					val = found->second + " , " + match[2].str();
					subgrChildren->keyVal[match[1].str()] = val;
				}
			}
			subgrChildren->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
			restOfString = match.prefix().str() + match.suffix().str();
			next_primitive++;
			std::sregex_iterator tmp3 = next_primitive;
			if (tmp3 == end)
			{
				subjElem = restOfString;
				next_primitive = std::sregex_iterator(subjElem.begin(), subjElem.end(), primitives);
			}
		}

		std::regex array("([a-zA-Z0-9\"]+)\\s*:\\s*(\\[[a-zA-Z0-9\",\\s*]+\\]),{0,1}"); /*regex for key:[val,..] array*/
		std::sregex_iterator next_array(subjElem.begin(), subjElem.end(), array);

		/*iterate throw arrays*/
		while (next_array != end) {
			auto match = *next_array;
			std::cout << "\n" + printOffset << match[1].str() << " :" << std::endl;
			std::cout << "\n\t\t" << match[2].str() << std::endl;
			JSONNode* keyVect = parseElements(match[2].str(), printOffset);
			std::string val;
			if (subgrChildren->keyVect.count(match[1].str()) > 0)
			{
				auto found = subgrChildren->keyVect.find(match[1].str());
				if (found != subgrChildren->keyVect.end())
				{
					val = found->second + " , " + match[2].str();
					subgrChildren->keyVect[match[1].str()] = val;
				}
			}
			subgrChildren->keyVect.insert(std::make_pair(match[1].str(), match[2].str()));
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
		return subgrChildren;
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
		JSONNode *subgrChildren = new JSONNode;
		JSONNode* parentNode = new JSONNode;
		std::sregex_iterator end;
		std::string restOfString;

		std::regex whole_file("\\s*\\{([\\{A-Za-z0-9\":\\#\\n\\t\\s,\\{\\}\\[\\]]+)\\}\\s*"); 		/*	the whole file between the structure { }*/
		std::sregex_iterator whole_file_it(subject.begin(), subject.end(), whole_file);

		std::regex child_array("\\{\\s*([a-zA-Z0-9\"]+)\\s*:\\s*\\n*\\t*\\[\\s*([a-zA-Z\"\\[\\]#0-9\\s*:,\\{\\}]+)\\s*\\]\\s*\\}"); /*delimits the key from an array with children between the [].	The content between [] is parsed then with the regex called subgroup (the next regex)*/
		std::sregex_iterator next_child_array(subject.begin(), subject.end(), child_array);

		/*iterate throw the child array*/
		while (next_child_array != end)
		{
			auto match = *next_child_array;
			std::cout << "\n" + printOffset << match[1].str() << " :" << std::endl;
			parentNode->key = match[1].str();  /*the parent key*/

			std::string groupValue = match[2].str(); /*the parent value*/
			std::regex subgrup_child_array("\\{\\s*([\\{A-Za-z0-9\":\\#\\n\\t\\s,\\[\\]\\{\\}]+?\\})\\s*\\}"); /* regex for  the subgr of an array*/

			/*iterate throw subgroups*/
			std::sregex_iterator next_subgrup_child_array(groupValue.begin(), groupValue.end(), subgrup_child_array);
			for (; next_subgrup_child_array != end; ++next_subgrup_child_array)
			{
				auto groupMatch = *next_subgrup_child_array;
				std::cout << "\n" + printOffset << groupMatch[1].str() << std::endl;
				JSONNode* subgrChildren = parseElements(match[1].str(), printOffset);
				std::string subgrMatch = groupMatch[1].str();

				/*iterate throw children*/
				std::regex children("([a-z0-9\"]+)\\s*:\\s*\\{\\s*([\\#A-Za-z0-9\"\\s*\n*\t*:\\[\\],]+?)\\s*\\},{0,1}");
				std::sregex_iterator next_child(subgrMatch.begin(), subgrMatch.end(), children);
				while (next_child != end)
				{
					auto match = *next_child;
					std::cout << "\n" + printOffset << match[1].str() << " : {" << std::endl;
					JSONNode* child = parseElements(match[2].str(), printOffset); /*call parseElements function for the elements of the children*/
					/* see if child is empty*/
					if (!(*child == *(new JSONNode)))
					{
						child->key = match[1].str();
						subgrChildren->child.push_back(child);
					}
					next_child++;
					restOfString = match.prefix().str() + match.suffix().str(); /*update the string to parse with the rest of elements*/
					std::sregex_iterator tmp = next_child;
					if (tmp == end)
					{
						subject = restOfString;
						next_child = std::sregex_iterator(subject.begin(), subject.end(), children);
					}
					std::cout << "\n" + printOffset << "\t}" << std::endl;
				}

				/*iterate throw primitives*/
				std::regex primitives("\\s*([a-zA-Z0-9\"]+)\\s*:\\s*([a-zA-Z0-9#\"]+),{0,1}\\s*"); /* checked with or without ,"color": "yellow" */
				std::sregex_iterator next_primitive(subject.begin(), subject.end(), primitives);
				while (next_primitive != end)
				{
					auto match = *next_primitive;
					JSONNode* keyVal = parseElements(match[2].str(), printOffset);

					std::string val; /*if the key already exist in a subgroup, merge the values by adding to the unique key the first value and the second value*/
					if (subgrChildren->keyVal.count(match[1].str()) > 0)
					{
						auto found = subgrChildren->keyVal.find(match[1].str());
						if (found != subgrChildren->keyVal.end())
						{
							val = found->second + " , " + match[2].str();
							subgrChildren->keyVal[match[1].str()] = val;
						}
					}
					subgrChildren->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
					restOfString = match.prefix().str() + match.suffix().str(); /*update the string*/
					next_primitive++;
					std::sregex_iterator tmp3 = next_primitive;
					if (tmp3 == end)
					{
						subject = restOfString;
						next_primitive = std::sregex_iterator(subject.begin(), subject.end(), primitives);
					}

				}

				/*iterate throw array*/
				std::regex array("([a-zA-Z0-9\"]+)\\s*:\\s*(\\[[a-zA-Z0-9\",\\s*]+\\]),{0,1}");	/* key : array -checked with or without ,"hex" : ["5", "5", "7", "1"] */
				std::sregex_iterator next_array(subgrMatch.begin(), subgrMatch.end(), array);
				while (next_array != end)
				{
					auto match = *next_array;
					JSONNode* keyVect = parseElements(match[2].str(), printOffset);
					subgrChildren->keyVect.insert(std::make_pair(match[1].str(), match[2].str()));
					restOfString = match.prefix().str() + match.suffix().str();
					next_array++;
					std::sregex_iterator tmp2 = next_array;
					if (tmp2 == end)
					{
						subject = restOfString;
						next_array = std::sregex_iterator(subject.begin(), subject.end(), array);
					}
				}

				/*update parentNode*/
				if (!(*subgrChildren == *(new JSONNode)))
				{
					parentNode->subgrChildrenVect.push_back(subgrChildren);
				}
				else
				{
					delete subgrChildren;
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

JSONNode* test::structureElements(const std::string & subject)
{
	JSONNode *parsedNode = parseElements(subject); /*parse the subject*/
	std::map<std::string, std::string>::iterator itForKeyVal;
	std::map<std::string, std::string>::iterator itForKeyVal1;
	std::map<std::string, std::string>::iterator itForKeyVect;
	std::map<std::string, std::string>::iterator itForKeyVect1;
	std::vector<JSONNode*>::iterator itForChild;
	std::cout << "\t{";
	/*----------organize the KeyVal structure to have a valide JSON format----------*/
	if (parsedNode->keyVal.size() > 0)
	{
		for (itForKeyVal = parsedNode->keyVal.begin(), itForKeyVal1 = parsedNode->keyVal.begin(); itForKeyVal1++ != parsedNode->keyVal.end(); itForKeyVal++)
		{
			if (itForKeyVal1 != parsedNode->keyVal.end())
			{
				std::cout << "\n\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
			}
			else
			{
				std::cout << "\n\t" << itForKeyVal->first << ": " << itForKeyVal->second << std::endl;
				break;
			}
		}
	}

	/*----------organize the KeyVect structure to have a valide JSON format----------*/
	if (parsedNode->keyVect.size() > 0)
	{
		for (itForKeyVect = parsedNode->keyVect.begin(), itForKeyVect1 = parsedNode->keyVect.begin(); itForKeyVect1++ != parsedNode->keyVect.end(); itForKeyVect++)
		{
			if (itForKeyVect1 != parsedNode->keyVect.end())
			{
				std::cout << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << "\,";
			}
			else
			{
				std::cout << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << std::endl;
				break;
			}
		}
	}

	/*----------organize the children structure  to have a valide JSON format------*/
	if (parsedNode->child.size() > 0)
	{
		int i = 0;

		/*organize the structure for the children besides the last child*/
		while (i < parsedNode->child.size() - 1)
		{
			std::cout << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
			/*organize the keyVal structure from a child*/
			if (parsedNode->child[i]->keyVal.size() > 0)
			{
				std::cout << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
				for (itForKeyVal = parsedNode->child[i]->keyVal.begin(), itForKeyVal1 = parsedNode->child[i]->keyVal.begin(); itForKeyVal1++ != parsedNode->child[i]->keyVal.end(); itForKeyVal++)
				{
					if (itForKeyVal1 != parsedNode->child[i]->keyVal.end())
					{
						std::cout << "\n\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
					}
					else
					{
						std::cout << "\n\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << std::endl;
						break;
					}
				}
				std::cout << "\t\t}," << std::endl;
			}
			else

				/*organize the keyVect structure from a child*/
				if (parsedNode->child[i]->keyVect.size() > 0)
				{
					std::cout << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
					for (itForKeyVect = parsedNode->child[i]->keyVect.begin(), itForKeyVect1 = parsedNode->child[i]->keyVect.begin(); itForKeyVect1++ != parsedNode->child[i]->keyVect.end(); itForKeyVect++)
					{
						if (itForKeyVect1 != parsedNode->child[i]->keyVect.end())
						{
							std::cout << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << "\,";
						}
						else
						{
							std::cout << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << std::endl;
							break;
						}
					}
					std::cout << "\t\t}," << std::endl;
				}

				else
				{
					break;
				}
			i++;
		}

		/*organize the structure for the last child*/
		if (i = parsedNode->child.size() - 1)
		{
			/*organize the keyVect structure from a keyVal*/
			if (parsedNode->child[i]->keyVal.size() > 0)
			{
				std::cout << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
				for (itForKeyVal = parsedNode->child[i]->keyVal.begin(), itForKeyVal1 = parsedNode->child[i]->keyVal.begin(); itForKeyVal1++ != parsedNode->child[i]->keyVal.end(); itForKeyVal++)
				{
					if (itForKeyVal1 != parsedNode->child[i]->keyVal.end())
					{
						std::cout << "\n\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
					}
					else
					{
						std::cout << "\n\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << std::endl;
						break;
					}
				}
				std::cout << "\t\t}" << std::endl;
			}
			else

				/*organize the keyVect structure from a keyVect*/
				if (parsedNode->child[i]->keyVect.size() > 0)
				{
					std::cout << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
					for (itForKeyVect = parsedNode->child[i]->keyVect.begin(), itForKeyVect1 = parsedNode->child[i]->keyVect.begin(); itForKeyVect1++ != parsedNode->child[i]->keyVect.end(); itForKeyVect++)
					{
						if (itForKeyVect1 != parsedNode->child[i]->keyVect.end())
						{
							std::cout << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << "\,";
						}
						else
						{
							std::cout << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << std::endl;
							break;
						}
					}
					std::cout << "\t\t}" << std::endl;
				}

				else
				{
					/*break;*/
				}
		}
	}
	else{}
	std::cout << "\t" << "}" << std::endl;
	return parsedNode;
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
{
	std::ifstream ifs1("../../JSONParserTest/resources/colors.json");
	std::ifstream ifs2("../../JSONParserTest/resources/colors2.json");
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
			if (found != kV2.end()) /*if the key of kv1 and kv2 is equal*/
			{
				if (kV1 == kV2)/*if kv1 and kv2 is equal*/
				{
					kV2.erase(found);/*erase kv2*/
				}
				if (pair.second == found->second)/*if the value of kv1 is equal to kv2*/
				{
					kV2.erase(found);/*erase kv2*/
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

JSONNode* test::indexElements(JSONNode *node)
{
	int i = 1;
	std::map<std::string, std::string>::iterator itForKeyVal;
	if (!(node->keyVal.empty()))
	{
		for (itForKeyVal = node->keyVal.begin(); itForKeyVal != node->keyVal.end(); itForKeyVal++, i++)
		{
			std::cout << "\t" << itForKeyVal->first << "[" << std::to_string(i) << "]:" << itForKeyVal->second << std::endl;
		}
	}
	return node;
}

void test::mergeElements(JSONNode *node1, JSONNode *node2)
{
	/*----------merge the children with primitives of KeyVal/KeyVect-------------*/
	if (node1->child.size() == node2->child.size())
	{
		if (!(node1->child.empty()))
		{
			for (int i = 0; i < node1->child.size(); i++)
			{
				if (node1->child == node2->child)
				{
					removeKeyValDuplicates(node1->keyVal, node2->keyVal);
					removeKeyValDuplicates(node1->keyVect, node2->keyVect);
					removeChildDuplicates(node1->child, node2->child);
					std::cout << node1->child << std::endl;
					std::cout << "\nChildren are the same\n";
				}
				/*keyVal*/
				if (node1->child[i]->keyVal.size() == node2->child[i]->keyVal.size())
				{
					if (!(node1->child[i]->keyVal.empty()))
					{
						if (node1->child[i]->keyVal == node2->child[i]->keyVal)
						{
							removeKeyValDuplicates(node1->child[i]->keyVal, node2->child[i]->keyVal);
							std::cout << node1->child[i]->keyVal << std::endl;
							std::cout << "\nKeyVal are the same\n";
						}
						else
						{
							removeKeyValDuplicates(node1->child[i]->keyVal, node2->child[i]->keyVal);
							std::cout << node1->child[i]->keyVal << node2->child[i]->keyVal << std::endl;
							std::cout << "\nKeyVal has duplicates or different values\n";
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
					std::cout << "\nDifferent sizes of keyVal\n";
				}

				/*keyVect*/
				if (node1->child[i]->keyVect.size() == node2->child[i]->keyVect.size())
				{
					if (!(node1->child[i]->keyVect.empty()))
					{
						if (node1->child[i]->keyVect == node2->child[i]->keyVect)
						{
							removeKeyValDuplicates(node1->child[i]->keyVect, node2->child[i]->keyVect);
							std::cout << node1->child[i]->keyVect << std::endl;
							std::cout << "\keyVect are the same\n";
						}
						else
						{
							removeKeyValDuplicates(node1->child[i]->keyVect, node2->child[i]->keyVect);
							std::cout << node1->child[i]->keyVect << node2->child[i]->keyVect << std::endl;
							std::cout << "\keyVect has duplicates or different values\n";
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
					std::cout << "\nDifferent sizes of keyVal\keyVect \n";
				}
			}
		}
		else
		{
			std::cout << "\nEMPTY keyVal\keyVect\n";
		}
	}
	else
	{
		std::cout << "\nDiferent size of children\n";
	}

	/*----------merge the primitive KeyVal----------*/
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

	//----------merge the primitive KeyVect
	if (node1->keyVect.size() == node2->keyVect.size())
	{
		if (!(node1->keyVect.empty()))
		{
			if (node1->keyVect == node2->keyVect)
			{
				std::cout << node1->keyVect << std::endl;
				std::cout << "\keyVect are the same\n";
			}
			else
			{
				removeKeyValDuplicates(node1->keyVect, node2->keyVect);
				std::cout << node1->keyVect << node2->keyVect << std::endl;
				std::cout << "\keyVect has duplicates or different values\n";
			}
		}
		else
		{
			std::cout << "\nEMPTY keyVect\n";
		}
	}
	else
	{
		std::cout << "\nDifferent sizes of keyVect\n";
	}
}

/*===============================================================FUNCTIONS THAT PRINT IN A FILE====================================================================*/

JSONNode* test::parseElementsInFile(std::ofstream &out_data, const std::string & elements, std::string printOffset)
{
	printOffset += "\t";
	std::string subjElem = elements;
	try {
		JSONNode *subgrChildren = new JSONNode;
		std::sregex_iterator end;
		std::string restOfString;

		std::regex whole_file("\\s*\\{([\\{A-Za-z0-9\":\\#\\n\\t\\s,\\{\\}\\[\\]]+)\\}\\s*"); /*regex for whole file*/
		std::sregex_iterator whole_file_it(subjElem.begin(), subjElem.end(), whole_file);

		std::regex children("([a-z0-9\"]+)\\s*:\\s*\\s*(\\{[\\#A-Za-z0-9\"\\s*\n*\t*:\\[\\],]+?\\})\\s*,{0,1}"); /*regex for key:{ values} children*/
		std::sregex_iterator next_child(subjElem.begin(), subjElem.end(), children);

		/*iterate throw the children*/
		while (next_child != end)
		{
			auto match = *next_child;
			std::cout << "\n" + printOffset << match[1].str() << " :" << std::endl;
			std::cout << "\n" + printOffset << match[2].str() << std::endl;
			out_data << "\n" + printOffset << match[1].str() << " :" << std::endl;
			out_data << "\n" + printOffset << match[2].str() << std::endl;
			JSONNode* child = parseElementsInFile(out_data, match[2].str(), printOffset);
			if (!(*child == *(new JSONNode)))  /* see if child is empty*/
			{
				child->key = match[1].str();
				subgrChildren->child.push_back(child);
			}
			/*node->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));*/
			next_child++;
			restOfString = match.prefix().str() + match.suffix().str();
			std::sregex_iterator tmp = next_child;
			if (tmp == end)
			{
				subjElem = restOfString;
				next_child = std::sregex_iterator(subjElem.begin(), subjElem.end(), children);
			}
			else
			{
			}
		}

		std::regex primitives("\\s*([a-zA-Z0-9\"]+)\\s*:\\s*([a-zA-Z0-9#\"]+),{0,1}\\s*"); /*regex for key:val primitives*/
		std::sregex_iterator next_primitive(subjElem.begin(), subjElem.end(), primitives);

		/*iterate throw primitives*/
		while (next_primitive != end) {
			auto match = *next_primitive;
			std::cout << "\n\t" << match[1].str() << " :" << std::endl;
			std::cout << "\n\t\t" << match[2].str() << std::endl;
			out_data << "\n\t" << match[1].str() << " :" << std::endl;
			out_data << "\n\t\t" << match[2].str() << std::endl;
			JSONNode* keyVal = parseElementsInFile(out_data, match[2].str(), printOffset);
			std::string val;
			if (subgrChildren->keyVal.count(match[1].str()) > 0)
			{
				auto found = subgrChildren->keyVal.find(match[1].str());
				if (found != subgrChildren->keyVal.end())
				{
					val = found->second + " , " + match[2].str();
					subgrChildren->keyVal[match[1].str()] = val;
				}
			}
			subgrChildren->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
			restOfString = match.prefix().str() + match.suffix().str();
			next_primitive++;
			std::sregex_iterator tmp3 = next_primitive;
			if (tmp3 == end)
			{
				subjElem = restOfString;
				next_primitive = std::sregex_iterator(subjElem.begin(), subjElem.end(), primitives);
			}
		}

		std::regex array("([a-zA-Z0-9\"]+)\\s*:\\s*(\\[[a-zA-Z0-9\",\\s*]+\\]),{0,1}"); /*regex for key:[val,..] array*/
		std::sregex_iterator next_array(subjElem.begin(), subjElem.end(), array);

		/*iterate throw arrays*/
		while (next_array != end) {
			auto match = *next_array;
			std::cout << "\n" + printOffset << match[1].str() << " :" << std::endl;
			std::cout << "\n\t\t" << match[2].str() << std::endl;
			out_data << "\n" + printOffset << match[1].str() << " :" << std::endl;
			out_data << "\n\t\t" << match[2].str() << std::endl;
			JSONNode* keyVect = parseElementsInFile(out_data, match[2].str(), printOffset);
			std::string val;
			if (subgrChildren->keyVect.count(match[1].str()) > 0)
			{
				auto found = subgrChildren->keyVect.find(match[1].str());
				if (found != subgrChildren->keyVect.end())
				{
					val = found->second + " , " + match[2].str();
					subgrChildren->keyVect[match[1].str()] = val;
				}
			}
			subgrChildren->keyVect.insert(std::make_pair(match[1].str(), match[2].str()));
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
		return subgrChildren;
	}
	catch (std::regex_error& e) {
		std::cout << e.what();
	}
	std::cout << "\n\nEND\n\n" << std::endl;
	out_data << "\n\nEND\n\n" << std::endl;
}

JSONNode* test::parseKeyListElementsInFile(std::ofstream &out_data, const std::string & s, std::string printOffset)
{
	printOffset += "\t";
	std::cout << "value:\t\t" << s << std::endl;
	out_data << "value:\t\t" << s << std::endl;
	std::string subject = s;
	try
	{
		JSONNode *node = new JSONNode;
		JSONNode *subgrChildren = new JSONNode;
		JSONNode* parentNode = new JSONNode;
		std::sregex_iterator end;
		std::string restOfString;

		std::regex whole_file("\\s*\\{([\\{A-Za-z0-9\":\\#\\n\\t\\s,\\{\\}\\[\\]]+)\\}\\s*"); 		/*	the whole file between the structure { }*/
		std::sregex_iterator whole_file_it(subject.begin(), subject.end(), whole_file);

		std::regex child_array("\\{\\s*([a-zA-Z0-9\"]+)\\s*:\\s*\\n*\\t*\\[\\s*([a-zA-Z\"\\[\\]#0-9\\s*:,\\{\\}]+)\\s*\\]\\s*\\}"); /*delimits the key from an array with children between the [].	The content between [] is parsed then with the regex called subgroup (the next regex)*/
		std::sregex_iterator next_child_array(subject.begin(), subject.end(), child_array);

		/*iterate throw the child array*/
		while (next_child_array != end)
		{
			auto match = *next_child_array;
			std::cout << "\n" + printOffset << match[1].str() << " :" << std::endl;
			out_data << "\n" + printOffset << match[1].str() << " :" << std::endl;
			parentNode->key = match[1].str();  /*the parent key*/

			std::string groupValue = match[2].str(); /*the parent value*/
			std::regex subgrup_child_array("\\{\\s*([\\{A-Za-z0-9\":\\#\\n\\t\\s,\\[\\]\\{\\}]+?\\})\\s*\\}"); /* regex for  the subgr of an array*/

			/*iterate throw subgroups*/
			std::sregex_iterator next_subgrup_child_array(groupValue.begin(), groupValue.end(), subgrup_child_array);
			for (; next_subgrup_child_array != end; ++next_subgrup_child_array)
			{
				auto groupMatch = *next_subgrup_child_array;
				std::cout << "\n" + printOffset << groupMatch[1].str() << std::endl;
				out_data << "\n" + printOffset << groupMatch[1].str() << std::endl;
				JSONNode* subgrChildren = parseElementsInFile(out_data, match[1].str(), printOffset);
				std::string subgrMatch = groupMatch[1].str();

				/*iterate throw children*/
				std::regex children("([a-z0-9\"]+)\\s*:\\s*\\{\\s*([\\#A-Za-z0-9\"\\s*\n*\t*:\\[\\],]+?)\\s*\\},{0,1}");
				std::sregex_iterator next_child(subgrMatch.begin(), subgrMatch.end(), children);
				while (next_child != end)
				{
					auto match = *next_child;
					std::cout << "\n" + printOffset << match[1].str() << " : {" << std::endl;
					out_data << "\n" + printOffset << match[1].str() << " : {" << std::endl;
					JSONNode* child = parseElementsInFile(out_data, match[2].str(), printOffset); /*call parseElements function for the elements of the children*/
					/* see if child is empty*/
					if (!(*child == *(new JSONNode)))
					{
						child->key = match[1].str();
						subgrChildren->child.push_back(child);
					}
					next_child++;
					restOfString = match.prefix().str() + match.suffix().str(); /*update the string to parse with the rest of elements*/
					std::sregex_iterator tmp = next_child;
					if (tmp == end)
					{
						subject = restOfString;
						next_child = std::sregex_iterator(subject.begin(), subject.end(), children);
					}
					std::cout << "\n" + printOffset << "\t}" << std::endl;
					out_data << "\n" + printOffset << "\t}" << std::endl;
				}

				/*iterate throw primitives*/
				std::regex primitives("\\s*([a-zA-Z0-9\"]+)\\s*:\\s*([a-zA-Z0-9#\"]+),{0,1}\\s*"); /* checked with or without ,"color": "yellow" */
				std::sregex_iterator next_primitive(subject.begin(), subject.end(), primitives);
				while (next_primitive != end)
				{
					auto match = *next_primitive;
					JSONNode* keyVal = parseElementsInFile(out_data, match[2].str(), printOffset);

					std::string val; /*if the key already exist in a subgroup, merge the values by adding to the unique key the first value and the second value*/
					if (subgrChildren->keyVal.count(match[1].str()) > 0)
					{
						auto found = subgrChildren->keyVal.find(match[1].str());
						if (found != subgrChildren->keyVal.end())
						{
							val = found->second + " , " + match[2].str();
							subgrChildren->keyVal[match[1].str()] = val;
						}
					}
					subgrChildren->keyVal.insert(std::make_pair(match[1].str(), match[2].str()));
					restOfString = match.prefix().str() + match.suffix().str(); /*update the string*/
					next_primitive++;
					std::sregex_iterator tmp3 = next_primitive;
					if (tmp3 == end)
					{
						subject = restOfString;
						next_primitive = std::sregex_iterator(subject.begin(), subject.end(), primitives);
					}

				}

				/*iterate throw array*/
				std::regex array("([a-zA-Z0-9\"]+)\\s*:\\s*(\\[[a-zA-Z0-9\",\\s*]+\\]),{0,1}");	/* key : array -checked with or without ,"hex" : ["5", "5", "7", "1"] */
				std::sregex_iterator next_array(subgrMatch.begin(), subgrMatch.end(), array);
				while (next_array != end)
				{
					auto match = *next_array;
					JSONNode* keyVect = parseElementsInFile(out_data, match[2].str(), printOffset);
					subgrChildren->keyVect.insert(std::make_pair(match[1].str(), match[2].str()));
					restOfString = match.prefix().str() + match.suffix().str();
					next_array++;
					std::sregex_iterator tmp2 = next_array;
					if (tmp2 == end)
					{
						subject = restOfString;
						next_array = std::sregex_iterator(subject.begin(), subject.end(), array);
					}
				}

				/*update parentNode*/
				if (!(*subgrChildren == *(new JSONNode)))
				{
					parentNode->subgrChildrenVect.push_back(subgrChildren);
				}
				else
				{
					delete subgrChildren;
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
	out_data << "\n\nEND\n\n" << std::endl;
	return nullptr;
}

JSONNode* test::structureElementsInFile(std::ofstream &out_data, const std::string & subject)
{
	JSONNode *parsedNode = parseElementsInFile(out_data, subject); /*parse the subject*/
	std::map<std::string, std::string>::iterator itForKeyVal;
	std::map<std::string, std::string>::iterator itForKeyVal1;
	std::map<std::string, std::string>::iterator itForKeyVect;
	std::map<std::string, std::string>::iterator itForKeyVect1;
	std::vector<JSONNode*>::iterator itForChild;
	std::cout << "\t{";
	out_data << "\t{";
	/*----------organize the KeyVal structure to have a valide JSON format----------*/
	if (parsedNode->keyVal.size() > 0)
	{
		for (itForKeyVal = parsedNode->keyVal.begin(), itForKeyVal1 = parsedNode->keyVal.begin(); itForKeyVal1++ != parsedNode->keyVal.end(); itForKeyVal++)
		{
			if (itForKeyVal1 != parsedNode->keyVal.end())
			{
				std::cout << "\n\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
				out_data << "\n\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
			}
			else
			{
				std::cout << "\n\t" << itForKeyVal->first << ": " << itForKeyVal->second << std::endl;
				out_data << "\n\t" << itForKeyVal->first << ": " << itForKeyVal->second << std::endl;
				break;
			}
		}
	}

	/*----------organize the KeyVect structure to have a valide JSON format----------*/
	if (parsedNode->keyVect.size() > 0)
	{
		for (itForKeyVect = parsedNode->keyVect.begin(), itForKeyVect1 = parsedNode->keyVect.begin(); itForKeyVect1++ != parsedNode->keyVect.end(); itForKeyVect++)
		{
			if (itForKeyVect1 != parsedNode->keyVect.end())
			{
				std::cout << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << "\,";
				out_data << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << "\,";
			}
			else
			{
				std::cout << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << std::endl;
				out_data << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << std::endl;
				break;
			}
		}
	}

	/*----------organize the children structure  to have a valide JSON format------*/
	if (parsedNode->child.size() > 0)
	{
		int i = 0;

		/*organize the structure for the children besides the last child*/
		while (i < parsedNode->child.size() - 1)
		{
			std::cout << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
			out_data << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
			/*organize the keyVal structure from a child*/
			if (parsedNode->child[i]->keyVal.size() > 0)
			{
				std::cout << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
				out_data << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
				for (itForKeyVal = parsedNode->child[i]->keyVal.begin(), itForKeyVal1 = parsedNode->child[i]->keyVal.begin(); itForKeyVal1++ != parsedNode->child[i]->keyVal.end(); itForKeyVal++)
				{
					if (itForKeyVal1 != parsedNode->child[i]->keyVal.end())
					{
						std::cout << "\n\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
						out_data << "\n\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
					}
					else
					{
						std::cout << "\n\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << std::endl;
						out_data << "\n\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << std::endl;
						break;
					}
				}
				std::cout << "\t\t}," << std::endl;
				out_data << "\t\t}," << std::endl;
			}
			else

				/*organize the keyVect structure from a child*/
				if (parsedNode->child[i]->keyVect.size() > 0)
				{
					std::cout << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
					out_data << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
					for (itForKeyVect = parsedNode->child[i]->keyVect.begin(), itForKeyVect1 = parsedNode->child[i]->keyVect.begin(); itForKeyVect1++ != parsedNode->child[i]->keyVect.end(); itForKeyVect++)
					{
						if (itForKeyVect1 != parsedNode->child[i]->keyVect.end())
						{
							std::cout << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << "\,";
							out_data << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << "\,";
						}
						else
						{
							std::cout << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << std::endl;
							out_data << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << std::endl;
							break;
						}
					}
					std::cout << "\t\t}," << std::endl;
					out_data << "\t\t}," << std::endl;
				}

				else
				{
					break;
				}
			i++;
		}

		/*organize the structure for the last child*/
		if (i = parsedNode->child.size() - 1)
		{
			/*organize the keyVect structure from a keyVal*/
			if (parsedNode->child[i]->keyVal.size() > 0)
			{
				std::cout << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
				out_data << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
				for (itForKeyVal = parsedNode->child[i]->keyVal.begin(), itForKeyVal1 = parsedNode->child[i]->keyVal.begin(); itForKeyVal1++ != parsedNode->child[i]->keyVal.end(); itForKeyVal++)
				{
					if (itForKeyVal1 != parsedNode->child[i]->keyVal.end())
					{
						std::cout << "\n\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
						out_data << "\n\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
					}
					else
					{
						std::cout << "\n\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << std::endl;
						out_data << "\n\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << std::endl;
						break;
					}
				}
				std::cout << "\t\t}" << std::endl;
				out_data << "\t\t}" << std::endl;
			}
			else

				/*organize the keyVect structure from a keyVect*/
				if (parsedNode->child[i]->keyVect.size() > 0)
				{
					std::cout << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
					out_data << "\n\t" << parsedNode->child[i]->key << ":" << "\n\t\t{";
					for (itForKeyVect = parsedNode->child[i]->keyVect.begin(), itForKeyVect1 = parsedNode->child[i]->keyVect.begin(); itForKeyVect1++ != parsedNode->child[i]->keyVect.end(); itForKeyVect++)
					{
						if (itForKeyVect1 != parsedNode->child[i]->keyVect.end())
						{
							std::cout << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << "\,";
							out_data << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << "\,";
						}
						else
						{
							std::cout << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << std::endl;
							out_data << "\n\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << std::endl;
							break;
						}
					}
					std::cout << "\t\t}" << std::endl;
					out_data << "\t\t}" << std::endl;
				}

				else
				{
					/*break;*/
				}
		}
	}
	else{}
	std::cout << "\t" << "}" << std::endl;
	out_data << "\t" << "}" << std::endl;
	return parsedNode;
}

JSONNode* test::structureFilesinFile(std::ofstream &out_data, const std::string & subject)
{
	JSONNode *parsedNode = parseKeyListElements(subject);
	std::map<std::string, std::string>::iterator itForKeyVal;
	std::map<std::string, std::string>::iterator itForKeyVal1;
	std::map<std::string, std::string>::iterator itForKeyVect;
	std::map<std::string, std::string>::iterator itForKeyVect1;
	std::vector<JSONNode*>::iterator itForChild;
	out_data << "\t{";
	std::cout << "\t{";

	/*----------organize the subgroup children structure to have a valide JSON format----------*/
	if (parsedNode->subgrChildrenVect.size() > 0)
	{
		int i = 0;
		while (i < parsedNode->subgrChildrenVect.size() - 1)
		{
			out_data << "\n\t" << parsedNode->key << ":" << "\n\t\t[{";
			std::cout << "\n\t" << parsedNode->key << ":" << "\n\t\t[{";

			/*----------keyVal of subgroup----------*/
			if (parsedNode->subgrChildrenVect[i]->keyVal.size() > 0)
			{
				for (itForKeyVal = parsedNode->subgrChildrenVect[i]->keyVal.begin(), itForKeyVal1 = parsedNode->subgrChildrenVect[i]->keyVal.begin(); itForKeyVal1++ != parsedNode->subgrChildrenVect[i]->keyVal.end(); itForKeyVal++)
				{
					if (itForKeyVal1 != parsedNode->subgrChildrenVect[i]->keyVal.end())
					{
						out_data << "\n\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
						std::cout << "\n\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
					}
					else
					{
						out_data << "\n\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
						std::cout << "\n\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
						break;
					}
				}
			}
			else {}

			/*----------keyVect of subgroup----------*/
			if (parsedNode->subgrChildrenVect[i]->keyVect.size() > 0)
			{
				for (itForKeyVal = parsedNode->subgrChildrenVect[i]->keyVect.begin(), itForKeyVal1 = parsedNode->subgrChildrenVect[i]->keyVect.begin(); itForKeyVal1++ != parsedNode->subgrChildrenVect[i]->keyVect.end(); itForKeyVal++)
				{
					if (itForKeyVal1 != parsedNode->subgrChildrenVect[i]->keyVect.end())
					{
						out_data << "\n\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
						std::cout << "\n\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
					}
					else
					{
						out_data << "\n\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
						std::cout << "\n\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
						break;
					}
				}
			}
			else{}

			/*----------children of subgroup----------*/
			if (parsedNode->subgrChildrenVect[i]->child.size() > 0)
			{
				out_data << "\n\t\t\t" << parsedNode->subgrChildrenVect[i]->child[i]->key << ":" << "\n\t\t\t{";
				std::cout << "\n\t\t\t" << parsedNode->subgrChildrenVect[i]->child[i]->key << ":" << "\n\t\t\t{";

				for (itForKeyVect = parsedNode->subgrChildrenVect[i]->child[i]->keyVect.begin(), itForKeyVect1 = parsedNode->subgrChildrenVect[i]->child[i]->keyVect.begin(); itForKeyVect1++ != parsedNode->subgrChildrenVect[i]->child[i]->keyVect.end(); itForKeyVect++)
				{
					if (itForKeyVect1 != parsedNode->subgrChildrenVect[i]->child[i]->keyVect.end())
					{
						out_data << "\n\t\t\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << "\,";
						std::cout << "\n\t\t\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << "\,";
					}
					else
					{
						out_data << "\n\t\t\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << std::endl;
						std::cout << "\n\t\t\t\t" << itForKeyVect->first << ": " << itForKeyVect->second << std::endl;
						break;
					}
				}
				out_data << "\t\t\t}" << std::endl;
				std::cout << "\t\t\t}" << std::endl;

				out_data << "\t\t}," << std::endl;
				std::cout << "\t\t}," << std::endl;
			}
			i++;
		}

		/*----------Last element of the file----------*/
		if (i = parsedNode->subgrChildrenVect.size() - 1)
		{
			out_data << "\n\t\t{";
			std::cout << "\n\t\t{";
			for (itForKeyVal = parsedNode->subgrChildrenVect[i]->keyVal.begin(), itForKeyVal1 = parsedNode->subgrChildrenVect[i]->keyVal.begin(); itForKeyVal1++ != parsedNode->subgrChildrenVect[i]->keyVal.end(); itForKeyVal++)
			{
				if (itForKeyVal1 != parsedNode->subgrChildrenVect[i]->keyVal.end())
				{
					out_data << "\n\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
					std::cout << "\n\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
				}
				else
				{
					out_data << "\n\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
					std::cout << "\n\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
					break;
				}
			}
			if (parsedNode->subgrChildrenVect[i]->child[0]->keyVal.size() > 0)
			{
				out_data << "\n\t\t\t" << parsedNode->subgrChildrenVect[i]->child[0]->key << ":" << "\n\t\t\t{";
				std::cout << "\n\t\t\t" << parsedNode->subgrChildrenVect[i]->child[0]->key << ":" << "\n\t\t\t{";
				for (itForKeyVal = parsedNode->subgrChildrenVect[i]->child[0]->keyVal.begin(), itForKeyVal1 = parsedNode->subgrChildrenVect[i]->child[0]->keyVal.begin(); itForKeyVal1++ != parsedNode->subgrChildrenVect[i]->child[0]->keyVal.end(); itForKeyVal++)
				{
					if (itForKeyVal1 != parsedNode->subgrChildrenVect[i]->child[0]->keyVal.end())
					{
						out_data << "\n\t\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
						std::cout << "\n\t\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << "\,";
					}
					else
					{
						out_data << "\n\t\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << std::endl;
						std::cout << "\n\t\t\t\t" << itForKeyVal->first << ": " << itForKeyVal->second << std::endl;
						break;
					}
				}
				out_data << "\t\t\t}" << std::endl;
				std::cout << "\t\t\t}" << std::endl;
			}

			out_data << "\t\t}]" << std::endl;
			std::cout << "\t\t}]" << std::endl;
		}
	}
	out_data << "\t" << "}" << std::endl;
	std::cout << "\t" << "}" << std::endl;
	return parsedNode;
}

void test::indexElementsInFile(std::ofstream &out_data, JSONNode *node)
{
	int i = 1;
	std::map<std::string, std::string>::iterator itForKeyVal;
	if (!(node->keyVal.empty()))
	{
		for (itForKeyVal = node->keyVal.begin(); itForKeyVal != node->keyVal.end(); itForKeyVal++, i++)
		{
			out_data << "\t" << itForKeyVal->first << "[" << std::to_string(i) << "]:" << itForKeyVal->second << std::endl;
		}
	}
}

void test::mergeInFile(std::ofstream &out_data, JSONNode *node1, JSONNode *node2)
{
	/*----------merge the children with primitives of KeyVal/KeyVect-------------*/
	if (node1->child.size() == node2->child.size())
	{
		if (!(node1->child.empty()))
		{
			for (int i = 0; i < node1->child.size(); i++)
			{
				if (node1->child == node2->child)
				{
					removeKeyValDuplicates(node1->keyVal, node2->keyVal);
					removeKeyValDuplicates(node1->keyVect, node2->keyVect);
					removeChildDuplicates(node1->child, node2->child);
					std::cout << node1->child << std::endl;
					std::cout << "\nChildren are the same\n";
					out_data << node1->child << std::endl;
					out_data << "\nChildren are the same\n";

				}

				/*keyVal*/
				if (node1->child[i]->keyVal.size() == node2->child[i]->keyVal.size())
				{
					if (!(node1->child[i]->keyVal.empty()))
					{
						if (node1->child[i]->keyVal == node2->child[i]->keyVal)
						{
							removeKeyValDuplicates(node1->child[i]->keyVal, node2->child[i]->keyVal);
							std::cout << node1->child[i]->keyVal << std::endl;
							std::cout << "\nKeyVal are the same\n";
							out_data << node1->child[i]->keyVal << std::endl;
							out_data << "\nKeyVal are the same\n";
						}
						else
						{
							removeKeyValDuplicates(node1->child[i]->keyVal, node2->child[i]->keyVal);
							std::cout << node1->child[i]->keyVal << node2->child[i]->keyVal << std::endl;
							std::cout << "\nKeyVal has duplicates or different values\n";
							out_data << node1->child[i]->keyVal << node2->child[i]->keyVal << std::endl;
							out_data << "\nKeyVal has duplicates or different values\n";
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
					std::cout << "\nDifferent sizes of keyVal\n";
					out_data << "\nDifferent sizes of keyVal\n";
				}

				/*keyVect*/
				if (node1->child[i]->keyVect.size() == node2->child[i]->keyVect.size())
				{
					if (!(node1->child[i]->keyVect.empty()))
					{
						if (node1->child[i]->keyVect == node2->child[i]->keyVect)
						{
							removeKeyValDuplicates(node1->child[i]->keyVect, node2->child[i]->keyVect);
							std::cout << node1->child[i]->keyVect << std::endl;
							std::cout << "\keyVect are the same\n";
							out_data << node1->child[i]->keyVect << std::endl;
							out_data << "\keyVect are the same\n";
						}
						else
						{
							removeKeyValDuplicates(node1->child[i]->keyVect, node2->child[i]->keyVect);
							std::cout << node1->child[i]->keyVect << node2->child[i]->keyVect << std::endl;
							std::cout << "\keyVect has duplicates or different values\n";
							out_data << node1->child[i]->keyVect << node2->child[i]->keyVect << std::endl;
							out_data << "\keyVect has duplicates or different values\n";
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
					std::cout << "\nDifferent sizes of keyVal\keyVect \n";
					out_data << "\nDifferent sizes of keyVal\keyVect \n";
				}
			}
		}
		else
		{
			std::cout << "\nEMPTY keyVal\keyVect\n";
			out_data << "\nEMPTY keyVal\keyVect\n";
		}
	}
	else
	{
		std::cout << "\nDiferent size of children\n";
		out_data << "\nDiferent size of children\n";
	}

	/*----------merge the primitive KeyVal----------*/
	if (node1->keyVal.size() == node2->keyVal.size())
	{
		if (!(node1->keyVal.empty()))
		{
			if (node1->keyVal == node2->keyVal)
			{
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
			out_data << "\nEMPTY keyVal\n";
		}
	}
	else
	{
		std::cout << "\nDifferent sizes of keyVal\n";
		out_data << "\nDifferent sizes of keyVal\n";
	}

	//----------merge the primitive KeyVect
	if (node1->keyVect.size() == node2->keyVect.size())
	{
		if (!(node1->keyVect.empty()))
		{
			if (node1->keyVect == node2->keyVect)
			{
				std::cout << node1->keyVect << std::endl;
				std::cout << "\keyVect are the same\n";
				out_data << node1->keyVect << std::endl;
				out_data << "\keyVect are the same\n";
			}
			else
			{
				removeKeyValDuplicates(node1->keyVect, node2->keyVect);
				std::cout << node1->keyVect << node2->keyVect << std::endl;
				std::cout << "\keyVect has duplicates or different values\n";
				out_data << node1->keyVect << node2->keyVect << std::endl;
				out_data << "\keyVect has duplicates or different values\n";
			}
		}
		else
		{
			std::cout << "\nEMPTY keyVect\n";
			out_data << "\nEMPTY keyVect\n";
		}
	}
	else
	{
		std::cout << "\nDifferent sizes of keyVect\n";
		out_data << "\nDifferent sizes of keyVect\n";
	}
}

/*=======================================================================BOOL/OPERATORS============================================================================*/

bool test::operator==(const std::map<std::string, std::string> &kV1, const std::map<std::string, std::string> &kV2)
{
	if (kV1.size() == kV2.size())
	{
		/*bool aux = false;*/
		if (kV1.empty())
		{
			/*std::cout << "\nEmpty keyVal";*/
			return true;
		}
		else{ /*std::cout << "\nKeyVal found";*/
		}
		for (auto& pair : kV1)
		{
			auto found = kV2.find(pair.first);
			if (found != kV2.end())
			{
				/*return pair.second == found->second;*/
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
			/*std::cout << "\nEmpty keyChild";*/
			return true;
		}
		else{ /*std::cout << "\nKeyChild found";*/
		}
		for (auto& pair : kC1)
		{
			auto found = kC2.find(pair.first);
			if (found != kC2.end()){
				/*return pair.second == found->second;*/
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
			/*std::cout << "\nEmpty child";*/
			return true;
		}
		else
		{/*std::cout << "\nChild found"; */

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
	if (!(node1.keyVect == node2.keyVect)) { return false; }
	if (!(node1.child == node2.child)) { return false; }
	return true;
}

/*bool test::compare(JSONNode *node1, JSONNode *node2)
{//not updated
if (node1->child.size() == node2->child.size())
{
for (auto it = node1->child.begin(); it != node1->child.end(); ++it)
{
for (auto childIT = it->second.begin(); childIT != it->second.end(); ++childIT)
{
size_t index = std::distance(it->second.begin(), childIT);
for (auto& pair : node2->child)
compare(*childIT, *(pair.second.begin() + index));
}
}
if (node1->key == node2->key)
{
if (node1->child == node2->child)
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
}*/

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
		for (JSONNode *n : p.second) os << n->keyVect << ' ';
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
