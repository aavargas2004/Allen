#include "AST/AST.h"
#include "Types/Types.h"
#include <iostream>
#include <stdio.h>
extern FILE * yyin;
using AST::ExprType;
using AST::Type;
using std::cout;
using std::cerr;
using std::endl;



extern int yyparse();
extern AST::BlockNode* root;

int main(int argc, char** argv) {
    if(argc == 1)
    	yyparse();
    else {
    	yyin = fopen(argv[1], "r");
    	yyparse();
    }
    root->printNode();
}
