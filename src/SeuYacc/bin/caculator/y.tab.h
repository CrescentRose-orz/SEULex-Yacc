#ifndef Y_TAB_H
#define Y_TAB_H

typedef union
{
    long long   integer;   
    double  real;
}YYSTYPE;
 
 

// epsilon
#define epsilon -1
#define TNBound 139

// terminals
#define OP_MOD 128
#define OP_MUL 129
#define OP_SUB 130
#define OP_ADD 131
#define RPAR 132
#define OP_DIV 133
#define END 134
#define NL 135
#define INTEGER 136
#define LPAR 137
#define DOUBLE 138

// nonterminals
#define BSexprreal 139
#define ASexprreal 140
#define MDexprreal 141
#define exprreal 142
#define start 143
#define MDexprint 144
#define expression 145
#define ASexprint 146
#define exprint 147
#define BSexprint 148
#define acc 24

#define NTerminalBase 139
#define isTerminal(x) (x<139)
#ifdef ISCPP
#endif
extern char const *I2S[];
extern int _GOTO[55][10];
extern int ACTION[55][139];
extern int proCnt[30];
extern int proGet[30];
extern char yytext[1024];
#endif
