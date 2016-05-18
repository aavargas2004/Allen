/*
 * BooleanExpressions.h
 *
 *  Created on: May 17, 2016
 *      Author: mitthy
 */

#ifndef BOOLEANEXPRESSIONS_H_
#define BOOLEANEXPRESSIONS_H_
#include "AST/BaseTree.h"

namespace AST {


class BooleanExpressionNode : public ExpressionNode {
public:
	BooleanExpressionNode();
	virtual void printNode() const = 0;
};

class BinaryBooleanExpressionNode : public BooleanExpressionNode {
public:
	BinaryBooleanExpressionNode(ExpressionNode* lhs, ExpressionNode* rhs);
	virtual void printNode() const;
protected:
	virtual void printOperation() const = 0;
private:
	ExpressionNode* lhs, * rhs;
};

class UnaryBooleanExpressionNode : public BooleanExpressionNode {
public:
	UnaryBooleanExpressionNode(ExpressionNode* node);
	virtual void printNode() const;
protected:
	virtual void printOperation() const = 0;
private:
	ExpressionNode* node;
};

class AndBinaryNode: public BinaryBooleanExpressionNode {
public:
	AndBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class GreaterThanBinaryNode : public BinaryBooleanExpressionNode {
public:
	GreaterThanBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class GreaterOrEqualThanBinaryNode : public BinaryBooleanExpressionNode {
public:
	GreaterOrEqualThanBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class LessThanBinaryNode : public BinaryBooleanExpressionNode {
public:
	LessThanBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class LessOrEqualThanBinaryNode : public BinaryBooleanExpressionNode {
public:
	LessOrEqualThanBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class EqualBinaryNode : public BinaryBooleanExpressionNode {
public:
	EqualBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class DifferentBinaryNode : public BinaryBooleanExpressionNode {
public:
	DifferentBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class OrBinaryNode: public BinaryBooleanExpressionNode {
public:
	OrBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class NotUnaryNode : public UnaryBooleanExpressionNode {
public:
	NotUnaryNode(ExpressionNode* node);
protected:
	virtual void printOperation() const;
};


}

#endif /* BOOLEANEXPRESSIONS_H_ */
