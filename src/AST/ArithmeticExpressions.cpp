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
extern void yyerror(const char* s, ...);
using namespace std;
using namespace AST;
void BinaryNode::printExpression() const {
	lhs->printNode();
	cout << " (";
	printOperation();
	if (getType() != TERROR) {
		auto typePtr = ExprType::makeType(getType());
		std::cout << typePtr->getTypeName();
	} else {
		std::cout << "inteira";
	}
	cout << ") ";
	rhs->printNode();
}

BinaryNode::BinaryNode(ExpressionNode* lhs_, ExpressionNode* rhs_) :
		lhs(lhs_), rhs(rhs_) {
	Type lt = lhs_->getType();
	Type rt = rhs_->getType();
	auto ptrLT = ExprType::makeType(lt);
	auto ptrRT = ExprType::makeType(rt);
	try {
		auto newType = ptrLT->getNewType(ptrRT.get());
		initialize(newType->getTypeCode());
	} catch (IncompatibleTypeException& e) {
		initialize(TERROR);
	}
}

PlusBinaryNode::PlusBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {
	auto typeVal = ExprType::makeType(getType());
	if (getType() != TINT && getType() != TREAL) {
		yyerror(
				"semantico: operacao soma espera inteiro ou real mas recebeu %s.",
				typeVal->getTypeNameMasculino().c_str());
		initialize(TERROR);
	}

}

void PlusBinaryNode::printOperation() const {
	cout << "soma ";
}

MinusBinaryNode::MinusBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {
	auto typeVal = ExprType::makeType(getType());
	if (getType() != TINT && getType() != TREAL) {
		yyerror(
				"semantico: operacao subtracao espera inteiro ou real mas recebeu %s.",
				typeVal->getTypeNameMasculino().c_str());
		initialize(TERROR);
	}
}

void MinusBinaryNode::printOperation() const {
	cout << "subtracao ";
}

MultiplicationBinaryNode::MultiplicationBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {
	auto typeVal = ExprType::makeType(getType());
	if (getType() != TINT && getType() != TREAL) {
		yyerror(
				"semantico: operacao multiplicacao espera inteiro ou real mas recebeu %s.",
				typeVal->getTypeNameMasculino().c_str());
		initialize(TERROR);
	}
}
void MultiplicationBinaryNode::printOperation() const {
	cout << "multiplicacao ";
}

DivisionBinaryNode::DivisionBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {

	auto typeVal = ExprType::makeType(getType());
	if (getType() != TINT && getType() != TREAL) {
		yyerror(
				"semantico: operacao divisao espera inteiro ou real mas recebeu %s.",
				typeVal->getTypeNameMasculino().c_str());
		initialize(TERROR);
	}
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
	cout << ") ";
	node->printNode();
}

UnaryMinusNode::UnaryMinusNode(ExpressionNode* node) :
		UnaryNode(node) {
	auto typeVal = ExprType::makeType(getType());
	if (getType() != TINT && getType() != TREAL) {
		yyerror(
				"semantico: operacao menos unario espera inteiro ou real mas recebeu %s.",
				typeVal->getTypeNameMasculino().c_str());
		initialize(TERROR);
	}
}

void UnaryMinusNode::printOperation() const {
	cout << "menos unario ";
	auto typeptr = ExprType::makeType(getType());
	if (getType() != TERROR) {
		cout << typeptr->getTypeNameMasculino();
	} else {
		std::cout << "inteiro";
	}

}

ParenthesisNode::ParenthesisNode(ExpressionNode* expr) :
		expression(expr) {
	initialize(expression->getType());
}

void ParenthesisNode::printExpression() const {
	cout << "(abre paretenses) ";
	expression->printNode();
	cout << "(fecha parenteses)";
}

