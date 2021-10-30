%{
#include <ctype.h>
#include"y.tab.h"
#include "stdio.h"
#include "string.h"
%}
 
 
%%
 
 
"+" {return OP_ADD;}
"-" {return OP_SUB;}
"*" {return OP_MUL;}
"/" {return OP_DIV;}
"%" {return OP_MOD;}
"(" {return LPAR;}
")" {return RPAR;}

 
 
[0-9]+       {yylval.integer = atoi(yytext); return INTEGER;}
 
 
[0-9]*\.[0-9]+    {sscanf(yytext,"%lf",&yylval.real);return DOUBLE;}
 
 
"0x"[0-9a-f]+ {sscanf (yytext,"%x",&yylval.integer); return (INTEGER); }
 
[ \t] {return yyLex();}
\r {return END;}
\n { return END;}
 
%%
 
int yywrap()
{
return 1;
}