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
	auto newType = typeptrleft->getNewType(typeptrright.get());
	comparisonType = newType->getTypeCode();
}

UnaryBooleanExpressionNode::UnaryBooleanExpressionNode(ExpressionNode* node) :
		node(node) {

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
	cout << "e booleano";
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
	cout << "ou booleano";
}

GreaterThanBinaryNode::GreaterThanBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {

}

void GreaterThanBinaryNode::printOperation() const {
	cout << "maior booleano";
}

GreaterOrEqualThanBinaryNode::GreaterOrEqualThanBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {

}

void GreaterOrEqualThanBinaryNode::printOperation() const {
	cout << "maior ou igual booleano";
}

LessThanBinaryNode::LessThanBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {

}
void LessThanBinaryNode::printOperation() const {
	cout << "menor booleano";
}

LessOrEqualThanBinaryNode::LessOrEqualThanBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {

}
void LessOrEqualThanBinaryNode::printOperation() const {
	cout << "menor ou igual booleano";
}

EqualBinaryNode::EqualBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {

}

void EqualBinaryNode::printOperation() const {
	cout << "igual booleano";
}

DifferentBinaryNode::DifferentBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryBooleanExpressionNode(lhs, rhs) {

}
void DifferentBinaryNode::printOperation() const {
	cout << "diferente booleano";
}

NotUnaryNode::NotUnaryNode(ExpressionNode* node) :
		UnaryBooleanExpressionNode(node) {
	if (node->getType() != Type::TBOOL) {
		throw InvalidType(node->getType());
	}
}

void NotUnaryNode::printOperation() const {
	cout << "nao booleano";
}

void BinaryBooleanExpressionNode::printExpression() const {
	lhs->printNode();
	if (lhs->getType() != comparisonType) {
		auto typeptr = ExprType::makeType(comparisonType);
		cout << " para " << typeptr->getTypeName() << " ";
	}
	cout << "(";
	printOperation();
	cout << ") ";
	rhs->printNode();
	if (rhs->getType() != comparisonType) {
		auto typeptr = ExprType::makeType(comparisonType);
		cout << " para " << typeptr->getTypeName();
	}
}

void UnaryBooleanExpressionNode::printExpression() const {
	cout << "(";
	printOperation();
	cout << ") ";
	node->printNode();
}

