% {
    #include <stdio.h>
    #define YY_DECL int yylex()
    #include "parser.tab.h"
    #include <string>
    #include <iostream>
    void printerr(std::string err);
%  }

digito [0-9] //Expressão regular que dá match em qualquer número de 0 a 9.

%%

[ \t\n]     ; //Ignora espaços em branco.
int         
{
    return T_TINT;
}
real        
{
    return T_TREAL;
}
bool 
{
    return T_TBOOL;
}
[a-z]+  
{
    yylval.var_name = strdup(yytext); 
    return T_VARNAME;
}
digito+ 
{
    yylval.val_int = atoi(yytext); 
    return T_INT;
}
digito+"."digito* | digito*"."digito+
{
    yylval.val_float = atof(yytext);
    return T_FLOAT;    
}
":="   
{
    return T_ASSIGN;
}
":"
{
    return T_DECLVAR;
}
"-"
{
    return T_MINUS;
}
"+"
{
    return T_PLUS;
}
"*"
{
    return T_TIMES;
}
">"
{
    return T_GRT;
}
"<"
{
    return T_LSS;
}
";"
{
    return T_SMC;
}
"("
{
    return T_LPR;
}
")"
{
    return T_RPR;
}
"/"
{
    return T_DIV;
}
","
{
    return T_COM;
}
TRUE
{
    yylval.bool_val = true;
    return T_TRUE;
}
FALSE
{
    yylval.bool_val = false;
    return T_FALSE;
}
"="
{
    return T_EQ;
}
"~="
{
    return T_DIF;
}
">="
{
    return T_GRTEQ;
}
"<="
{
    return T_LSSEQ;
}
AND
{
    return T_AND;
}
OR
{
    return T_OR;
}
"~"
{
    return T_NOT;
}
.
{
    printerr(std::string(strdup(yytext)));
}
%%

void printerr(std::string err) 
{
    std::cerr << "Erro lexico: simbolo desconhecido:" << err << "." << std::endl;
}
