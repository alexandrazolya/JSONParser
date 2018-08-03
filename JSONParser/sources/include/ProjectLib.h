#include <iostream>
#include <fstream>
#include<string>
#include<vector>
#include <map>
#include <regex>
#include <sstream>
#include<fstream>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#define PROJECTLIB_H

namespace test{
	enum node_type {
		PRIMITIVE,
		ARRAYS,
		CHILD,
		CHILD_ARRAY
	};

	class JSONNode
	{
		friend bool operator==(const JSONNode &node1, const JSONNode &node2);
		friend std::string operator+(const std::string &s1, const std::string &s2);
	public:
		JSONNode(std::map <std::string, std::vector<JSONNode*>> keyChild);
		JSONNode(const std::string &k, node_type t) : type(t){};
		JSONNode() :type(node_type::PRIMITIVE){}
		JSONNode(std::string name) :type(node_type::PRIMITIVE){}
		std::map<std::string, std::string> keyVal; // key : value
		std::map<std::string, std::string> keyVect;
	//	std::map <std::string, std::vector<JSONNode*>> keyVect;// key : [ value, value]
	//	std::map<std::string, std::string>subgrChildren;
		std::string key; // key
		//std::string value; //value of the key
		std::vector<JSONNode*> child; // [value, value]
		std::vector<JSONNode*> subgrChildrenVect;
	private:
		const node_type type;
		//const mergeOption option;

	};
	//=======================================================================FUNCTIONS===============================================================================

	JSONNode* parseElements(const std::string & elements, std::string printOffset = "");

	JSONNode* parseKeyListElements(const std::string & s, std::string printOffset = "");

	JSONNode* indexElements(JSONNode *node);

	JSONNode* structureElements(const std::string & subject);

	void deleteFunction(JSONNode *node);

	void openFiles(std::string& test1, std::string& test2);

	void removeKeyValDuplicates(std::map<std::string, std::string> &kV1, std::map<std::string, std::string> &kV2);

	void removeChildDuplicates(std::vector<JSONNode*> &C1, std::vector<JSONNode*> &C2);

	void mergeElements(JSONNode *node1, JSONNode *node2);

	template<typename Type>
	void GetValue(const std::regex &primitive, const std::regex &array_, const std::regex &children, const std::regex &child_array)
	{
		JSONNode root = parse_json(myfile);

		for (auto &node : root)
		{
			switch (node.GetType())
			{
			case node_type::PRIMITIVE:
				JSONPrimitiveNode &v = static_cast<JSONPrimitiveNode>(node_type);
				break;
			case node_type::ARRAYS:
				JSONArrayNode &a = static_cast<JSONArrayNode>(node_type);
				auto t = a[100].GetType();
				break;
			case node_type::CHILD:
				JSONChildrenNode &o = static_cast<JSONChildrenNode>(node_type);
				auto t = o.GetType();
				break;
			case node_type::CHILD_ARRAY:
				JSONChild_ArrayNode &c = static_cast<JSONChild_ArrayNode>(node_type);
				auto t = c.GetType();
				break;
			}
		}
	}
	//===============================================================FUNCTIONS THAT PRINT IN A FILE====================================================================

	JSONNode* parseElementsInFile(std::ofstream &out_data, const std::string & elements, std::string printOffset = "");

	JSONNode* parseKeyListElementsInFile(std::ofstream &out_data, const std::string & s, std::string printOffset = "");

	JSONNode* structureElementsInFile(std::ofstream &out_data, const std::string & subject);

	JSONNode* structureFilesinFile(std::ofstream &out_data, const std::string & subject);

	void mergeInFile(std::ofstream &out_data, JSONNode *node1, JSONNode *node2);

	void indexElementsInFile(std::ofstream &out_data, JSONNode *node);

	//=======================================================================BOOL/OPERATORS============================================================================

	bool operator==(const std::map<std::string, std::string> &kV1, const std::map<std::string, std::string> &kV2);

	bool operator==(const std::map <std::string, std::vector<JSONNode*>> &kC1, const std::map <std::string, std::vector<JSONNode*>> &kC2);

	bool operator==(const std::vector<JSONNode*> &ch1, const std::vector<JSONNode*> &ch2);

	bool operator==(const JSONNode &node1, const JSONNode &node2);

	bool compare(JSONNode *node1, JSONNode *node2);

	std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::string> &kV);

	std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::vector<JSONNode*>> &kC);

	std::ostream& operator<<(std::ostream& os, const std::vector<JSONNode*> &C);

	//std::string operator +(const std::string &, const std::string &);

}
