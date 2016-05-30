/*
 * Statement.cpp
 *
 *  Created on: May 19, 2016
 *      Author: mitthy
 */

#include "AST/Statement.h"
#include <iostream>
#include <vector>
#include <string>
#include "Exceptions.h"
using namespace AST;
using namespace std;
DeclareVariableNode::DeclareVariableNode(vector<VariableNode*>* declaredVar) :
		vec(declaredVar), arrSize(0), isArray(false) {

}
DeclareVariableNode::DeclareVariableNode(vector<VariableNode*>* declaredVar,
		size_t arrSize) :
		vec(declaredVar), arrSize(arrSize), isArray(true) {

}
void DeclareVariableNode::printNode() const {
	if(vec->empty()) {
		return;
	}
	VariableNode* firstVar = (*vec)[0];
	auto typePtr = ExprType::makeType(firstVar->getType());
	cout << "Declaracao de ";
	if (isArray) {
		cout << "arranjo " << typePtr->getTypeNameMasculino() << " de tamanho "
				<< arrSize;
	} else {
		cout << "variavel " << typePtr->getTypeName();
	}
	cout << ":";
	string prefix = " ";
	for (auto& var : *(vec)) {
		cout << prefix << var->getName();
		prefix = ", ";
	}
}

IfNode::IfNode(ExpressionNode* condition, BlockNode* thenNode,
		BlockNode* elseNode) :
		condition(condition), thenNode(thenNode), elseNode(elseNode) {
	if (condition->getType() != TBOOL) {
		throw InvalidType(condition->getType());
	}
}

IfNode::IfNode(ExpressionNode* condition, BlockNode* then) :
		condition(condition), thenNode(then), elseNode(nullptr) {
	if (condition->getType() != TBOOL) {
		throw InvalidType(condition->getType());
	}
}

void IfNode::printNode() const {
	cout << "Expressao condicional" << endl;
	cout << "+se: ";
	condition->printNode();
	cout << endl << "+entao: " << endl;
	thenNode->printNode();
	if (elseNode) {

		cout << "+senao: " << endl;
		elseNode->printNode();
	}
	cout << "Fim expressao condicional";
}

DefineVariableNode::DefineVariableNode(VariableNode* var, ExpressionNode* value) :
		var(var), expr(value) {
	if (var->isArray()) {
		throw InvalidType(TERROR);
	}
}

void DefineVariableNode::printNode() const {
	cout << "Atribuicao de valor para ";
	var->printNode();
	cout << ": ";
	expr->printNode();
}

WhileNode::WhileNode(ExpressionNode* condition, BlockNode* statements) :
		condition(condition), statements(statements) {
	if (condition->getType() != TBOOL) {
		throw InvalidType(condition->getType());
	}
}

void WhileNode::printNode() const {
	cout << "Laco" << endl;
	cout << "+enquanto: ";
	condition->printNode();
	cout << endl;
	cout << "+faca:" << endl;
	statements->printNode();
	cout << "Fim laco";
}

ReturnNode::ReturnNode(ExpressionNode* expr) :
		returnExpression(expr) {
	initialize(expr->getType());
}

void ReturnNode::findReturnStatement(std::vector<ReturnNode*>& vec) {
	vec.push_back(this);
}

void ReturnNode::printNode() const {
	cout << "Retorno de funcao: ";
	returnExpression->printNode();
}

DeclareFunctionNode::DeclareFunctionNode(std::string name, Type type,
		std::vector<VariableNode*>* args): functionName(name), args(args) {
	initialize(type);
}

DeclareFunctionNode::DeclareFunctionNode(std::string name, Type type): functionName(name), args(nullptr) {
	initialize(type);
}

void DeclareFunctionNode::printNode() const {
	auto typeptr = ExprType::makeType(getType());
	cout << "Declaracao de funcao " << typeptr->getTypeName() << ": ";
	cout << functionName << endl;
	cout << "+parametros:" << endl;
	if(args) {
		for(auto& val: *(args)) {
			auto argTypePtr = ExprType::makeType(val->getType());
			cout << "Parametro " << argTypePtr->getTypeNameMasculino() << ": " << val->getName();
			cout << endl;
		}
	}
	cout << "Fim declaracao";
}

void IfNode::findReturnStatement(std::vector<ReturnNode*>& vec) {
	this->thenNode->findReturnStatement(vec);
	if(elseNode)
		this->elseNode->findReturnStatement(vec);
}

void WhileNode::findReturnStatement(std::vector<ReturnNode*>& vec) {
	this->statements->findReturnStatement(vec);
}
