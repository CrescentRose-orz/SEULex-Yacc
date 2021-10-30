#include<iostream>
#include<deque>
#include<stack>
#include<vector>
#include<exception>
#include<cstdlib>
#include<cstring>
#include"y.tab.h"

using std::stack;

const int initialStatus = 0;
YYSTYPE yyVal;
extern bool yyEOF;
int yyLex();
typedef union{
    int yyToken;
    int a;
    int b;
}YYSTYPE;


class LexerEOFException:public std::exception{
public:
	LexerEOFException():std::exception(){}
	LexerEOFException(std::string info){}
	const char* what(){
		return "meet ‘end of file’ Flag while parser try to read more tokens.";
	}
};

class yyParser{
private:
    int token,now;
    stack<YYSTYPE> valStack;
    stack<int> status;
    int read();
    void shift(int target);
    void reduce(int producer);
    int parse();
public:
    void yyParse(){
		int flag = 1;
        status.push(initialStatus);
        read();
        while(flag){
			if (0 < (flag = parse())){
				read();
			}
		}
    }
};

int yyParser::read(){
		memset(yyVal,0,sizeof(YYSTYPE));
		if (yyEOF){
			throw LexerEOFException();
		}
        token = yyLex();
		return 0;
    }
void yyParser::shift(int target){
		valStack.push(yyVal);
		now = target;
		status.push(now);
    }
int yyParser::parse(){
    int now = status.top();
    if (ACTION[now][token] > 0){
        // ACTION[now][lookAhead] > 0 , shift
		shift(ACTION[now][token]-1);
		return 1;
    } else if (ACTION[now][token] < 0){
        // ACTION[now][lookAhead] < 0 , reduce
		reduce(-ACTION[now][token]-1);
		return -1;
    } else {
		//ACTION[now][lookAhead] == 0,accept
        // by default,acc = 0
		return 0;
    }
}
void yyParser::reduce(int producer){
{
        int cnt = proCnt[producer]; 
        int newSymbol = proGet[producer];
        YYSTYPE *proVal = new YYSTYPE[cnt+1];
        for (int i = cnt; i > 0; --i){
			proVal[i] = valStack.top();
			valStack.pop();
        }
		switch(now){
			#define $$ proVal[0]
			/*
				define $$ here
			*/
			case 0:

				break;
			/*
			 actions here
			 */
			default:
				std::cerr<<"Error occurs with status"<<now<<std::endl;
			#undef $$;
			/*
				undefine $$ here
			*/
		}
		status.push(_GOTO[now][newSymbol-TNBound]);
		valStack.push(proVal[0]);
		delete[] proVal;
	}
}




