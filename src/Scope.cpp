/*
 * Scope.cpp
 *
 *  Created on: May 13, 2016
 *      Author: mitthy
 */
#include "Scope.h"
#include "Types/Types.h"
#include "AST/AST.h"
#include <string>
#include <iostream>

using namespace AST;
using namespace std;

variableInfo::_variableInfo() {
	this->defined = false;
	this->type = Type::TERROR;
}

ScopeNode::ScopeNode(ScopeNode* previous) :
		previous(previous) {
}

void ScopeNode::addToScope(const std::string& name, const AST::Type& varType) {
	variableInfo varInfo;
	varInfo.type = varType;
	variableValMap[name] = varInfo;
}

bool ScopeNode::searchScope(const string& name, variableInfo** outputInfo) {
	decltype(variableValMap.find(name)) position;
	if ((position = variableValMap.find(name)) == variableValMap.end()) {
		if (previous) {
			return previous->searchScope(name, outputInfo);
		}
		return false;
	} else {
		*outputInfo = &position->second;

		return true;
	}
}

ScopeNode* ScopeNode::previousNode() {
	return this->previous;
}

Scope::Scope() :
		globalScope(new ScopeNode(nullptr)), tail(globalScope) {
}

Scope::~Scope() {
	while (deleteScope())
		;
	delete globalScope;
}

void Scope::generateScope() {

}

variableInfo* Scope::searchScope(const std::string& varName) {
	variableInfo* varInfo = nullptr;
	tail->searchScope(varName, &varInfo);
	return varInfo;
}

bool Scope::deleteScope() {
	if (tail != globalScope) {
		ScopeNode* temp = tail->previousNode();
		delete tail;
		tail = temp;
		return true;
	}
	return false;
}

void Scope::addToScope(const std::string& name, const AST::Type& varType) {
	tail->addToScope(name, varType);
}
