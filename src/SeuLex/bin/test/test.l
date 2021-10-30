D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			([Ee][+-]?{D}+)
P                       ([Pp][+-]?{D}+)
FS			(f|F|l|L)
IS                      ((u|U)|(u|U)?(l|L|ll|LL)|(l|L|ll|LL)(u|U))

%{
#include <stdio.h>
#include "y.tab.h"

void comment(void);
int check_type(void);
void count(void);
%}

%%
a{1,5} {}
b{1,3}"test"|d{1,1}ff {  }
"/*"			{ comment(); }
"//"[^\n]*              { /* consume //-comment */ }

{D}{0,9} {  orz = qwq ; return 0 ;}
FS|H|(LD){2,3} {}

%%


int main(){}