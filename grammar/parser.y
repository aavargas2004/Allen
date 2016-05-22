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
%type<exprNode> expr functioncall functioncallstat
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
    auto funcInfo = funcScope->searchScope(std::string($1));
    if(funcInfo) {
        if(!funcInfo->defined) {
            std::cerr << "Erro. Funcao declarada mas nao definida " << std::endl;
            std::exit(-1);
        }
        if($3->size() != funcInfo->argInfo.size()) {
            std::cerr << "Erro. Tentando chamar funcao de "  << funcInfo->argInfo.size() << " argumentos, mas passou somente " << $3->size() << std::endl;
            std::exit(-1);
        }
        else {
            auto end = $3->end();
            auto indexBeg = funcInfo->argInfo.begin();
            for(auto beg = $3->begin(); beg != end; ++beg, ++ indexBeg) {
                auto requiredTypePtr = AST::ExprType::makeType((*indexBeg).type);
                auto actualTypePtr = AST::ExprType::makeType((*beg)->getType());
                if(!requiredTypePtr->compatible(actualTypePtr.get())) {
                    std::cerr << "Argumento inválido. Tipo não bate" << std::endl;
                    std::exit(-1);                
                }
                if((*beg)->getName() != "") {
                    auto var = scope->searchScope((*beg)->getName());
                    if((*indexBeg).isArray) {
                        if(var->isArray) {
                            if((*indexBeg).arrSize != var->arrSize) {
                                std::cerr << "Tamanho array não bate." << std::endl;
                                std::exit(-1);
                            }
                        }
                        else {
                            std::cerr << "Esperado array." << std::endl;
                            std::exit(-1);
                        }
                    }
                    else {
                        if(var->isArray) {
                            std::cerr << "Esperado expressao. Passado array." << std::endl;
                            std::exit(-1);
                        }
                    }
                }
                else {
                    if((*indexBeg).isArray) {
                        std::cerr << "Esperado array." << std::endl;
                        std::exit(-1);
                    }
                }
            }
            $$ = new AST::FunctionCallNode(std::string($1),funcInfo->type, $3);
            
        }
    }
    else {
        std::cerr << "Erro. Funcao nao declarada." << std::endl;
        std::exit(-1);
    }
}
;

callarglist:
callarglist T_COM expr
{
    $$ = $1;
    $$->push_back($3);
}
|
expr
{
    $$ = new std::vector<AST::ExpressionNode*>();
    $$->push_back($1);
}
;

declfunc:
T_DECL T_FUN tipo_base T_DECLVAR T_VARNAME createscope T_LPR arglist T_RPR destroyscope T_SMC
{
    if(funcScope->searchScope(std::string($5))) {
        std::cerr << "Erro. Redeclaracao de funcao." << std::endl;
        std::exit(-1);
    }
    else {
        funcScope->addToScope(std::string($5), $3, *($8));
    }
    $$ = new AST::DeclareFunctionNode(std::string($5), $3, $8);
}
|
T_DECL T_FUN tipo_base T_DECLVAR T_VARNAME createscope T_LPR T_RPR destroyscope T_SMC
{
    if(funcScope->searchScope(std::string($5))) {
        std::cerr << "Erro. Redeclaracao de funcao." << std::endl;
        std::exit(-1);
    }
    else {
        funcScope->addToScope(std::string($5), $3, std::vector<AST::VariableNode*>());
    }
    $$ = new AST::DeclareFunctionNode(std::string($5), $3);
}
;

funct:
T_DEF T_FUN tipo_base T_DECLVAR T_VARNAME enterfunc createscope T_LPR arglist T_RPR funcbody destroyscope leavefunc T_END T_DEF
{
    decltype(funcScope->searchScope(std::string($5))) funcSymbol;    
    if(!(funcSymbol = funcScope->searchScope(std::string($5)))) {
        funcScope->addToScope(std::string($5), $3, *($9));
        funcSymbol = funcScope->searchScope(std::string($5));
    }
    else {
        if(funcSymbol->defined) {
            std::cerr << "Erro. Redefinicao de funcao." << std::endl;
            std::exit(-1);
        }
    }
    funcSymbol->defined = true;
    auto typeptr = AST::ExprType::makeType($3);
    std::vector<AST::ReturnNode*> returnVec;
    $11->findReturnStatement(returnVec);
    if(returnVec.empty()) {
        std::cerr << "Erro. Funcao nao tem retorno" << std::endl;
        std::exit(-1);
    }
    
    for(auto& retExpression : returnVec) {
        auto retExpressionType = AST::ExprType::makeType(retExpression->getType());
        if(!typeptr->compatible(retExpressionType.get())) {
            std::cerr << "Erro. Retorno de funcao nao bate com tipo." << std::endl;
            std::exit(-1);
        }
    }
    $$ = new AST::FunctionNode($3, std::string($5), $9, $11);
}
|
T_DEF T_FUN tipo_base T_DECLVAR T_VARNAME enterfunc createscope T_LPR T_RPR funcbody destroyscope leavefunc T_END T_DEF
{
    decltype(funcScope->searchScope(std::string($5))) funcSymbol;    
    if(!(funcSymbol = funcScope->searchScope(std::string($5)))) {
        funcScope->addToScope(std::string($5), $3,  std::vector<AST::VariableNode*>());
        funcSymbol = funcScope->searchScope(std::string($5));
    }
    else {
        if(funcSymbol->defined) {
            std::cerr << "Erro. Redefinicao de funcao." << std::endl;
            std::exit(-1);
        }
    }
    funcSymbol->defined = true;
    auto typeptr = AST::ExprType::makeType($3);
    std::vector<AST::ReturnNode*> returnVec;
    $10->findReturnStatement(returnVec);
    if(returnVec.empty()) {
        std::cerr << "Erro. Funcao nao tem retorno" << std::endl;
        std::exit(-1);
    }
    for(auto& retExpression : returnVec) {
        auto retExpressionType = AST::ExprType::makeType(retExpression->getType());
        if(!typeptr->compatible(retExpressionType.get())) {
            std::cerr << "Erro. Retorno de funcao nao bate com tipo." << std::endl;
            std::exit(-1);       
        }
    }
    $$ = new AST::FunctionNode($3, std::string($5), new std::vector<AST::VariableNode*>(), $10);
}
;

returnstat:
T_RETURN expr T_SMC
{
    if(!insideFunc) {
        std::cerr << "Calling return outside function";
        std::exit(-1);
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
            $$ = new AST::DefineVariableNode(new AST::VariableNode(std::string($1), variableSymbol->type), $3);
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
            $$ = new AST::DefineVariableNode(new AST::ArrayAccessNode(new AST::ArrayNode(std::string($1), variableSymbol->type, variableSymbol->arrSize), $3 ),$6);
            variableSymbol->defined = true;
            //Gerar código de atribuição variável 
        }
    }
    else
    {
        yyerror("semantico: variavel %s sem declaracao", $1);
        $$ = new AST::DefineVariableNode(new AST::ArrayAccessNode(new AST::ArrayNode(std::string($1), AST::TERROR, variableSymbol->arrSize), $3 ),$6);
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
    if(!(variableSymbol = scope->searchScope(std::string($1)))) {
        yyerror("semantico: variavel %s sem declaracao", $1);
    }
    else if(!variableSymbol->defined){
        yyerror("semantico: variavel %s nao inicializada", $1);
    }
    else if(variableSymbol->isArray) {
        //REMOVER??
        std::cerr << "Atribuindo valor arrayzal para alguma expressão. Não pode" << std::endl;
        std::exit(-1);
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
T_MINUS T_VARNAME T_LBR expr T_RBR
{
  decltype(scope->searchScope(std::string($2))) variableSymbol;
    if(!(variableSymbol = scope->searchScope(std::string($2)))) {
        std::cerr << "Variavel não declarada usada em expressão " << std::string($2) <<std::endl;
        std::exit(-1);
    }
    else if(!variableSymbol->defined){
        std::cerr << "Variavel não definida usada em expressão " << std::string($2) << std::endl;
        std::exit(-1);
    }
    else if(!variableSymbol->isArray) {
        std::cerr << "Tentando acessar índice de não array" << std::endl;
        std::exit(-1);
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
            yyerror("semantico: variavel %s sofrendo redefinicao.", name.c_str());
        }
        else {
            varVec->push_back(new AST::VariableNode(name, type));
            scope->addToScope(name, type, $1.isArray, $1.size);
        }
    }
    if($1.isArray) {
        $$ = new AST::DeclareVariableNode(varVec, $1.size);
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
    $$ = NULL;    
}
;

destroyscope:
{
    $$ = NULL;
    scope->deleteScope();
}
;


%%

