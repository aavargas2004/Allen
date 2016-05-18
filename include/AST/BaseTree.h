/*
 * BaseTree.h
 *
 *  Created on: May 17, 2016
 *      Author: mitthy
 */

#ifndef BASETREE_H_
#define BASETREE_H_
#include "Types/Types.h"
#include <vector>

namespace AST {

class AbstractNode {
public:
	virtual void printNode() const = 0;
	virtual ~AbstractNode();
};

using nodeList = std::vector<AbstractNode*>;

class BlockNode: public AbstractNode {
public:
	virtual void printNode() const;
	void addNode(AbstractNode* node);
private:
	nodeList nodes;
};

class ExpressionNode: public AbstractNode {
public:
	virtual void printNode() const = 0;
	ExpressionNode(Type type);
	ExpressionNode();
	Type getType() const;
protected:
	void initialize(const Type type);
	Type type;
};



}

#endif /* BASETREE_H_ */
