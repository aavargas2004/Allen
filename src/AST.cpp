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
	}
}

VariableNode::VariableNode(const string& name_, const Type& type) :
		ExpressionNode(type), name(name_) {
}
//Change later to support other streams
void VariableNode::printNode() const {
	cout << name << endl;
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
	cout << value << endl;
}

void BinaryNode::printNode() const {
	lhs->printNode();
	printOperation();
	rhs->printNode();
}

BinaryNode::BinaryNode(ExpressionNode* lhs_,
		ExpressionNode* rhs_) :
		lhs(lhs_), rhs(rhs_) {
	Type lt = lhs_->getType();
	Type rt = rhs_->getType();
	auto ptrLT = ExprType::makeType(lt);
	auto ptrRT = ExprType::makeType(rt);
	auto newType = ptrLT->getNewType(ptrRT.get());
	initialize(newType->getTypeCode());
}

PlusBinaryNode::PlusBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {
}

void PlusBinaryNode::printOperation() const {
	cout << "+";
}

MinusBinaryNode::MinusBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {
}

void MinusBinaryNode::printOperation() const {
	cout << "-";
}

MultiplicationBinaryNode::MultiplicationBinaryNode(
		ExpressionNode* lhs, ExpressionNode* rhs) :
		BinaryNode(lhs, rhs) {
}
void MultiplicationBinaryNode::printOperation() const {
	cout << "*";
}

DivisionBinaryNode::DivisionBinaryNode(ExpressionNode* lhs,
		ExpressionNode* rhs) :
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

void UnaryNode::printNode() const {
	printOperation();
	node->printNode();
}

UnaryNode::UnaryNode(ExpressionNode* node_) :
		node(node_) {
}

