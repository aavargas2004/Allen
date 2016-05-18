/*
 * ValueNodes.h
 *
 *  Created on: May 17, 2016
 *      Author: mitthy
 */

#include "AST/ValueNodes.h"
#include "AST/BaseTree.h"
#include <iostream>
#include "Types/Types.h"

using namespace AST;
using namespace std;

IntegerNode::IntegerNode(int val) :
		value(val), ExpressionNode(Type::TINT) {
}

void IntegerNode::printNode() const {
	cout << value;
}

void RealNode::printNode() const {
	cout << value;
}
RealNode::RealNode(const float val) :
		value(val), ExpressionNode(AST::Type::TREAL) {

}

void BoolNode::printNode() const {
	if (value) {
		cout << " TRUE ";
	} else {
		cout << " FALSE ";
	}
}

BoolNode::BoolNode(const bool value) :
		value(value), ExpressionNode(TBOOL) {

}

VariableNode::VariableNode(const string& name_, const Type& type) :
		ExpressionNode(type), name(name_) {
}
//Change later to support other streams
void VariableNode::printNode() const {
	cout << name;
}

string VariableNode::getName() const {
	return name;
}
