/*
 * ArithmeticExpressions.h
 *
 *  Created on: May 17, 2016
 *      Author: mitthy
 */

#ifndef ARITHMETICEXPRESSIONS_H_
#define ARITHMETICEXPRESSIONS_H_

namespace AST {

class BinaryNode: public ExpressionNode {
public:
	virtual void printNode() const;
	BinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const = 0;
private:
	AbstractNode* lhs;
	AbstractNode* rhs;
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
	virtual void printNode() const;
	UnaryNode(ExpressionNode* node);
protected:
	virtual void printOperation() const = 0;
private:
	AbstractNode* node;
};

}

#endif /* ARITHMETICEXPRESSIONS_H_ */
