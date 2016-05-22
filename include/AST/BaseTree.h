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

class ReturnNode;

class AbstractNode {
public:
	virtual void printNode() const = 0;
	virtual ~AbstractNode();
	virtual void findReturnStatement(std::vector<ReturnNode*>& vec);
};

using nodeList = std::vector<AbstractNode*>;

class BlockNode: public AbstractNode {
public:
	virtual void printNode() const;
	void addNode(AbstractNode* node);
	virtual void findReturnStatement(std::vector<ReturnNode*>& vec);
private:
	nodeList nodes;
};

class ExpressionNode: public AbstractNode {
public:
	virtual void printNode() const;
	ExpressionNode(Type type);
	ExpressionNode();
	Type getType() const;
	virtual std::string getName() const;
protected:
	virtual void printExpression() const;
	void initialize(const Type type);
	Type type;
};



}

#endif /* BASETREE_H_ */
