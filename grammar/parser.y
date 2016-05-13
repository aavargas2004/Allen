%{
#include <iostream>
#include "AST.h"
#include <vector>
extern int yylex();
extern int yyparse();
extern void yyerror(const char* s, ...);
AST::BlockNode* root;
%}



%union {
    AST::AbstractNode* abstractNode;
    AST::BlockNode* nodeBlock;
    AST::Type type;
    const char* var_name;
    std::vector<std::string>* varNameList;
    std::vector<AST::VariableNode*>* varList;
    int val_int;
    double val_float;
    bool bool_val;
}

%token T_TINT T_TREAL T_TBOOL T_VARNAME T_INT T_FLOAT T_ASSIGN T_DECLVAR T_MINUS T_PLUS T_TIMES T_GRT T_LSS T_SMC T_LPR T_RPR T_DIV T_COM 
%token T_TRUE T_FALSE T_EQ T_DIF T_GRTEQ T_LSSEQ T_AND T_OR T_NOT
%type<type> tipo
%type<varNameList> varlist
%type<varList> declarevar
%type<nodeBlock> head statements
%type<abstractNode> statement
%type<var_name> T_VARNAME
%%

head: 
statements
{
    root = $1;
}
;

statements: 
statements statement
{
    $$ = $1;
    $$->addNode($2);
}
|
statement
{
    $$ = new AST::BlockNode();
    $$->addNode($1);    
} 
;

statement:
declarevar
{
    AST::BlockNode* node = new AST::BlockNode();
    for(auto var : *($1))
    { 
        node->addNode(var);
    }
    $$ = node;    
}
;

declarevar: 
tipo T_DECLVAR varlist T_SMC
{
    AST::Type type = $1;
    std::vector<AST::VariableNode*>* varVec = new std::vector<AST::VariableNode*>();
    auto typeobjptr = AST::ExprType::makeType(type);
    std::cout << "Declaração de variável " << typeobjptr->getTypeName() << " ";
    std::string prefix = "";
    for(auto& name : *($3))
    {
    	std::cout << prefix << name;
        varVec->push_back(new AST::VariableNode(name, type));
        prefix = ", ";
    }
    std::cout << std::endl;
    $$ = varVec;
}
;

tipo:
T_TINT
{
    $$ = AST::Type::TINT;
}
|
T_TREAL
{
    $$ = AST::Type::TREAL;
}
|
T_TBOOL
{
    $$ = AST::Type::TBOOL;
}
;

varlist:
varlist T_COM T_VARNAME
{
	$$->push_back(std::string($3));
}
|
T_VARNAME
{
	$$ = new std::vector<std::string>();
	$$->push_back(std::string($1));
}
;

%%
