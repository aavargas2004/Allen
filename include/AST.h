#ifndef AST_H__
#define AST_H__
#include <string>
#include <memory>
namespace AST {

enum Type
{
    TINT, TREAL, TBOOL
};

class AbstractNode 
{
public:
    virtual void printNode() const = 0;
};

class VariableNode : public AbstractNode 
{
public:
    virtual void printNode() const;
    VariableNode(const std::string& name, const Type type);
    //Disables copy operation.
    VariableNode(const VariableNode& rhs) = delete;
    VariableNode& operator=(const VariableNode& rhs) = delete; 
private:
    std::string name;
    Type type;
};

class ExpressionNode : public AbstractNode 
{
public:
    virtual void printNode() = 0;
    ExpressionNode(Type type);
protected:
    Type type;
};

class IntegerNode : public ExpressionNode
{
public:
    virtual void printNode() const;
    IntegerNode(const int val);
    IntegerNode(const IntegerNode& rhs);
    IntegerNode& operator=(const IntegerNode& rhs);
private:
    int value;
};

class BinaryNode : public ExpressionNode
{
public:
    virtual void printNode() const;
    BinaryNode(const std::shared_ptr<ExpressionNode> lhs, const std::shared_ptr<ExpressionNode> rhs);
    BinaryNode(const BinaryNode& rhs) = delete;
    BinaryNode& operator=(const BinaryNode& rhs) = delete;
protected:
    virtual void printOperation() const = 0;    
private:
    std::shared_ptr<AbstractNode> lhs;
    std::shared_ptr<AbstractNode> rhs; 
}

class UnaryNode : public ExpressionNode
{
public:
    virtual void printNode() const;
    UnaryNode(const std::shared_ptr<ExpressionNode> node);
    UnaryNode(const UnaryNode& rhs) = delete;
    UnaryNode& operator=(const UnaryNode& rhs) = delete;
protected:
    virtual void printOperation() const = 0;
private:
    std::shared_ptr<AbstractNode> node;
};





}


#endif
