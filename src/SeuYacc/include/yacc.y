%type <id>primary_expression
%type <id>primary_expressionss <gg> qwq orz <qwq> qqqq
%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token <qwq>PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP < a_z >OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN <gg>ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER INLINE RESTRICT
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token BOOL COMPLEX IMAGINARY
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%start translation_unit
%union 
{
	int id;
	char* str;
};
%%

primary_expression
	: IDENTIFIER {$$ = $1 + $2}
	| CONSTANT
	| STRING_LITERAL
	| '(' expression ')'
	 {$$ = $1 + $3}
	;

postfix_expression
	: primary_expression{$$ = $1 - $2 }
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	{ $$ = $1 + $2 }
	| postfix_expression DEC_OP
{$$ = $1 + $4}	
| '(' type_name ')' '{' initializer_list '}'
	| '(' type_name ')' '{' initializer_list ',' '}'{$$ = $1 + $10}
	;


%%
#include <stdio.h>

extern char yytext[];
extern int column;

void yyerror(char const *s)
{
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);
}
