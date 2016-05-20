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
	cout << "( ";
	lhs->printNode();
	cout << "(";
	printOperation();
	auto typePtr = ExprType::makeType(getType());
	std::cout << typePtr->getTypeName() << " ";
	cout << ")";
	rhs->printNode();
	cout << ")";
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
	cout << "soma ";
}

MinusBinaryNode::MinusBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {
}

void MinusBinaryNode::printOperation() const {
	cout << "subtracao ";
}

MultiplicationBinaryNode::MultiplicationBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {
}
void MultiplicationBinaryNode::printOperation() const {
	cout << "multiplicacao ";
}

DivisionBinaryNode::DivisionBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {

}

void DivisionBinaryNode::printOperation() const {
	cout << "divisao ";
}

UnaryNode::UnaryNode(ExpressionNode* node_) :
		node(node_) {
	initialize(node->getType());
}

void UnaryNode::printNode() const {
	cout << "( ";
	printOperation();
	auto typePtr = ExprType::makeType(getType());
	std::cout << typePtr->getTypeName() << " ";
	node->printNode();
	cout << ")";
}

UnaryMinusNode::UnaryMinusNode(ExpressionNode* node) :
		UnaryNode(node) {

}

void UnaryMinusNode::printOperation() const {
	cout << "menos unario ";
}

