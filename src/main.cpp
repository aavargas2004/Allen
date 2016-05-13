#include "AST.h"
#include "Types.h"
#include <iostream>
using AST::ExprType;
using AST::Type;
using std::cout;
using std::cerr;
using std::endl;



extern int yyparse();
extern AST::BlockNode* root;

int main() {
    yyparse();
}
