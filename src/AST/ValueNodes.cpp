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
#include "Exceptions.h"
#include <vector>

using namespace AST;
using namespace std;

IntegerNode::IntegerNode(int val) :
		value(val), ExpressionNode(Type::TINT) {
}

void IntegerNode::printNode() const {
	cout << "valor inteiro " << value;
}

void RealNode::printNode() const {
	cout << "valor real " << value;
}
RealNode::RealNode(const float val) :
		value(val), ExpressionNode(AST::Type::TREAL) {

}

void BoolNode::printNode() const {
	if (value) {
		cout << "valor booleano TRUE ";
	} else {
		cout << "valor booleano FALSE ";
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

	cout << "variavel ";
	auto typePtr = ExprType::makeType(getType());
	cout << typePtr->getTypeName() << " " << getName();
}

VariableNode* VariableNode::at(int pos) {
	return nullptr;
}

string VariableNode::getName() const {
	return name;
}

ArrayNode::ArrayNode(const std::string& varName, Type varType,
		unsigned int size) :
		VariableNode(varName, varType), size(size), vars(
				vector<VariableNode*>(size)) {
	for (auto& var : vars) {
		var = new VariableNode(varName, varType);
	}
}

void ArrayNode::printNode() const {
	cout << "arranjo ";
	auto typePtr = ExprType::makeType(getType());
	cout << typePtr->getTypeNameMasculino() << " " << getName();
	cout << "[" << size << "]";
}

VariableNode* ArrayNode::at(int pos) {
	return vars[pos];
}

ArrayAccessNode::ArrayAccessNode(ArrayNode* array, ExpressionNode* accessExpr) :
		VariableNode(array->getName(), array->getType()), arr(array), index(
				accessExpr) {
}

void ArrayAccessNode::printNode() const {
	auto typeptr = ExprType::makeType(arr->getType());
	cout << "arranjo " << typeptr->getTypeNameMasculino() << " " << getName()
			<< "{+indice: ";
	index->printNode();
	cout << "}";
}

VariableNode* ArrayAccessNode::at(int pos) {
	return arr->at(pos);
}

void VariableNode::set(int pos, ExpressionNode* expr) {
	throw InvalidType(TERROR);
}

void VariableNode::set(ExpressionNode* expr) {
	//TODO
}

void ArrayNode::set(int pos, ExpressionNode* expr) {
	vars[pos]->set(expr);
}

void ArrayAccessNode::set(int pos, ExpressionNode* expr) {
	arr->set(pos, expr);
}

bool VariableNode::isArray() {
	return false;
}

bool ArrayAccessNode::isArray() {
	return arr->at(0)->isArray();
}

bool ArrayNode::isArray() {
	return true;
}

FunctionNode::FunctionNode(Type type, std::string name,
		std::vector<VariableNode*>* args, BlockNode* body) :
		ExpressionNode(type), name(name), args(args), body(body) {

}

void FunctionNode::printNode() const {
	auto typeptr = ExprType::makeType(getType());
	cout << "Definicao de funcao " << typeptr->getTypeName() << ": " << name;
	cout << endl;
	cout << "parametros:";
	cout << endl;
	for (auto& pamptr : *(args)) {
		auto pamType = ExprType::makeType(pamptr->getType());
		cout << "Parametro " << pamType->getTypeNameMasculino() << ": "
				<< pamptr->getName();
		cout << endl;
	}
	cout << "corpo:" << endl;
	body->printNode();
	cout << "Fim definicao";

}

std::string FunctionNode::getFuncName() {
	return name;
}

std::vector<VariableNode*>* FunctionNode::getArgs() {
	return args;
}

unsigned int VariableNode::arrSize() const {
	return 0;
}

unsigned int ArrayAccessNode::arrSize() const {
	return 0;
}

unsigned int ArrayNode::arrSize() const {
	return size;
}

FunctionCallNode::FunctionCallNode(std::string function, Type type,
		std::vector<ExpressionNode*>* args) :
		function(function), args(args) {
	initialize(type);
}

void FunctionCallNode::printNode() const {
	cout << "chamada de funcao ";
	auto typePtr = ExprType::makeType(getType());
	cout << typePtr->getTypeName() << " " << function << " {+parametros: ";
	std::string prefix = "";
	for(auto& node : *args) {
		cout << prefix;
		node->printNode();
		prefix = ", ";
	}
	cout << "}";
}

