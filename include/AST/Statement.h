/*
 * Statement.h
 *
 *  Created on: May 19, 2016
 *      Author: mitthy
 */

#ifndef STATEMENT_H_
#define STATEMENT_H_
#include <vector>
#include "AST/BaseTree.h"
#include "AST/ValueNodes.h"
#include "Types/Types.h"

namespace AST {


class DeclareVariableNode: public AbstractNode {
public:
	DeclareVariableNode(std::vector<VariableNode*>* declaredVar);
	DeclareVariableNode(std::vector<VariableNode*>* declaredVar, size_t arrSize);
	virtual void printNode() const;
private:
	std::vector<VariableNode*>* vec;
	size_t arrSize;
	bool isArray;
};

class IfNode: public AbstractNode {
public:
	IfNode(ExpressionNode* condition, BlockNode* thenNode,
			BlockNode* elseNode);
	IfNode(ExpressionNode* condition, BlockNode* then);
	virtual void printNode() const;
	virtual void findReturnStatement(std::vector<ReturnNode*>& vec);
private:
	ExpressionNode* condition;
	BlockNode* thenNode;
	BlockNode* elseNode;
};

class DefineVariableNode: public AbstractNode {
public:
	DefineVariableNode(VariableNode* var, ExpressionNode* value);
	virtual void printNode() const;
private:
	VariableNode* var;
	ExpressionNode* expr;
};

class WhileNode : public AbstractNode {
public:
	WhileNode(ExpressionNode* condition, BlockNode* statements);
	virtual void printNode() const;
	virtual void findReturnStatement(std::vector<ReturnNode*>& vec);
private:
	ExpressionNode* condition;
	BlockNode* statements;
};

class ReturnNode : public ExpressionNode {
public:
	ReturnNode(ExpressionNode* expr);
	virtual void printNode() const;
	virtual void findReturnStatement(std::vector<ReturnNode*>& vec);
private:
	ExpressionNode* returnExpression;
};

class DeclareFunctionNode : public ExpressionNode {
public:
	DeclareFunctionNode(std::string name, Type type, std::vector<VariableNode*>* args);
	DeclareFunctionNode(std::string name, Type type);
	virtual void printNode() const;
private:
	std::string functionName;
	std::vector<VariableNode*>* args;
};

}

#endif /* STATEMENT_H_ */
