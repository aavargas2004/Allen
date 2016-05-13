#ifndef AST_H__
#define AST_H__
#include <string>
#include <vector>
#include "Types.h"
namespace AST {

enum Type;

class AbstractNode 
{
public:
    virtual void printNode() const = 0;
    virtual ~AbstractNode();
};

using nodeList = std::vector<AbstractNode*>;


class BlockNode : public AbstractNode
{
public:
    virtual void printNode() const;
    void addNode(AbstractNode* node);
private:
    nodeList nodes;
};

class ExpressionNode : public AbstractNode 
{
public:
    virtual void printNode() const = 0;
    ExpressionNode(Type type);
    ExpressionNode();
    Type getType() const;
protected:
    void initialize(const Type type);
    Type type;
};

class VariableNode : public ExpressionNode 
{
public:
    virtual void printNode() const;
    VariableNode(const std::string& name, const Type& type);
    //Disables copy operation.
    std::string getName() const; 
private:
    std::string name;
};

class IntegerNode : public ExpressionNode
{
public:
    virtual void printNode() const;
    IntegerNode(const int val);
private:
    int value;
};


class BinaryNode : public ExpressionNode
{
public:
    virtual void printNode() const;
    BinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
    virtual void printOperation() const = 0;    
private:
    AbstractNode* lhs;
    AbstractNode* rhs;
};

class PlusBinaryNode : public BinaryNode 
{
public:
    PlusBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
    virtual void printOperation() const;
};

class MinusBinaryNode : public BinaryNode
{
public:
    MinusBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
    virtual void printOperation() const;
};


class MultiplicationBinaryNode : public BinaryNode
{
public:
	MultiplicationBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};


class DivisionBinaryNode : public BinaryNode
{
public:
	DivisionBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class AndBinaryNode : public BinaryNode
{
public:
	AndBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};

class OrBinaryNode : public BinaryNode
{
public:
	OrBinaryNode(ExpressionNode* lhs, ExpressionNode* rhs);
protected:
	virtual void printOperation() const;
};



class UnaryNode : public ExpressionNode
{
public:
    virtual void printNode() const;
    UnaryNode(ExpressionNode* node);
protected:
    virtual void printOperation() const = 0;
private:
    AbstractNode* node;
};

}


#endif
