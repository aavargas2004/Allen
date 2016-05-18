/*
 * BooleanExpressions.h
 *
 *  Created on: May 17, 2016
 *      Author: mitthy
 */

#include "AST/BooleanExpressions.h"
#include "AST/BaseTree.h"
#include <iostream>
#include "Types/Types.h"
#include "Exceptions.h"
using std::cout;
using namespace AST;

BooleanExpressionNode::BooleanExpressionNode() :
		ExpressionNode(TBOOL) {

}

BinaryBooleanExpressionNode::BinaryBooleanExpressionNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BooleanExpressionNode(), lhs(lhs), rhs(rhs) {
	auto typeptrleft = ExprType::makeType(lhs->getType());
	auto typeptrright = ExprType::makeType(rhs->getType());
	if (!typeptrleft->compatible(typeptrright.get())) {
		throw IncompatibleTypeException(lhs->getType(), rhs->getType());
	}
}

void BinaryBooleanExpressionNode::printNode() const {
	lhs->printNode();
	printOperation();
	rhs->printNode();
}

UnaryBooleanExpressionNode::UnaryBooleanExpressionNode(ExpressionNode* node) :
		node(node) {

}

void UnaryBooleanExpressionNode::printNode() const {
	printOperation();
	node->printNode();
}

AndBinaryNode::AndBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {
	if ((lhs->getType() != Type::TBOOL) || (rhs->getType() != Type::TBOOL)) {
		if (lhs->getType() != Type::TBOOL) {
			throw InvalidType(lhs->getType());
		} else {
			throw InvalidType(rhs->getType());
		}
	}
}
void AndBinaryNode::printOperation() const {
	cout << " AND ";
}

OrBinaryNode::OrBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {
	if ((lhs->getType() != Type::TBOOL) || (rhs->getType() != Type::TBOOL)) {
		if (lhs->getType() != Type::TBOOL) {
			throw InvalidType(lhs->getType());
		} else {
			throw InvalidType(rhs->getType());
		}
	}
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

EqualBinaryNode::EqualBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {

}

void EqualBinaryNode::printOperation() const {
	cout << "=";
}

DifferentBinaryNode::DifferentBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {

}
void DifferentBinaryNode::printOperation() const {
	cout << "~=";
}

NotUnaryNode::NotUnaryNode(ExpressionNode* node) :
		UnaryBooleanExpressionNode(node) {
	if (node->getType() != Type::TBOOL) {
		throw InvalidType(node->getType());
	}
}

void NotUnaryNode::printOperation() const {
	cout << "~";
}

