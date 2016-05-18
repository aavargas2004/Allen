%{
#include <iostream>
#include "AST/AST.h"
#include "Scope.h"
#include <cstdlib>
#include <vector>
extern int yylex();
extern int yyparse();
extern void yyerror(const char* s, ...);
AST::BlockNode* root;
AST::Scope* scope = new AST::Scope();
%}



%union {
    AST::AbstractNode* abstractNode;
    AST::BlockNode* nodeBlock;
    AST::Type type;
    AST::TypeArray type_arr;
    AST::ExpressionNode* exprNode;
    AST::BoolNode* boolNode;
    const char* var_name;
    std::vector<std::string>* varNameList;
    std::vector<AST::VariableNode*>* varList;
    int val_int;
    double val_float;
    bool bool_val;
}

%token T_TINT T_TREAL T_TBOOL T_VARNAME T_INT T_FLOAT T_ASSIGN T_DECLVAR T_MINUS T_PLUS T_TIMES T_GRT T_LSS T_SMC T_LPR T_RPR T_DIV T_COM 
%token T_TRUE T_FALSE T_EQ T_DIF T_GRTEQ T_LSSEQ T_AND T_OR T_NOT T_LBR T_RBR
%type<type> tipo_base
%type<type_arr> tipo
%type<varNameList> varlist
%type<varList> declarevar
%type<nodeBlock> head statements
%type<abstractNode> statement valattr
%type<var_name> T_VARNAME
%type<exprNode> expr
%type<val_int> T_INT
%type<val_float> T_FLOAT
%type<boolNode> bool
%left T_AND T_OR
%left T_NOT
%left T_GRT T_LSS T_EQ T_DIF T_GRTEQ T_LSSEQ 
%left T_MINUS T_PLUS
%left T_TIMES T_DIV
%left UNARY_MINUS
%start head
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
|
valattr
{
    $$ = $1;
}
;

valattr:
T_VARNAME T_ASSIGN expr T_SMC
{
    decltype(scope->searchScope(std::string($1))) variableSymbol;
    if((variableSymbol = scope->searchScope(std::string($1))))
    {
        if(variableSymbol->isArray) {
            std::cerr << "Tentando atribuir valor a array." << std::endl;
            std::exit(-1);
        }
        auto typeExpr = AST::ExprType::makeType($3->getType());
        auto typeVarSymbol = AST::ExprType::makeType(variableSymbol->type);
        if(!(typeVarSymbol->compatible(typeExpr.get()))) 
        {
            std::cerr << "Incompatible type: " << typeExpr->getTypeName() << " " << 
                typeVarSymbol->getTypeName() << std::endl;
            std::exit(-1);
        }
        else 
        {
            std::cout << "Atribuicao de valor para variavel " << typeVarSymbol->getTypeName()
                <<" " << $1 << " ";
            $3->printNode();            
            std::cout << std::endl;
            variableSymbol->defined = true;
            //Gerar código de atribuição variável 
        }
    }
    else
    {
        std::cerr << "Variável não declarada " << std::string($1) << std::endl;
        std::exit(-1);
    }
}
|
T_VARNAME T_LBR expr T_RBR T_ASSIGN expr T_SMC
{
    auto variableSymbol = scope->searchScope(std::string($1));
    if(variableSymbol)
    {
        if(!variableSymbol->isArray) {
            std::cerr << "Tentando atribuir índice de variável não array." << std::endl;
            std::exit(-1);
        }
        auto typeExpr = AST::ExprType::makeType($6->getType());
        auto typeVarSymbol = AST::ExprType::makeType(variableSymbol->type);
        if(!(typeVarSymbol->compatible(typeExpr.get()))) 
        {
            std::cerr << "Incompatible type: " << typeExpr->getTypeName() << " " << 
                typeVarSymbol->getTypeName() << std::endl;
            std::exit(-1);
        }
        if($3->getType() != AST::Type::TINT) {
            std::cerr << "Tentando acessar índice não inteiro no array. Burro" << std::endl;
            std::exit(-1);
        }
        else 
        {
            std::cout << "Atribuicao de valor para índice ";
            $3->printNode(); 
            std::cout << " do array " << $1 << " de tipo " << typeVarSymbol->getTypeName() <<" : ";
            $6->printNode();            
            std::cout << std::endl;
            variableSymbol->defined = true;
            //Gerar código de atribuição variável 
        }
    }
    else
    {
        std::cerr << "Variável não declarada " << std::string($1) << std::endl;
        std::exit(-1);
    }
}
;

expr:
expr T_MINUS expr
{
    $$ = new AST::MinusBinaryNode($1, $3);
}
|
expr T_TIMES expr
{
    $$ = new AST::MultiplicationBinaryNode($1, $3);
}
|
expr T_DIV expr
{
    $$ = new AST::DivisionBinaryNode($1, $3);
}
|
expr T_PLUS expr
{
    $$ = new AST::PlusBinaryNode($1, $3);
}
|
expr T_GRT expr
{
    $$ = new AST::GreaterThanBinaryNode($1, $3);
}
|
expr T_LSS expr 
{
    $$ = new AST::LessThanBinaryNode($1, $3);
}
|
T_LPR expr T_RPR
{
    $$ = $2;
}
|
expr T_EQ expr
{
    $$ = new AST::EqualBinaryNode($1, $3);
}
| 
expr T_DIF expr
{
    $$ = new AST::DifferentBinaryNode($1, $3);
}
|
expr T_GRTEQ expr
{
    $$ = new AST::GreaterOrEqualThanBinaryNode($1, $3);
}
|
expr T_LSSEQ expr
{
    $$ = new AST::LessOrEqualThanBinaryNode($1, $3);
}
|
expr T_AND expr
{
    $$ = new AST::AndBinaryNode($1, $3);
}
|
expr T_OR expr
{
    $$ = new AST::OrBinaryNode($1, $3);
}
|
T_NOT expr
{
    $$ = new AST::NotUnaryNode($2);
}
|
T_VARNAME
{
    decltype(scope->searchScope(std::string($1))) variableSymbol;
    if(!(variableSymbol = scope->searchScope(std::string($1)))) {
        std::cerr << "Variavel não declarada usada em expressão " << std::string($1) <<std::endl;
        std::exit(-1);
    }
    else if(!variableSymbol->defined){
        std::cerr << "Variavel não definida usada em expressão " << std::string($1) << std::endl;
        std::exit(-1);
    }
    else if(variableSymbol->isArray) {
        std::cerr << "Atribuindo valor arrayzal para alguma expressão. Não pode" << std::endl;
        std::exit(-1);
    }
    else {
        $$ = new AST::VariableNode(std::string($1), variableSymbol->type);
    }
}
|
T_VARNAME T_LBR expr T_RBR
{
    auto variableSymbol = scope->searchScope(std::string($1));
    if(!variableSymbol) {
        std::cerr << "Variavel não declarada." << std::endl;
        std::exit(-1);
    }
    else if (!variableSymbol->defined) {
        std::cerr << "Variavel não definida." << std::endl;
        std::exit(-1); 
    }
    else if (!variableSymbol->isArray) {
        std::cerr << "Tentando acessar o indice de array numa variavel que não é array." << std::endl;
        std::exit(-1);
    }
    else {
        $$ = new AST::ArrayNode(std::string($1), variableSymbol->type, variableSymbol->size);
    }
}
|
T_INT
{
    $$ = new AST::IntegerNode($1);
}
| 
T_MINUS T_INT %prec UNARY_MINUS
{
    $$ = new AST::UnaryMinusNode(new AST::IntegerNode($2));
}
| 
T_MINUS T_VARNAME %prec UNARY_MINUS
{
    //Add support to unary minus operation on variables.
      decltype(scope->searchScope(std::string($2))) variableSymbol;
    if(!(variableSymbol = scope->searchScope(std::string($2)))) {
        std::cerr << "Variavel não declarada usada em expressão " << std::string($2) <<std::endl;
        std::exit(-1);
    }
    else if(!variableSymbol->defined){
        std::cerr << "Variavel não definida usada em expressão " << std::string($2) << std::endl;
        std::exit(-1);
    }
    else if(variableSymbol->isArray) {
        std::cerr << "Atribuindo valor arrayzal para alguma expressão. Não pode" << std::endl;
        std::exit(-1);
    }
    else {
        $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), variableSymbol->type));
    }
}
|
T_MINUS T_FLOAT %prec UNARY_MINUS
{
    $$ = new AST::UnaryMinusNode(new AST::RealNode($2));
}
|
T_FLOAT
{
    $$ = new AST::RealNode($1);
}
|
bool
{
    $$ = $1;
}
;

bool:
T_TRUE
{
    $$ = new AST::BoolNode(true);
}
|
T_FALSE
{
    $$ = new AST::BoolNode(false);
}
;

declarevar: 
tipo T_DECLVAR varlist T_SMC
{
    AST::Type type = $1.type;
    std::vector<AST::VariableNode*>* varVec = new std::vector<AST::VariableNode*>();
    auto typeobjptr = AST::ExprType::makeType(type);
    std::cout << "Declaração de variável " << typeobjptr->getTypeName() << " ";
    std::string prefix = "";
    for(auto& name : *($3))
    {
    	std::cout << prefix << name;
        varVec->push_back(new AST::VariableNode(name, type));
        scope->addToScope(name, type, $1.isArray, $1.size);
        prefix = ", ";
    }
    std::cout << std::endl;
    $$ = varVec;
}
;

tipo:
tipo_base
{
    $$ = AST::TypeArray();
    $$.type = $1;
    $$.isArray = false;
    $$.size = 0;
    
}
|
tipo_base T_LBR T_INT T_RBR
{
    $$ = AST::TypeArray();
    $$.type = $1;
    $$.isArray = true;
    $$.size = $3;
}
;

tipo_base:
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

