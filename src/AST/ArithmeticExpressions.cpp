/*
 * ArithmeticExpressions.cpp
 *
 *  Created on: May 17, 2016
 *      Author: mitthy
 */

#include "AST/ArithmeticExpressions.h"
#include "AST/BaseTree.h"
#include <iostream>
using namespace std;
using namespace AST;
void BinaryNode::printNode() const {
	cout <<"(";
	lhs->printNode();
	printOperation();
	rhs->printNode();
	cout <<")";
}

BinaryNode::BinaryNode(ExpressionNode* lhs_, ExpressionNode* rhs_) :
		lhs(lhs_), rhs(rhs_) {
	Type lt = lhs_->getType();
	Type rt = rhs_->getType();
	auto ptrLT = ExprType::makeType(lt);
	auto ptrRT = ExprType::makeType(rt);
	auto newType = ptrLT->getNewType(ptrRT.get());
	initialize(newType->getTypeCode());
}

PlusBinaryNode::PlusBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {
}

void PlusBinaryNode::printOperation() const {
	cout << "+";
}

MinusBinaryNode::MinusBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {
}

void MinusBinaryNode::printOperation() const {
	cout << "-";
}

MultiplicationBinaryNode::MultiplicationBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {
}
void MultiplicationBinaryNode::printOperation() const {
	cout << "*";
}

DivisionBinaryNode::DivisionBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {

}

void DivisionBinaryNode::printOperation() const {
	cout << "/";
}


UnaryNode::UnaryNode(ExpressionNode* node_) :
		node(node_) {
}

void UnaryNode::printNode() const {
	cout<<"(";
	printOperation();
	node->printNode();
	cout<<")";
}


