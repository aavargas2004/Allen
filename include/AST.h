#ifndef AST_H__
#define AST_H__
#include <string>
#include <memory>
#include <vector>
#include <exception>
namespace AST {


enum Type
{
    TINT, TREAL, TBOOL
};

class IncompatibleTypeException : public Exception
{
public:
    IncompatibleTypeException(Type tl, Type tr);
};

class AbstractNode 
{
public:
    virtual void printNode() const = 0;
};

using nodeList = std::vector<std::unique_ptr<AbstractNode>>;


class BlockNode : public AbstractNode
{
public:
    BlockNode(const BlockNode& rhs) = delete;
    BlockNode& operator=(const BlockNode& rhs) = delete;
    virtual void printNode() const;
    void addNode(std::unique_ptr<AbstractNode> node);
private:
    nodeList nodes;
};

class ExpressionNode : public AbstractNode 
{
public:
    virtual void printNode() = 0;
    ExpressionNode(Type type);
    Type getType() const;
protected:
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
    BinaryNode(std::unique_ptr<ExpressionNode> lhs, std::unique_ptr<ExpressionNode> rhs);
    BinaryNode(const BinaryNode& rhs) = delete;
    BinaryNode& operator=(const BinaryNode& rhs) = delete;
protected:
    virtual void printOperation() const = 0;    
private:
    std::unique_ptr<AbstractNode> lhs;
    std::unique_ptr<AbstractNode> rhs; 
};

class PlusBinaryNode : public BinaryNode 
{
public:
    PlusBinaryNode(std::unique_ptr<ExpressionNode> lhs, std::unique_ptr<ExpressionNode> rhs);
    PlusBinaryNode(const PlusBinaryNode& rhs) = delete;
    PlusBinaryNode& operator=(const PlusBinaryNode& rhs) = delete;
protected:
    virtual void printOperation() const;
};

class MinusBinaryNode : public BinaryNode
{
public:
    MinusBinaryNode(std::unique_ptr<ExpressionNode> lhs, std::unique_ptr<ExpressionNode> rhs);
    MinusBinaryNode(const MinusBinaryNode& rhs) = delete;
    MinusBinaryNode& operator=(const MinusBinaryNode& rhs) = delete;
protected:
    virtual void printOperation() const;
};


class UnaryNode : public ExpressionNode
{
public:
    virtual void printNode() const;
    UnaryNode(std::unique_ptr<ExpressionNode> node);
    UnaryNode(const UnaryNode& rhs) = delete;
    UnaryNode& operator=(const UnaryNode& rhs) = delete;
protected:
    virtual void printOperation() const = 0;
private:
    std::unique_ptr<AbstractNode> node;
};

}


#endif
