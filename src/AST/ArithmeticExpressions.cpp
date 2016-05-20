/*
 * ArithmeticExpressions.cpp
 *
 *  Created on: May 17, 2016
 *      Author: mitthy
 */

#include "AST/ArithmeticExpressions.h"
#include "AST/BaseTree.h"
#include <iostream>
#include "Exceptions.h"
using namespace std;
using namespace AST;
void BinaryNode::printExpression() const {
	lhs->printNode();
	cout << "(";
	printOperation();
	auto typePtr = ExprType::makeType(getType());
	std::cout << typePtr->getTypeName();
	cout << ") ";
	rhs->printNode();
}

BinaryNode::BinaryNode(ExpressionNode* lhs_, ExpressionNode* rhs_) :
		lhs(lhs_), rhs(rhs_) {
	Type lt = lhs_->getType();
	Type rt = rhs_->getType();
	auto ptrLT = ExprType::makeType(lt);
	auto ptrRT = ExprType::makeType(rt);
	auto newType = ptrLT->getNewType(ptrRT.get());
	initialize(newType->getTypeCode());
	if (lhs->getType() == TBOOL || rhs->getType() == TBOOL) {
		throw InvalidType(TBOOL);
	}
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

void UnaryNode::printExpression() const {
	cout << "(";
	printOperation();
	auto typePtr = ExprType::makeType(getType());
	std::cout << typePtr->getTypeName() << " ";
	cout << ") ";
	node->printNode();
}

UnaryMinusNode::UnaryMinusNode(ExpressionNode* node) :
		UnaryNode(node) {

}

void UnaryMinusNode::printOperation() const {
	cout << "menos unario ";
}

ParenthesisNode::ParenthesisNode(ExpressionNode* expr) :
		expression(expr) {

}

void ParenthesisNode::printExpression() const {
	cout << "(abre paretenses) ";
	expression->printNode();
	cout << "(fecha parenteses)";
}

