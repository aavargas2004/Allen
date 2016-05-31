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
AST::FunctionScope* funcScope = new AST::FunctionScope();
bool insideFunc = false;
int scopeCount = 0;
%}



%union {
    AST::AbstractNode* abstractNode;
    AST::BlockNode* nodeBlock;
    AST::Type type;
    AST::TypeArray type_arr;
    AST::ExpressionNode* exprNode;
    AST::DeclareVariableNode* declvarNode;
    AST::DefineVariableNode* defVar;
    AST::BoolNode* boolNode;
    AST::IfNode* ifNode;
    AST::WhileNode* whileNode;
    AST::FunctionBody* funcbody;
    AST::FunctionNode* func;
    const char* var_name;
    std::vector<std::string>* varNameList;
    std::vector<AST::VariableNode*>* args;
    std::vector<AST::ExpressionNode*>* callArgs;
    int val_int;
    double val_float;
    bool bool_val;
}

%token T_TINT T_TREAL T_TBOOL T_VARNAME T_INT T_FLOAT T_ASSIGN T_DECLVAR T_MINUS T_PLUS T_TIMES T_GRT T_LSS T_SMC T_LPR T_RPR T_DIV T_COM 
%token T_TRUE T_FALSE T_EQ T_DIF T_GRTEQ T_LSSEQ T_AND T_OR T_NOT T_LBR T_RBR 
%token T_IF T_THEN T_ELSE T_END T_WHILE T_DO T_DEF T_DECL T_FUN T_RETURN
%type<type> tipo_base
%type<type_arr> tipo
%type<varNameList> varlist
%type<nodeBlock> head statements funcbody
%type<abstractNode> statement valattr createscope destroyscope declfunc returnstat
%type<declvarNode> declarevar
%type<var_name> T_VARNAME
%type<exprNode> expr functioncall functioncallstat arglistexpr
%type<val_int> T_INT
%type<val_float> T_FLOAT
%type<boolNode> bool
%type<callArgs> callarglist
%type<ifNode> conditional
%type<whileNode> whileloop
%type<func> funct
%type<args> arglist
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
    for(auto beg = funcScope->begin(), end = funcScope->end(); beg != end; ++beg) {
        auto funcTable = beg->second;
        auto funcName = beg->first;
        if(!funcTable.defined) {
            yyerror("semantico: funcao %s declarada mas nunca definida.", funcName.c_str());
        }    
    }
}
;

statements: 
statements statement
{
    $$ = $1;
    if($2 != nullptr)
        $$->addNode($2);
}
|
statement
{
    $$ = new AST::BlockNode();
    if($1 != nullptr)
        $$->addNode($1);    
}
;

statement:
declarevar
{
    $$ = $1;   
}
|
valattr
{
    $$ = $1;
}
|
conditional
{
    $$ = $1;
}
|
whileloop
{
    $$ = $1;
}
|
funct
{
    $$ = $1;
}
|
declfunc
{
    $$ = $1;
}
|
returnstat
{
    $$ = $1;
}
|
functioncallstat
{
    $$ = $1;
}
|
error T_SMC
{
    $$ = nullptr;
    yyerrok;
}
;

functioncallstat:
functioncall T_SMC
{
    $$ = $1;
}
;

functioncall:
T_VARNAME T_LPR callarglist T_RPR
{
    $$ = nullptr;
    auto funcInfo = funcScope->searchScope(std::string($1));
    if(funcInfo) {
        if($3->size() != funcInfo->argInfo.size()) {
            yyerror("semantico: funcao %s espera %i parametros mas recebeu %d.", $1, funcInfo->argInfo.size(), $3->size());
            $$ = new AST::FunctionCallNode(std::string($1),AST::TERROR, $3); 
        }
        else {
            auto end = $3->end();
            auto indexBeg = funcInfo->argInfo.begin();
            for(auto beg = $3->begin(); beg != end; ++beg, ++ indexBeg) {
                auto requiredTypePtr = AST::ExprType::makeType((*indexBeg).type);
                auto actualTypePtr = AST::ExprType::makeType((*beg)->getType());
                if(!((*indexBeg).type == (*beg)->getType())) {
                    yyerror("semantico: parametro espera %s mas recebeu %s.", requiredTypePtr->getTypeNameMasculino().c_str(), actualTypePtr->getTypeNameMasculino().c_str());
                    if($$ == nullptr)
                        $$ = new AST::FunctionCallNode(std::string($1),AST::TERROR, $3);
                }
                if((*beg)->getName() != "") {
                    auto var = scope->searchScope((*beg)->getName());
                    if((*indexBeg).isArray) {
                        if(var->isArray) {
                            if((*indexBeg).arrSize > var->arrSize) {
                                yyerror("semantico: arranjo %s possui tamanho menor do que necessario.", (*beg)->getName().c_str());
                                if($$ == nullptr) {
                                    $$ = new AST::FunctionCallNode(std::string($1),AST::TERROR, $3);      
                                }
                            }
                        }
                        else {
                            yyerror("semantico: variavel %s com uso como arranjo", (*beg)->getName().c_str());
                            if($$ == nullptr) {
                                $$ = new AST::FunctionCallNode(std::string($1),AST::TERROR, $3);      
                            }
                        }
                    }
                    else {
                        if(var->isArray) {
                            yyerror("semantico: arranjo %s com uso como variavel", (*beg)->getName().c_str());
                            if($$ == nullptr) {
                                $$ = new AST::FunctionCallNode(std::string($1),AST::TERROR, $3);      
                            }
                        }
                    }
                }
                else {
                    if((*indexBeg).isArray) {
                        yyerror("semantico: expressao usada como arranjo.");
                        if($$ == nullptr) {
                            $$ = new AST::FunctionCallNode(std::string($1),AST::TERROR, $3);      
                        }
                    }
                }
            }
            if($$ == nullptr)
                $$ = new AST::FunctionCallNode(std::string($1),funcInfo->type, $3);
            
        }
    }
    else {
        yyerror("semantico: funcao sem declaracao");
        if($$ == nullptr) {
            $$ = new AST::FunctionCallNode(std::string($1),AST::TERROR, $3);      
        }
    }
}
|
T_VARNAME T_LPR T_RPR
{
    $$ = nullptr;
    auto funcInfo = funcScope->searchScope(std::string($1));
    if(funcInfo) {
        if(funcInfo->argInfo.size()) {
            yyerror("semantico: funcao %s espera %i parametros mas recebeu %d.", $1, funcInfo->argInfo.size(), 0);
            
            $$ = new AST::FunctionCallNode(std::string($1),AST::TERROR, new std::vector<AST::ExpressionNode*>());
        }
        if($$ == nullptr) {
            $$ = new AST::FunctionCallNode(std::string($1),funcInfo->type, new std::vector<AST::ExpressionNode*>());
        }
    }
    else {
        yyerror("semantico: funcao sem declaracao");
        if($$ == nullptr) {
            $$ = new AST::FunctionCallNode(std::string($1),AST::TERROR, new std::vector<AST::ExpressionNode*>());           
        }
    }
}
;

callarglist:
callarglist T_COM arglistexpr
{
    $$ = $1;
    $$->push_back($3);
}
|
arglistexpr
{
    $$ = new std::vector<AST::ExpressionNode*>();
    $$->push_back($1);
}
;

arglistexpr:
arglistexpr T_MINUS arglistexpr
{
    $$ = new AST::MinusBinaryNode($1, $3);
}
|
arglistexpr T_TIMES arglistexpr
{
    $$ = new AST::MultiplicationBinaryNode($1, $3);
}
|
arglistexpr T_DIV arglistexpr
{
    $$ = new AST::DivisionBinaryNode($1, $3);
}
|
arglistexpr T_PLUS arglistexpr
{
    $$ = new AST::PlusBinaryNode($1, $3);
}
|
arglistexpr T_GRT arglistexpr
{
    $$ = new AST::GreaterThanBinaryNode($1, $3);
}
|
arglistexpr T_LSS arglistexpr 
{
    $$ = new AST::LessThanBinaryNode($1, $3);
}
|
T_LPR arglistexpr T_RPR
{
    $$ = new AST::ParenthesisNode($2);
}
|
arglistexpr T_EQ arglistexpr
{
    $$ = new AST::EqualBinaryNode($1, $3);
}
| 
arglistexpr T_DIF arglistexpr
{
    $$ = new AST::DifferentBinaryNode($1, $3);
}
|
arglistexpr T_GRTEQ arglistexpr
{
    $$ = new AST::GreaterOrEqualThanBinaryNode($1, $3);
}
|
arglistexpr T_LSSEQ arglistexpr
{
    $$ = new AST::LessOrEqualThanBinaryNode($1, $3);
}
|
arglistexpr T_AND arglistexpr
{
    $$ = new AST::AndBinaryNode($1, $3);
}
|
arglistexpr T_OR arglistexpr
{
    $$ = new AST::OrBinaryNode($1, $3);
}
|
T_NOT arglistexpr
{
    $$ = new AST::NotUnaryNode($2);
}
|
T_VARNAME
{
    decltype(scope->searchScope(std::string($1))) variableSymbol;
    if(funcScope->searchScope(std::string($1))) {
        yyerror("semantico: funcao %s com uso como variavel", $1);
        $$ = new AST::VariableNode(std::string($1), AST::TERROR);
    }
    else if(!(variableSymbol = scope->searchScope(std::string($1)))) {
        yyerror("semantico: variavel %s sem declaracao", $1);
        $$ = new AST::VariableNode(std::string($1), AST::TERROR);
    }
    else if(!variableSymbol->defined && !variableSymbol->isArray){
        if(!(insideFunc && variableSymbol->globalScope)) {
            yyerror("semantico: variavel %s nao inicializada", $1);
            $$ = new AST::VariableNode(std::string($1), AST::TERROR);
        }
        else {
            if(!variableSymbol->isArray) {
                $$ = new AST::VariableNode(std::string($1), variableSymbol->type);
            }
            else {
                $$ = new AST::ArrayNode(std::string($1), variableSymbol->type, variableSymbol->arrSize);
            }
        }
    }
    else {
        if(!variableSymbol->isArray) {
            $$ = new AST::VariableNode(std::string($1), variableSymbol->type);
        }
        else {
            $$ = new AST::ArrayNode(std::string($1), variableSymbol->type, variableSymbol->arrSize);
        }
    }
}
|
functioncall
{
    $$ = $1;
}
|
T_VARNAME T_LBR expr T_RBR
{
    auto variableSymbol = scope->searchScope(std::string($1));
    if(funcScope->searchScope(std::string($1))) {
        yyerror("semantico: funcao %s com uso como arranjo", $1);
        $$ = new AST::ArrayAccessNode( new AST::ArrayNode(std::string($1), AST::TERROR, variableSymbol->arrSize), $3);
    }
    else if(!variableSymbol) {
        yyerror("semantico: arranjo %s sem declaracao", $1);
        $$ = new AST::ArrayAccessNode( new AST::ArrayNode(std::string($1), AST::TERROR, variableSymbol->arrSize), $3);
    }
    else if (!variableSymbol->isArray) {
        yyerror("semantico: variavel %s com uso como arranjo", $1);
        $$ = new AST::ArrayAccessNode( new AST::ArrayNode(std::string($1), AST::TERROR, variableSymbol->arrSize), $3);
    }
    else {
        $$ = new AST::ArrayAccessNode( new AST::ArrayNode(std::string($1), variableSymbol->type, variableSymbol->arrSize), $3);
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
    if(funcScope->searchScope(std::string($2))) {
        yyerror("semantico: funcao %s com uso como variavel", $2);
        $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), AST::TERROR));
    }
    else if(!(variableSymbol = scope->searchScope(std::string($2)))) {
        yyerror("semantico: variavel %s sem declaracao", $2);
        $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), AST::TERROR));
    }
    else if(variableSymbol->isArray) {
        yyerror("semantico: arranjo %s com uso como variavel.", $2);
        $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), AST::TERROR));
    }
    else if(!variableSymbol->defined){
        if(!(insideFunc && variableSymbol->globalScope)) {
            yyerror("semantico: variavel %s nao inicializada", $2);
            $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), AST::TERROR));
        }
        else {
            $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), variableSymbol->type));
        }    
    }
    else {
        $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), variableSymbol->type));
    }
}
|
T_MINUS T_VARNAME T_LBR expr T_RBR
{
    decltype(scope->searchScope(std::string($2))) variableSymbol;
    if(funcScope->searchScope(std::string($2))) {
        yyerror("semantico: funcao %s com uso como arranjo", $2);
        $$ = new AST::UnaryMinusNode( new AST::ArrayAccessNode(new AST::ArrayNode(std::string($2), AST::TERROR, variableSymbol->arrSize), $4));
    }
    else if(!(variableSymbol = scope->searchScope(std::string($2)))) {
        yyerror("semantico: arranjo %s sem declaracao", $2);
        $$ = new AST::UnaryMinusNode( new AST::ArrayAccessNode(new AST::ArrayNode(std::string($2), AST::TERROR, variableSymbol->arrSize), $4));
    }
    else if(!variableSymbol->isArray) {
        yyerror("semantico: variavel %s com uso como arranjo.", $2);
        $$ = new AST::UnaryMinusNode( new AST::ArrayAccessNode(new AST::ArrayNode(std::string($2), AST::TERROR, variableSymbol->arrSize), $4));
    }
    else {
        $$ = new AST::UnaryMinusNode( new AST::ArrayAccessNode(new AST::ArrayNode(std::string($2), variableSymbol->type, variableSymbol->arrSize), $4));
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

declfunc:
T_DECL T_FUN tipo_base T_DECLVAR T_VARNAME createscope T_LPR arglist T_RPR destroyscope T_SMC
{
    if(!scopeCount) {
        if(funcScope->searchScope(std::string($5))) {
            yyerror("semantico: funcao %s sofrendo redeclaracao.", $5);
            $$ = new AST::DeclareFunctionNode(std::string($5), AST::TERROR, $8);
        }
        else {
            funcScope->addToScope(std::string($5), $3, *($8));
        }
        $$ = new AST::DeclareFunctionNode(std::string($5), $3, $8);
    }
    else {
        yyerror("semantico: funcao declarada fora do escopo global.");
        $$ = new AST::DeclareFunctionNode(std::string($5), AST::TERROR, $8);
    }
}
|
T_DECL T_FUN tipo_base T_DECLVAR T_VARNAME createscope T_LPR T_RPR destroyscope T_SMC
{
    if(!scopeCount) {
        if(funcScope->searchScope(std::string($5))) {
            yyerror("semantico: funcao %s sofrendo redeclaracao.", $5);
            $$ = new AST::DeclareFunctionNode(std::string($5), AST::TERROR);
        }
        else {
            funcScope->addToScope(std::string($5), $3, std::vector<AST::VariableNode*>());
        }
        $$ = new AST::DeclareFunctionNode(std::string($5), $3);
    }
    else {
        yyerror("semantico: funcao declarada fora do escopo global.");
        $$ = new AST::DeclareFunctionNode(std::string($5), AST::TERROR);
    }
}
;

funct:
T_DEF T_FUN tipo_base T_DECLVAR T_VARNAME enterfunc createscope T_LPR arglist T_RPR funcbody destroyscope leavefunc T_END T_DEF
{
    $$ = nullptr;
    if(!scopeCount) {
        bool define = true;
        decltype(funcScope->searchScope(std::string($5))) funcSymbol;    
        if(!(funcSymbol = funcScope->searchScope(std::string($5)))) {
            funcScope->addToScope(std::string($5), $3, *($9));
            funcSymbol = funcScope->searchScope(std::string($5));
        }
        else {
            
            if(funcSymbol->defined) {
                yyerror("semantico: funcao %s sofrendo redefinicao.", $5);
                if(!$$)
                    $$ = new AST::FunctionNode(AST::TERROR, std::string($5), $9, $11);
            }

            auto indexBeg = funcSymbol->argInfo.begin();
            auto beg = $9->begin();
            if(funcSymbol->argInfo.size() != $9->size()) {
                yyerror("semantico: funcao %s sofrendo redeclaracao.", $5);
                define = false;
                if(!$$)
                    $$ = new AST::FunctionNode(AST::TERROR, std::string($5), $9, $11);            
            }
            else {
                for(; beg != $9->end(); ++beg, ++indexBeg) {
                    if((*beg)->getType() != indexBeg->type) {
                        yyerror("semantico: funcao %s sofrendo redeclaracao.", $5);
                        define = false;
                        if(!$$)
                            $$ = new AST::FunctionNode(AST::TERROR, std::string($5), $9, $11);
                    }
                }
            }
        }
        if(define)
            funcSymbol->defined = true;
        auto typeptr = AST::ExprType::makeType($3);
        std::vector<AST::ReturnNode*> returnVec;
        $11->findReturnStatement(returnVec);
        if(returnVec.empty()) {
            yyerror("semantico: funcao nao possui retorno.");
            if(!$$)
                    $$ = new AST::FunctionNode(AST::TERROR, std::string($5), $9, $11);
        }
    
        for(auto& retExpression : returnVec) {
            auto retExpressionType = AST::ExprType::makeType(retExpression->getType());
            if(!typeptr->compatible(retExpressionType.get())) {
                yyerror("semantico: retorno de funcao nao bate com o tipo.");
                if(!$$)
                    $$ = new AST::FunctionNode(AST::TERROR, std::string($5), $9, $11);
            }
        }
        if(!$$)
            $$ = new AST::FunctionNode($3, std::string($5), $9, $11);
    }
    else {
        yyerror("semantico: funcao definida fora do escopo global.");
        if(!$$)
            $$ = new AST::FunctionNode(AST::TERROR, std::string($5), $9, $11);
    }
}
|
T_DEF T_FUN tipo_base T_DECLVAR T_VARNAME enterfunc createscope T_LPR T_RPR funcbody destroyscope leavefunc T_END T_DEF
{
    $$ = nullptr;
    if(!scopeCount) {
        decltype(funcScope->searchScope(std::string($5))) funcSymbol;
        bool define = true;    
        if(!(funcSymbol = funcScope->searchScope(std::string($5)))) {
            funcScope->addToScope(std::string($5), $3,  std::vector<AST::VariableNode*>());
            funcSymbol = funcScope->searchScope(std::string($5));
        }
        else {
            if(funcSymbol->defined) {
                yyerror("semantico: funcao %s sofrendo redefinicao.", $5);
                if(!$$)
                    $$ = new AST::FunctionNode(AST::TERROR, std::string($5), new std::vector<AST::VariableNode*>(), $10);
            }
            if(funcSymbol->argInfo.size() != 0) {
                yyerror("semantico: funcao %s sofrendo redeclaracao.", $5);
                define = false;
                if(!$$)
                    $$ = new AST::FunctionNode(AST::TERROR, std::string($5), new std::vector<AST::VariableNode*>(), $10);
            } 
        }
        if(define)
            funcSymbol->defined = true;
        auto typeptr = AST::ExprType::makeType($3);
        std::vector<AST::ReturnNode*> returnVec;
        $10->findReturnStatement(returnVec);
        if(returnVec.empty()) {
            yyerror("semantico: funcao nao possui retorno.");
            if(!$$)
                    $$ = new AST::FunctionNode(AST::TERROR, std::string($5), new std::vector<AST::VariableNode*>(), $10);
        }
        for(auto& retExpression : returnVec) {
            auto retExpressionType = AST::ExprType::makeType(retExpression->getType());
            if(!typeptr->compatible(retExpressionType.get())) {
                yyerror("semantico: retorno de funcao nao bate com o tipo.");
                if(!$$)
                    $$ = new AST::FunctionNode(AST::TERROR, std::string($5), new std::vector<AST::VariableNode*>(), $10);
            }
        }
        if(!$$) {
            $$ = new AST::FunctionNode($3, std::string($5), new std::vector<AST::VariableNode*>(), $10);
        }
    }
    else {
        yyerror("semantico: funcao definida fora do escopo global.");
        if(!$$)
            $$ = new AST::FunctionNode(AST::TERROR, std::string($5), new std::vector<AST::VariableNode*>(), $10);
    }
}
;

returnstat:
T_RETURN expr T_SMC
{
    if(!insideFunc) {
        yyerror("semantico: return sendo chamado fora de funcao");
    }
    $$ = new AST::ReturnNode($2);
}
;

enterfunc:
{
    insideFunc = true;
}
;

leavefunc:
{
    insideFunc = false;
}
;

funcbody:
statements
{
    $$ = $1;
}
;

arglist:
arglist T_COM tipo T_DECLVAR T_VARNAME
{
    AST::Type type = $3.type;
    scope->addToScope(std::string($5), type, $3.isArray, $3.size);
    if($3.isArray) {
        $$->push_back(new AST::ArrayNode(std::string($5), type, $3.size));
    }
    else {
        $$->push_back(new AST::VariableNode(std::string($5), type));
    }
    auto symbol = scope->searchScope(std::string($5));
    symbol->defined = true;
    
}
|
tipo T_DECLVAR T_VARNAME
{
    $$ = new std::vector<AST::VariableNode*>();
    AST::Type type = $1.type;
    scope->addToScope(std::string($3), type, $1.isArray, $1.size);
    if($1.isArray) {
        $$->push_back(new AST::ArrayNode(std::string($3), type, $1.size));
    }
    else {
        $$->push_back(new AST::VariableNode(std::string($3), type));
    }
    auto symbol = scope->searchScope(std::string($3));
    symbol->defined = true;
}
;

whileloop:
T_WHILE expr T_DO createscope statements destroyscope T_END T_WHILE 
{
    if($2->getType() != AST::TBOOL) {
        std::cerr << "Expressão não booleana no laço." << std::endl;
        std::exit(-1);
    }
    $$ = new AST::WhileNode($2, $5);
}
;

conditional: 
T_IF expr T_THEN createscope statements destroyscope T_END T_IF
{
    $$ = new AST::IfNode($2, $5);
}
|
T_IF expr T_THEN createscope statements destroyscope T_ELSE createscope statements destroyscope T_END T_IF
{
    $$ = new AST::IfNode($2, $5, $9);
}
;

valattr:
T_VARNAME T_ASSIGN expr T_SMC
{
    if(funcScope->searchScope(std::string($1))) {
        yyerror("semantico: funcao %s com uso como variavel.", $1);
        $$ = new AST::DefineVariableNode(new AST::VariableNode(std::string($1), AST::TERROR), $3);
    }
    else{
        decltype(scope->searchScope(std::string($1))) variableSymbol;
        if((variableSymbol = scope->searchScope(std::string($1))))
        {
            if(variableSymbol->isArray) {
                yyerror("semantico: arranjo %s com uso como variavel.", $1);
                $$ = new AST::DefineVariableNode(new AST::VariableNode(std::string($1), AST::TERROR), $3);
            }
            auto typeExpr = AST::ExprType::makeType($3->getType());
            auto typeVarSymbol = AST::ExprType::makeType(variableSymbol->type);
            if(!(typeVarSymbol->compatible(typeExpr.get()))) 
            {
                $$ = new AST::DefineVariableNode(new AST::VariableNode(std::string($1), AST::TERROR), $3);
                variableSymbol->defined = true;
            }
            else 
            {
                $$ = new AST::DefineVariableNode(new AST::VariableNode(std::string($1), variableSymbol->type), $3);
                variableSymbol->defined = true;
            }
        }
        else
        {
            yyerror("semantico: variavel %s sem declaracao", $1);
            $$ = new AST::DefineVariableNode(new AST::VariableNode(std::string($1), AST::TERROR), $3);
        }
    }
}
|
T_VARNAME T_LBR expr T_RBR T_ASSIGN expr T_SMC
{
    if(funcScope->searchScope(std::string($1))) {
        yyerror("semantico: funcao %s com uso como arranjo.", $1);
        $$ = new AST::DefineVariableNode(new AST::ArrayAccessNode(new AST::ArrayNode(std::string($1), AST::TERROR, 0), $3 ),$6);
    }
    else{
        auto variableSymbol = scope->searchScope(std::string($1));
        if(variableSymbol)
        {
        if(!variableSymbol->isArray) {
                yyerror("semantico: variavel %s com uso como arranjo", $1);
                $$ = new AST::DefineVariableNode(new AST::ArrayAccessNode(new AST::ArrayNode(std::string($1), AST::TERROR, variableSymbol->arrSize), $3 ),$6);
            }
            auto typeExpr = AST::ExprType::makeType($6->getType());
            auto typeVarSymbol = AST::ExprType::makeType(variableSymbol->type);
            if($3->getType() != AST::Type::TINT) {
                auto typeIndex = AST::ExprType::makeType($3->getType());
                std::string wrongTypeName = typeIndex->getTypeNameMasculino();
                yyerror("semantico: indice de tipo %s.", wrongTypeName.c_str()); 
                $$ = new AST::DefineVariableNode(new AST::ArrayAccessNode(new AST::ArrayNode(std::string($1), variableSymbol->type, variableSymbol->arrSize), $3 ),$6);
            }
            else 
            {
                $$ = new AST::DefineVariableNode(new AST::ArrayAccessNode(new AST::ArrayNode(std::string($1), variableSymbol->type, variableSymbol->arrSize), $3 ),$6);
            variableSymbol->defined = true;
            }
        }
        else
        {
            yyerror("semantico: arranjo %s sem declaracao", $1);
            $$ = new AST::DefineVariableNode(new AST::ArrayAccessNode(new AST::ArrayNode(std::string($1), AST::TERROR, variableSymbol->arrSize), $3 ),$6);
        }
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
    $$ = new AST::ParenthesisNode($2);
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
    if(funcScope->searchScope(std::string($1))) {
        yyerror("semantico: funcao %s com uso como variavel", $1);
        $$ = new AST::VariableNode(std::string($1), AST::TERROR);
    }
    else if(!(variableSymbol = scope->searchScope(std::string($1)))) {
        yyerror("semantico: variavel %s sem declaracao", $1);
        $$ = new AST::VariableNode(std::string($1), AST::TERROR);
    }
    else if(variableSymbol->isArray) {
        yyerror("semantico: arranjo %s com uso como variavel.", $1);
        $$ = new AST::VariableNode(std::string($1), AST::TERROR);
    }
    else if(!variableSymbol->defined){
        if(!(insideFunc && variableSymbol->globalScope)) {
            yyerror("semantico: variavel %s nao inicializada", $1);
            $$ = new AST::VariableNode(std::string($1), AST::TERROR);
        }
        else {
            $$ = new AST::VariableNode(std::string($1), variableSymbol->type);
        }
    }
    else {
        $$ = new AST::VariableNode(std::string($1), variableSymbol->type);
    }
}
|
functioncall
{
    $$ = $1;
}
|
T_VARNAME T_LBR expr T_RBR
{
    auto variableSymbol = scope->searchScope(std::string($1));
    if(funcScope->searchScope(std::string($1))) {
        yyerror("semantico: funcao %s com uso como arranjo", $1);
        $$ = new AST::ArrayAccessNode( new AST::ArrayNode(std::string($1), AST::TERROR, variableSymbol->arrSize), $3);
    }
    else if(!variableSymbol) {
        yyerror("semantico: arranjo %s sem declaracao", $1);
        $$ = new AST::ArrayAccessNode( new AST::ArrayNode(std::string($1), AST::TERROR, variableSymbol->arrSize), $3);
    }
    else if (!variableSymbol->isArray) {
        yyerror("semantico: variavel %s com uso como arranjo", $1);
        $$ = new AST::ArrayAccessNode( new AST::ArrayNode(std::string($1), AST::TERROR, variableSymbol->arrSize), $3);
    }
    else {
        $$ = new AST::ArrayAccessNode( new AST::ArrayNode(std::string($1), variableSymbol->type, variableSymbol->arrSize), $3);
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
    if(funcScope->searchScope(std::string($2))) {
        yyerror("semantico: funcao %s com uso como variavel", $2);
        $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), AST::TERROR));
    }
    else if(!(variableSymbol = scope->searchScope(std::string($2)))) {
        yyerror("semantico: variavel %s sem declaracao", $2);
        $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), AST::TERROR));
    }
    else if(variableSymbol->isArray) {
        yyerror("semantico: arranjo %s com uso como variavel.", $2);
        $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), AST::TERROR));
    }
    else if(!variableSymbol->defined){
        if(!(insideFunc && variableSymbol->globalScope)) {
            yyerror("semantico: variavel %s nao inicializada", $2);
            $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), AST::TERROR));
        }
        else {
            $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), variableSymbol->type));
        }
    }
    else {
        $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), variableSymbol->type));
    }
}
|
T_MINUS T_VARNAME T_LBR expr T_RBR
{
    decltype(scope->searchScope(std::string($2))) variableSymbol;
    if(funcScope->searchScope(std::string($2))) {
        yyerror("semantico: funcao %s com uso como arranjo", $2);
        $$ = new AST::UnaryMinusNode( new AST::VariableNode(std::string($2), AST::TERROR));
    }
    else if(!(variableSymbol = scope->searchScope(std::string($2)))) {
        yyerror("semantico: arranjo %s sem declaracao", $2);
        $$ = new AST::UnaryMinusNode( new AST::ArrayAccessNode(new AST::ArrayNode(std::string($2), AST::TERROR, variableSymbol->arrSize), $4));
    }
    else if(!variableSymbol->isArray) {
        yyerror("semantico: variavel %s com uso como arranjo.", $2);
        $$ = new AST::UnaryMinusNode( new AST::ArrayAccessNode(new AST::ArrayNode(std::string($2), AST::TERROR, variableSymbol->arrSize), $4));
    }
    else {
        $$ = new AST::UnaryMinusNode( new AST::ArrayAccessNode(new AST::ArrayNode(std::string($2), variableSymbol->type, variableSymbol->arrSize), $4));
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
    for(auto& name : *($3))
    {
        if(scope->searchCurrentScope(name)){
            if(!scope->searchCurrentScope(name)->isArray) {
                yyerror("semantico: variavel %s sofrendo redefinicao.", name.c_str());
            }
            else {
                yyerror("semantico: arranjo %s sofrendo redefinicao.", name.c_str());
            }
        }
        else if (funcScope->searchScope(std::string(name))) {
            yyerror("semantico: funcao %s com uso como variavel", name.c_str());
        }
        else {
            varVec->push_back(new AST::VariableNode(name, type));
            if(!$1.isArray) {
                scope->addToScope(name, type, $1.isArray, $1.size);
            }
            else {
                if($1.size < 1) {
                    yyerror("semantico: arranjo %s com tamanho menor do que um.", name.c_str());
                    scope->addToScope(name, type, $1.isArray, 1);
                }
                else {
                    scope->addToScope(name, type, $1.isArray, $1.size);
                }
            }
            if(!scopeCount) {
                auto tableVal = scope->searchScope(name);
                tableVal->globalScope = true;
            }
        }
    }
    if($1.isArray) {
        if($1.size < 1) {
            $$ = new AST::DeclareVariableNode(varVec, 1);
        }
        else {
            $$ = new AST::DeclareVariableNode(varVec, $1.size);
        }
    }
    else {
        $$ = new AST::DeclareVariableNode(varVec);
    }
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

createscope:
{
    scope->generateScope();
    ++scopeCount;
    $$ = NULL;    
}
;

destroyscope:
{
    $$ = NULL;
    scope->deleteScope();
    --scopeCount;
}
;


%%

