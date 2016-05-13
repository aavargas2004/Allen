/*
 * Scope.cpp
 *
 *  Created on: May 13, 2016
 *      Author: mitthy
 */
#include "Scope.h"
#include "Types.h"
#include "AST.h"
#include <string>

using namespace AST;
using namespace std;

variableInfo::_variableInfo() {
	this->defined = false;
	this->type = Type::TERROR;
}

ScopeNode::ScopeNode(ScopeNode* previous) : previous(previous){
}

void ScopeNode::addToScope(const std::string& name, const AST::Type& varType) {
	variableInfo varInfo;
	varInfo.type = varType;
	variableValMap[name] = varInfo;
}

bool ScopeNode::searchScope(const string& name, variableInfo& outputInfo) {
	decltype(variableValMap.find(name)) position;
	if((position = variableValMap.find(name)) == variableValMap.end()) {
		if(previous) {
			return previous->searchScope(name, outputInfo);
		}
		return false;
	}
	else {
		 outputInfo = position->second;
		 return true;
	}
}

ScopeNode* ScopeNode::previousNode() {
	return this->previous;
}


Scope::Scope() : head(new ScopeNode(nullptr)), tail(head){
}

Scope::~Scope() {
	//TODO
	ScopeNode* pr = tail->previousNode();
	while(tail->previousNode()) {
		delete tail;
		tail = pr;
		pr = tail->previousNode;
	}
}

void Scope::generateScope() {

}

variableInfo& Scope::searchScope(const std::string& varName) {

}

bool Scope::deleteScope() {

}

void Scope::addToScope(const std::string& name, const AST::Type& varType) {

}
