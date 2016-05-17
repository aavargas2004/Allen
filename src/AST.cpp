#include "AST.h"
#include <iostream>
#include <utility>
#include "Types.h"
#include "Exceptions.h"
using std::cout;
using std::string;
using std::endl;
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

IntegerNode::IntegerNode(int val) :
		value(val), ExpressionNode(Type::TINT) {
}

void IntegerNode::printNode() const {
	cout << value;
}

void BinaryNode::printNode() const {
	lhs->printNode();
	printOperation();
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

AndBinaryNode::AndBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {
}
void AndBinaryNode::printOperation() const {
	cout << " AND ";
}

OrBinaryNode::OrBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {
}

void OrBinaryNode::printOperation() const {
	cout << " OR ";
}

GreaterThanBinaryNode::GreaterThanBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {

}

void GreaterThanBinaryNode::printOperation() const {
	cout << ">";
}

GreaterOrEqualThanBinaryNode::GreaterOrEqualThanBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {

}

void GreaterOrEqualThanBinaryNode::printOperation() const {
	cout << ">=";
}

LessThanBinaryNode::LessThanBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {

}
void LessThanBinaryNode::printOperation() const {
	cout << "<";
}

LessOrEqualThanBinaryNode::LessOrEqualThanBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {

}
void LessOrEqualThanBinaryNode::printOperation() const {
	cout << "<=";
}

void UnaryNode::printNode() const {
	printOperation();
	node->printNode();
}

UnaryNode::UnaryNode(ExpressionNode* node_) :
		node(node_) {
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

NotUnaryNode::NotUnaryNode(ExpressionNode* node) :
		UnaryNode(node) {

}

void NotUnaryNode::printOperation() const {
	cout << "~";
}

EqualBinaryNode::EqualBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) : BinaryNode(lhs, rhs) {

}

void EqualBinaryNode::printOperation() const {
	cout << "=";
}


DifferentBinaryNode::DifferentBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs) : BinaryNode(lhs, rhs){

}

void DifferentBinaryNode::printOperation() const {
	cout << "!=";
}
