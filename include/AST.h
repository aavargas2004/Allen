#ifndef AST_H__
#define AST_H__
#include <string>
#include <memory>
#include <vector>
#include "Types.h"
using std::shared_ptr;
namespace AST {

enum Type;

class AbstractNode 
{
public:
    virtual void printNode() const = 0;
    virtual ~AbstractNode();
};

using nodeList = std::vector<std::shared_ptr<AbstractNode>>;


class BlockNode : public AbstractNode
{
public:
    BlockNode(const BlockNode& rhs) = delete;
    BlockNode& operator=(const BlockNode& rhs) = delete;
    virtual void printNode() const;
    void addNode(std::shared_ptr<AbstractNode> node);
private:
    nodeList nodes;
};

class ExpressionNode : public AbstractNode 
{
public:
    virtual void printNode() = 0;
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
    VariableNode(const VariableNode& rhs) = delete;
    VariableNode& operator=(const VariableNode& rhs) = delete;
    std::string getName() const; 
private:
    std::string name;
};

class IntegerNode : public ExpressionNode
{
public:
    virtual void printNode() const;
    IntegerNode(const int val);
    IntegerNode(const IntegerNode& rhs)=delete;
    IntegerNode& operator=(const IntegerNode& rhs)=delete;
private:
    int value;
};

class BinaryNode : public ExpressionNode
{
public:
    virtual void printNode() const;
    BinaryNode(std::shared_ptr<ExpressionNode> lhs, std::shared_ptr<ExpressionNode> rhs);
    BinaryNode(const BinaryNode& rhs) = delete;
    BinaryNode& operator=(const BinaryNode& rhs) = delete;
protected:
    virtual void printOperation() const = 0;    
private:
    std::shared_ptr<AbstractNode> lhs;
    std::shared_ptr<AbstractNode> rhs;
};

class PlusBinaryNode : public BinaryNode 
{
public:
    PlusBinaryNode(std::shared_ptr<ExpressionNode> lhs, std::shared_ptr<ExpressionNode> rhs);
    PlusBinaryNode(const PlusBinaryNode& rhs) = delete;
    PlusBinaryNode& operator=(const PlusBinaryNode& rhs) = delete;
protected:
    virtual void printOperation() const;
};

class MinusBinaryNode : public BinaryNode
{
public:
    MinusBinaryNode(std::shared_ptr<ExpressionNode> lhs, std::shared_ptr<ExpressionNode> rhs);
    MinusBinaryNode(const MinusBinaryNode& rhs) = delete;
    MinusBinaryNode& operator=(const MinusBinaryNode& rhs) = delete;
protected:
    virtual void printOperation() const;
};


class MultiplicationBinaryNode : public BinaryNode
{
public:
	MultiplicationBinaryNode(std::shared_ptr<ExpressionNode> lhs, std::shared_ptr<ExpressionNode> rhs);
	MultiplicationBinaryNode(const MultiplicationBinaryNode& rhs) = delete;
	MultiplicationBinaryNode& operator=(const MultiplicationBinaryNode& rhs) = delete;
protected:
	virtual void printOperation() const;
};


class DivisionBinaryNode : public BinaryNode
{
public:
	DivisionBinaryNode(std::shared_ptr<ExpressionNode> lhs, std::shared_ptr<ExpressionNode> rhs);
	DivisionBinaryNode(const DivisionBinaryNode& rhs) = delete;
	DivisionBinaryNode& operator=(const DivisionBinaryNode& rhs) = delete;
protected:
	virtual void printOperation() const;
};

class AndBinaryNode : public BinaryNode
{
public:
	AndBinaryNode(std::shared_ptr<ExpressionNode> lhs, std::shared_ptr<ExpressionNode> rhs);
	AndBinaryNode(const AndBinaryNode& rhs) = delete;
	AndBinaryNode& operator=(const AndBinaryNode& rhs) = delete;
protected:
	virtual void printOperation() const;
};

class OrBinaryNode : public ExpressionNode
{
public:
	OrBinaryNode(std::shared_ptr<ExpressionNode> lhs, std::shared_ptr<ExpressionNode> rhs);
	OrBinaryNode(const OrBinaryNode& rhs) = delete;
	OrBinaryNode& operator=(const OrBinaryNode& rhs) = delete;
};


class UnaryNode : public ExpressionNode
{
public:
    virtual void printNode() const;
    UnaryNode(std::shared_ptr<ExpressionNode> node);
    UnaryNode(const UnaryNode& rhs) = delete;
    UnaryNode& operator=(const UnaryNode& rhs) = delete;
protected:
    virtual void printOperation() const = 0;
private:
    std::shared_ptr<AbstractNode> node;
};

}


#endif
