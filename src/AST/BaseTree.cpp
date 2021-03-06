/*
 * BaseTree.cpp
 *
 *  Created on: May 17, 2016
 *      Author: mitthy
 */


#include "AST/BaseTree.h"
#include <iostream>

using namespace std;
using namespace AST;

AbstractNode::~AbstractNode() {
}

void BlockNode::addNode(AbstractNode* node) {
	nodes.push_back(node);
}

void BlockNode::printNode() const {
	for (auto& node : nodes) {
		node->printNode();
		cout << endl;
	}
}



ExpressionNode::ExpressionNode(Type type_) :
		type(type_) {
}

ExpressionNode::ExpressionNode() :
		type(TERROR) {
}

void ExpressionNode::initialize(const Type t_) {
	type = t_;
}

Type ExpressionNode::getType() const {
	return type;
}

void ExpressionNode::printNode() const {
	cout << "(";
	printExpression();
	cout << ")";
}

std::string ExpressionNode::getName() const {
	return "";
}

void ExpressionNode::printExpression() const {
	//EMPTY
}


void AbstractNode::findReturnStatement(std::vector<ReturnNode*>& vec) {

}

void BlockNode::findReturnStatement(std::vector<ReturnNode*>& vec) {
	for(auto& node : nodes) {
		if(node)
			node->findReturnStatement(vec);
	}
}
