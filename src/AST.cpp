#include "AST.h"
#include <iostream>
#include <utility>
#include "Types.h"
using std::cout;
using std::string;
using std::unique_ptr;
using std::endl;
using std::move;
using namespace AST;


void BlockNode::addNode(unique_ptr<AbstractNode> node) 
{
    nodes.push_back(move(node));
}

void BlockNode::printNode() const 
{
    for(auto& node : nodes) 
    {
        node->printNode();
    }
}

VariableNode::VariableNode(const string& name_, const Type& type) : ExpressionNode(type), name(name_)
{
}
//Change later to support other streams
void VariableNode::printNode() const
{
    cout << name << endl;
}

string VariableNode::getName() const 
{
    return name;
}


ExpressionNode::ExpressionNode(Type type_) : type(type_)
{
}

ExpressionNode::ExpressionNode(): type(TERROR)
{
}

void ExpressionNode::initialize(const Type t_)
{
    type = t_;
}

Type ExpressionNode::getType() const
{
    return type;
}

IntegerNode::IntegerNode(int val) : value(val), ExpressionNode(Type::TINT)
{
}

void IntegerNode::printNode() const 
{
    cout << value << endl;
}

void BinaryNode::printNode() const
{
    lhs->printNode();
    printOperation();
    rhs->printNode();
}

BinaryNode::BinaryNode(unique_ptr<ExpressionNode> lhs_, unique_ptr<ExpressionNode> rhs_) : lhs(move(lhs_)), rhs(move(rhs_))
{
    Type lt = lhs_->getType();
    Type rt = rhs_->getType();
    auto ptrLT = ExprType::makeType(lt);
    auto ptrRT = ExprType::makeType(rt);
    auto newType = ptrLT->getNewType(ptrRT.get());
    initialize(newType->getTypeCode());
}

PlusBinaryNode::PlusBinaryNode(unique_ptr<ExpressionNode> lhs, unique_ptr<ExpressionNode> rhs) : BinaryNode(move(lhs), move(rhs))
{
}

void PlusBinaryNode::printOperation() const
{
    cout << "+" << endl;    
}

MinusBinaryNode::MinusBinaryNode(unique_ptr<ExpressionNode> lhs, unique_ptr<ExpressionNode> rhs) : BinaryNode(move(lhs), move(rhs))
{
}

void MinusBinaryNode::printOperation() const
{
    cout << "-" << endl;    
}

void UnaryNode::printNode() const 
{
    printOperation();
    node->printNode();
}

UnaryNode::UnaryNode(unique_ptr<ExpressionNode> node_) : node(move(node_))
{
}


    
