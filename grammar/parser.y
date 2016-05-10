%{
#include <iostream>
#include "AST.h"
#include <vector>
extern int yylex();
extern int yyparse();
%}

%union {
    AST::AbstractNode* abstractNode;
    AST::BlockNode* nodeBlock;
    AST::Type type;
    std::string var_name;
    std::vector<std::string> varNameList;
    std::vector<AST::VariableNode*>> varList;
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
%%

head: 
statements
;

statements: 
statements statement
{
}
|
statement
{
    
} 
;

statement:
declarevar
{
    AbstractNode* node = new BlockNode();
    for(auto var : $1)
    {
        //Não aceita ponteiro puro. Mudar para shared_ptr nesse arquivo e dentro do AST, pois os ponteiros devem ser compartilhados 
        node->addNode(var);
    }
}
;

declarevar: 
tipo T_DECLVAR varlist
{
    AST::Type type = $1;
    std::vector<AST::VariableNode*> varVec;
    for(auto& name : $3)
    {
        varVec.push_back(new AST::VariableNode(name, type));
    }
    $0 = varVec;
}
;

%%
