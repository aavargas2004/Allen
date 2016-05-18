/*
 * ValueNodes.h
 *
 *  Created on: May 17, 2016
 *      Author: mitthy
 */

#ifndef VALUENODES_H_
#define VALUENODES_H_

#include "AST/BaseTree.h"

namespace AST {

class VariableNode: public ExpressionNode {
public:
	virtual void printNode() const;
	VariableNode(const std::string& name, const Type& type);
	//Disables copy operation.
	std::string getName() const;
	bool isArray();
	void setArray();
private:
	std::string name;
	bool array = false;
};

class IntegerNode: public ExpressionNode {
public:
	virtual void printNode() const;
	IntegerNode(const int val);
private:
	int value;
};

class RealNode: public ExpressionNode {
public:
	virtual void printNode() const;
	RealNode(const float val);
private:
	float value;
};

class BoolNode: public ExpressionNode {
public:
	virtual void printNode() const;
	BoolNode(const bool value);
private:
	bool value;
};

//TODO
//Implement this shit
class ArrayNode : public VariableNode {
public:
	ArrayNode(const std::string& varName, Type varType, unsigned int size);
	virtual void printNode() const;
private:
	unsigned int size;
};


}



#endif /* VALUENODES_H_ */
