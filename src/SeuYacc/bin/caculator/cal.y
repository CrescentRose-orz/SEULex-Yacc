%{
 
 
#include"stdio.h"
 
%}
 
 
%union
{
    long long   integer;   
    double  real;
};
 
 
%token NL LPAR RPAR END
%token <integer>INTEGER
%token <real>DOUBLE
%token OP_ADD OP_SUB OP_MUL OP_DIV OP_MOD
 
%type <real>exprreal
%type <real>MDexprreal
%type <real>BSexprreal <real> ASexprreal
%type <integer> exprint <integer> BSexprint
%type <integer> MDexprint
%type <integer> ASexprint
%start expression 

%%
 

expression :
	exprint  { printf("expression has int value %lld\n",$1); }
	| exprreal  { printf("expression has real value %lf\n",$1); }
	;
exprint : 
	MDexprint {$$ = $1;}
	;
MDexprint :
	MDexprint OP_MUL MDexprint  {$$ = $1*$3; }
	| MDexprint OP_DIV MDexprint  {$$ = $1/$3; }
	| MDexprint OP_MOD MDexprint {$$ = $1%$3; }
	| ASexprint {$$ = $1; }
	;
ASexprint :
	ASexprint OP_ADD ASexprint {$$ = $1 + $3;}
	| ASexprint OP_SUB ASexprint {$$ = $1 - $3;}
	| BSexprint {$$ = $1;}
	;

BSexprint :
	LPAR exprint RPAR {$$ = $2;}
	| INTEGER {$$ = $1;}
	;
exprreal :
	MDexprreal {$$ = $1; }
	;
MDexprreal :
	MDexprreal OP_MUL MDexprreal {$$ = $1*$3; }
	| MDexprreal OP_DIV MDexprreal {$$ = $1/$3; }
	| MDexprint OP_MUL MDexprreal {$$ = $1*$3; }
	| MDexprint OP_DIV MDexprreal {$$ = $1/$3; }
	| MDexprreal OP_MUL MDexprint {$$ = $1*$3; }
	| MDexprreal OP_DIV MDexprint {$$ = $1/$3; }
	| ASexprreal { $$ = $1; }	
	;
ASexprreal :
	ASexprreal OP_ADD ASexprreal {$$ = $1 + $3;}
	| ASexprreal OP_SUB ASexprreal {$$ = $1 - $3;}
	| ASexprint OP_ADD ASexprreal {$$ = $1 + $3;}
	| ASexprint OP_SUB ASexprreal {$$ = $1 - $3;}
	| ASexprreal OP_ADD ASexprint {$$ = $1 + $3;}
	| ASexprreal OP_SUB ASexprint {$$ = $1 - $3;}
	| BSexprreal { $$ = $1;}
	;
BSexprreal :
	DOUBLE {$$ = $1 ;}
	| LPAR exprreal RPAR { $$ = $2;}
 	;

%%

using namespace std;
int main(){
string fileName;
	cin >>fileName;
	yyin = fopen(fileName.c_str(),"r");
    yyParser parser;
    parser.yyParse();
	return 0;
}
