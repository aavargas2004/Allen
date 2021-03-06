%{
    #include <stdio.h>
    #include <stdarg.h>
    #include <string>
    #include "Scope.h"
    #include <iostream>
    #include "AST/AST.h"
    #include <vector>
    #define YY_DECL int yylex()
    #include "parser.tab.h"
    extern int yylineno;
    void yyerror(const char* s, ...);
%}

%option noyywrap nodefault yylineno

digito      [0-9] //Expressão regular que dá match em qualquer número de 0 a 9.

%%

[ \t\n]     ; //Ignora espaços em branco.

do {
    return T_DO;
}
 
def {
    return T_DEF;
}

decl {
    return T_DECL;
}

fun {
    return T_FUN;
}

return {
    return T_RETURN;
}

if {
    return T_IF;
}

then {
    return T_THEN;
}

while {
    return T_WHILE;
}

else {
    return T_ELSE;
}

end {
    return T_END;
}

int {
    return T_TINT;
}
real {
    return T_TREAL;
}
bool {
    return T_TBOOL;
}
[a-z]+  {
    yylval.var_name = strdup(yytext); 
    return T_VARNAME;
}
[0-9]+ {
    yylval.val_int = atoi(yytext); 
    return T_INT;
}
[0-9]+"."[0-9]*|[0-9]*"."[0-9]+ {
    yylval.val_float = atof(yytext);
    return T_FLOAT;    
}
":=" {
    return T_ASSIGN;
}
":" {
    return T_DECLVAR;
}
"-" {
    return T_MINUS;
}
"+" {
    return T_PLUS;
}
"*" {
    return T_TIMES;
}
">" {
    return T_GRT;
}
"<" {
    return T_LSS;
}
";" {
    return T_SMC;
}
"(" {
    return T_LPR;
}
")" {
    return T_RPR;
}
"/" {
    return T_DIV;
}
"," {
    return T_COM;
}
TRUE {
    yylval.bool_val = true;
    return T_TRUE;
}
FALSE {
    yylval.bool_val = false;
    return T_FALSE;
}
"=" {
    return T_EQ;
}
"~=" {
    return T_DIF;
}
">=" {
    return T_GRTEQ;
}
"<=" {
    return T_LSSEQ;
}
AND {
    return T_AND;
}
OR {
    return T_OR;
}
"~" {
    return T_NOT;
}
"[" {
    return T_LBR;
}
"]" {
    return T_RBR;
}
. {
    yyerror("Erro lexico: simbolo desconhecido: %c", *yytext);
}

%%

void yyerror(const char *s, ...){
    va_list ap;
    va_start(ap, s);
    std::fprintf(stderr, "[Linha %d] Erro ", yylineno);
    std::vfprintf(stderr, s, ap);
    std::fprintf(stderr, "\n");
}
