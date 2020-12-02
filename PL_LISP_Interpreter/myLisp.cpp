#include "lex.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <ctype.h>

enum NODE_TYPE { STR, NUM, LIST };

class Node {

private:

public:
	NODE_TYPE type;
	std::string value;
	std::list<std::shared_ptr<Node>> childNodeList;

	Node(NODE_TYPE type, std::string value = "")
	{
		this->type = type;
		this->value = value;
	}

	~Node()
	{
	}
};

typedef std::shared_ptr<Node> PNode;
std::map<std::string, PNode> NodeMap;


PNode func_add(std::list<PNode>& nodeList) {
	int a = atoi(nodeList.front()->value.c_str());
	std::list<PNode>::iterator iter = ++nodeList.begin();
	for (iter; iter != nodeList.end(); iter++)
		a += atoi((*iter)->value.c_str());
	std::shared_ptr<Node> NewNode(new Node(NODE_TYPE::NUM, std::to_string(a)));

	return NewNode;
}

PNode func_SETQ(std::list<PNode>& nodeList) {

	NodeMap[nodeList.front()->value] = nodeList.back();
	return nodeList.back();
}

PNode func_sub(std::list<PNode>& nodeList) {
	int a = atoi(nodeList.front()->value.c_str());
	std::list<PNode>::iterator iter = ++nodeList.begin();
	for (iter; iter != nodeList.end(); iter++)
		a -= atoi((*iter)->value.c_str());
	std::shared_ptr<Node> NewNode(new Node(NODE_TYPE::NUM, std::to_string(a)));
	return NewNode;
}

PNode func_mul(std::list<PNode>& nodeList) {
	int a = atoi(nodeList.front()->value.c_str());
	std::list<PNode>::iterator iter = ++nodeList.begin();
	for (iter; iter != nodeList.end(); iter++)
		a *= atoi((*iter)->value.c_str());
	std::shared_ptr<Node> NewNode(new Node(NODE_TYPE::NUM, std::to_string(a)));
	return NewNode;
}

PNode func_div(std::list<PNode>& nodeList) {
	int a = atoi(nodeList.front()->value.c_str());
	std::list<PNode>::iterator iter = ++nodeList.begin();
	for (iter; iter != nodeList.end(); iter++)
		a /= atoi((*iter)->value.c_str());
	std::shared_ptr<Node> NewNode(new Node(NODE_TYPE::NUM, std::to_string(a)));
	return NewNode;
}

PNode func_LIST(std::list<PNode>& nodeList) {
	std::shared_ptr<Node> NewNode(new Node(NODE_TYPE::LIST));
	std::list<PNode>::iterator iter;
	for (iter = nodeList.begin(); iter != nodeList.end(); iter++)
		NewNode->childNodeList.push_back(*iter);
	return NewNode;
}

PNode func_NTH(std::list<PNode>& nodeList)
{
	int num = atoi(nodeList.front()->value.c_str());
	int a = 0;
	std::list<PNode>::iterator iter = nodeList.back()->childNodeList.begin();
	while (a < num) {
		iter++;
		a++;
	}
	return *iter;
}

//bMark(') 가 있을때 true
//bMark(') 가 있으면 밑에 연산을 수행하지 않고 그냥 리턴함
PNode interpret(PNode node, bool bMark = false)
{
	std::string result = "";


	if (node->type == NODE_TYPE::NUM)
		return node;
	if (node->type == NODE_TYPE::STR)
	{
		std::map<std::string, PNode>::iterator mapiter = NodeMap.find(node->value);
		if (mapiter == NodeMap.end() || bMark)
			return node;
		else
			return mapiter->second;

	}
	if (node->type == NODE_TYPE::LIST && bMark)
	{
		return node;
	}

	std::list<PNode> elements;
	std::list<PNode>::iterator iter = ++node->childNodeList.begin();
	bool mark = false;

	// 여기까지 중간 리턴을 당하지 않고 왔다면 연산자가 붙어있는 List 코드임  ex (+ 1 2)
	// "+" 같은 연산자가 무조건 있기 때문에 elements에 1과 2만 넣어줌
	for (iter; iter != node->childNodeList.end(); iter++)
	{
		// mark가 있으면 그 다음에 (A B C) 같은 list가 나오기 때문에
		// 후에 연산하지 않고 그냥 리턴해버리도록 mark값 사용
		if ((*iter)->value == "'")
		{
			mark = true;
			continue;
		}
		elements.push_back(interpret(*iter, mark));
		if (mark) mark = !mark;
	}

	if (node->childNodeList.front()->value == "+")
	{
		node = func_add(elements);
	}
	else if (node->childNodeList.front()->value == "SETQ")
	{
		node = func_SETQ(elements);
	}
	else if (node->childNodeList.front()->value == "-")
	{
		node = func_sub(elements);
	}
	else if (node->childNodeList.front()->value == "*")
	{
		node = func_mul(elements);
	}
	else if (node->childNodeList.front()->value == "/")
	{
		node = func_div(elements);
	}

	else if (node->childNodeList.front()->value == "LIST")
	{
		node = func_LIST(elements);
	}
	else if (node->childNodeList.front()->value == "CAR")
	{
		node = elements.front()->childNodeList.front();
	}
	else if (node->childNodeList.front()->value == "CDR")
	{
		elements.front()->childNodeList.pop_front();
		node = elements.front();
	}
	else if (node->childNodeList.front()->value == "NTH")
	{
		node = func_NTH(elements);
	}

	return node;

}

PNode parse(Token*& token)
{
	PNode newNode;
	// list type
	if (token->type == LPAREN)
	{
		newNode = std::make_shared<Node>(NODE_TYPE::LIST);
		// recursively find the list.
		while ((++token)->type != RPAREN)
			newNode->childNodeList.push_back(parse(token));
	}
	// atom type
	else
	{
		NODE_TYPE node_type;
		std::string node_value;

		// positive integer
		if (token->type == INT)
		{
			node_type = NODE_TYPE::NUM;
			node_value = std::string(token->value);
		}
		// string
		else
		{
			node_type = NODE_TYPE::STR;
			node_value = std::string(token->value);

			// signed integer
			if ((token - 1)->type != LPAREN)
			{
				if (token->type == ADD_OP && (token + 1)->type == INT)
				{
					node_type = NODE_TYPE::NUM;
					node_value = std::string((token + 1)->value);
					token++;
				}
				else if (token->type == SUB_OP && (token + 1)->type == INT)
				{
					node_type = NODE_TYPE::NUM;
					node_value = "-" + std::string((token + 1)->value);
					token++;
				}
			}
		}
		newNode = std::make_shared<Node>(node_type, node_value);
	}

	return newNode;
}

void main()
{
	for (;;) {
		std::cout << "< ";
		std::string line;
		std::getline(std::cin, line);

		Token* token = lex((char*)line.c_str());
		Token* tokenRef = token;
		PNode parseTree = interpret(parse(tokenRef));
		delete[] token;

		if (parseTree->type == NODE_TYPE::LIST) {
			std::list<PNode>::iterator iter;
			std::cout << "(";
			for (iter = parseTree->childNodeList.begin(); iter != parseTree->childNodeList.end(); iter++)
				std::cout << (*iter)->value << " ";
			std::cout << ")";
			std::cout << std::endl;
		}
		else
			std::cout << parseTree->value << std::endl;
	}
}
