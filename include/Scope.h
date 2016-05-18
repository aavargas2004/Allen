/*
 * Scope.h
 *
 *  Created on: May 13, 2016
 *      Author: mitthy
 */

#ifndef SCOPE_H_
#define SCOPE_H_
#include <map>
#include <string>
#include "Types/Types.h"

namespace AST {

typedef struct _variableInfo {
	bool defined;
	AST::Type type;
	bool isArray;
	unsigned int arrSize;
	_variableInfo();
} variableInfo;

class ScopeNode {
public:
	ScopeNode(ScopeNode* previous);
	bool searchScope(const std::string& name, variableInfo** outputInfo);
	void addToScope(const std::string& name, const AST::Type& varType,
			const bool& isArray, const unsigned int& arrSize);
	ScopeNode* previousNode();
	ScopeNode(const ScopeNode& rhs) = delete;
	ScopeNode& operator=(const ScopeNode& rhs) = delete;
	~ScopeNode() = default;
private:
	ScopeNode* previous;
	std::map<std::string, variableInfo> variableValMap;
};

class Scope {
public:
	Scope();
	Scope(const Scope& rhs) = delete;
	Scope& operator=(const Scope& rhs) = delete;
	void addToScope(const std::string& name, const AST::Type& varType,
			const bool& isArray, const unsigned int& arrSize);
	~Scope();
	void generateScope();
	variableInfo* searchScope(const std::string& varName);
	bool deleteScope();
private:
	ScopeNode* globalScope;
	ScopeNode* tail;
};

}

#endif /* SCOPE_H_ */
