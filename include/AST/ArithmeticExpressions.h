/*
 * ArithmeticExpressions.h
 *
 *  Created on: May 17, 2016
 *      Author: mitthy
 */

#ifndef ARITHMETICEXPRESSIONS_H_
#define ARITHMETICEXPRESSIONS_H_
#include "BaseTree.h"
namespace AST {

class BinaryNode: public ExpressionNode {
public:
	BinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printExpression() const;
	virtual void printOperation() const = 0;
private:
	ExpressionNode* lhs;
	ExpressionNode* rhs;
};

class PlusBinaryNode: public BinaryNode {
public:
	PlusBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class MinusBinaryNode: public BinaryNode {
public:
	MinusBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class MultiplicationBinaryNode: public BinaryNode {
public:
	MultiplicationBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class DivisionBinaryNode: public BinaryNode {
public:
	DivisionBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class UnaryNode: public ExpressionNode {
public:
	UnaryNode(ExpressionNode* node);
protected:
	virtual void printExpression() const;
	virtual void printOperation() const = 0;
private:
	ExpressionNode* node;
};

class UnaryMinusNode: public UnaryNode {
public:
	UnaryMinusNode(ExpressionNode* node);
protected:
	virtual void printOperation() const;
};

class ParenthesisNode : public ExpressionNode {
public:
	ParenthesisNode(ExpressionNode* expr);
protected:
	virtual void printExpression() const;
private:
	ExpressionNode* expression;
};

}

#endif /* ARITHMETICEXPRESSIONS_H_ */
