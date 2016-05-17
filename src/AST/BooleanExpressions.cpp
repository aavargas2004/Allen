/*
 * BooleanExpressions.h
 *
 *  Created on: May 17, 2016
 *      Author: mitthy
 */

#include <AST/BooleanExpressions.h>
#include <iostream>
using std::cout;
using namespace AST;

AndBinaryNode::AndBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {
}
void AndBinaryNode::printOperation() const {
	cout << " AND ";
}

OrBinaryNode::OrBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {
}

void OrBinaryNode::printOperation() const {
	cout << " OR ";
}

GreaterThanBinaryNode::GreaterThanBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {

}

void GreaterThanBinaryNode::printOperation() const {
	cout << ">";
}

GreaterOrEqualThanBinaryNode::GreaterOrEqualThanBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {

}

void GreaterOrEqualThanBinaryNode::printOperation() const {
	cout << ">=";
}

LessThanBinaryNode::LessThanBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {

}
void LessThanBinaryNode::printOperation() const {
	cout << "<";
}

LessOrEqualThanBinaryNode::LessOrEqualThanBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {

}
void LessOrEqualThanBinaryNode::printOperation() const {
	cout << "<=";
}

