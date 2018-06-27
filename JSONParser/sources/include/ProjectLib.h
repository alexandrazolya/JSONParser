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

	class JSONNode {
		friend bool operator==(const JSONNode &node1, const JSONNode &node2);
	public:

		JSONNode(std::map <std::string, std::vector<JSONNode*>> keyChild);
		JSONNode(const std::string &k, node_type t) : type(t){};
		JSONNode() :type(node_type::PRIMITIVE){}
		JSONNode(std::string name) :type(node_type::PRIMITIVE){}
		std::map<std::string, std::string> keyVal; // key : value
		std::map <std::string, std::vector<JSONNode*>> keyChild;// key : [ value, value]
		std::string key; // key
		std::vector<JSONNode*> child; // [value, value]
		

	private:
		const node_type type;
		

	};
	//=======================================================================FUNCTIONS============================================================================

	JSONNode* parseElements(const std::string & elements, std::string printOffset = "");
	JSONNode* parseKeyListElements(const std::string & s, std::string printOffset = "");
	void deleteFunction(JSONNode *node);
	void openFiles(std::string& test1, std::string& test2);
	void removeKeyValDuplicates(std::map<std::string, std::string> &kV1, std::map<std::string, std::string> &kV2);
	void removeKeyChildDuplicates(std::map <std::string, std::vector<JSONNode*>> &kC1, std::map <std::string, std::vector<JSONNode*>> &kC2);
	void mergeFiles(JSONNode *node1, JSONNode *node2);


	


//=======================================================================BOOL/OPERATORS============================================================================
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

	bool operator==(const std::map<std::string, std::string> &kV1, const std::map<std::string, std::string> &kV2);

	bool operator==(const std::map <std::string, std::vector<JSONNode*>> &kC1, const std::map <std::string, std::vector<JSONNode*>> &kC2);

	bool operator==(const std::vector<JSONNode*> &ch1, const std::vector<JSONNode*> &ch2);

	bool operator==(const JSONNode &node1, const JSONNode &node2);

	bool compare(JSONNode *node1, JSONNode *node2);

	std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::string> &kV);

	std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::vector<JSONNode*>> &kC);


}
