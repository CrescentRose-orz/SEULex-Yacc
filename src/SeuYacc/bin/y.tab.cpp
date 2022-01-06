

#include <stdio.h>

extern char yytext[];
extern int column;

void yyerror(char const *s)
{
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);
}


#define ISCPP
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
extern int yyEOF;
extern FILE* yyin;
extern FILE* yyout;
bool yyerrorFlag;
int yyLex();

class LexerEOFException:public std::exception{
public:
	LexerEOFException():std::exception(){}
	LexerEOFException(std::string info){}
	const char* what(){
		return "meet 'end of file' Flag while parser try to read more tokens.";
	}
};



void yyParser::yyParse(){
	int flag = 1;
	status.push(initialStatus);
	read();
	while(flag != -2){
		if (0 < (flag = parse())){
			read();
		}
	}
}
int yyParser::read(){
		//memset(yyVal,0,sizeof(YYSTYPE));
        // char c;
        // c= getchar();
        //system("pause");
		if (yyEOF == 2){
			throw LexerEOFException();
		}
        token = yyLex();
        #ifdef DEBUG_YACC
        cout<<"get token "<<I2S[token]<<" text val:"<<yytext<<endl;
        #endif
        #ifdef DEBUG
		system("pause");
        #endif
        return 0;
    }
void yyParser::shift(int target){
		valStack.push(yyVal);
		yySymbolStack.push(token);
        placeStack.push(nowTokenPlace);
        #ifdef DEBUG
        cout<<"from "<<now;
        #endif
        now = target;
        #ifdef DEBUG
        cout<<" shift to target "<<target<<endl;
		#endif
        status.push(now);
    }
int yyParser::yyMeetError(){

		//ACTION[now][lookAhead] == 0,error
        int errToken = RSerror ;
        yyerrorFlag = true;
        while(!yyEOF&&!status.empty()){
            status.pop();
            valStack.pop();
            placeStack.pop();
            yySymbolStack.pop();
            if (status.empty()){
                return -2;
            }
            now = status.top();
            if (ACTION[now][errToken] == 0){
                continue;
            } else {
                placeStack.push(nowTokenPlace);
                valStack.push(yyVal);
                yySymbolStack.push(RSerror);
                now = ACTION[now][errToken]-1;
                status.push(now);
                while (!yyEOF){
                    errToken = read();
                    if (ACTION[now][errToken] > 0){
                        placeStack.push(nowTokenPlace);
                        valStack.push(yyVal);
                        yySymbolStack.push(RSerror);
                        now = ACTION[now][errToken]-1;
                        status.push(now);
                        //shift
                    } else if (ACTION[now][errToken] != 0) {
                        //ended		
                        reduce(-ACTION[now][errToken]-1);       
                        return -1;
                    } else {
                        return yyMeetError();
                    }
                }
                return -2;
            }
        }
		return -2;
}

int yyParser::parse(){
    int now = status.top();
    if (ACTION[now][token] > 0){
        // ACTION[now][lookAhead] > 0 , shift
		if (ACTION[now][token] == acc){
			cout<<"ACCEPT!"<<endl;
			return -2;
		}
        #ifdef DEBUG
        cout<<"shifting..."<<endl;
        #endif
        shift(ACTION[now][token]-1);
		return 1;
    } else if (ACTION[now][token] < 0){
        // ACTION[now][lookAhead] < 0 , reduce
        #ifdef DEBUG
        cout<<"reducing... action "<<ACTION[now][token]<<endl;
		#endif
        reduce(-ACTION[now][token]-1);
		return -1;
    } else {
		//ACTION[now][lookAhead] == 0,error
        vector<string> possibleToken;
        for (int i = 0 ;i < TNBound;++i){
            if (ACTION[now][i]!=0){
                possibleToken.push_back(I2S[i]);
            }
        }
        for (int i = TNBound; i < NLBound; ++i){
            if (_GOTO[now][i]!=-1){
                possibleToken.push_back(I2S[i]);
            }            
        }
        cout<<"[syntax error] at"<<nowTokenPlace.genMsg();
        for (auto &s:possibleToken){
            cout<<", "<<s;
        }
        cout<<"expected but "<<I2S[token]<<" found"<<endl;
        cout<<"error occurs"<<endl;
        cout<<"now at"<<now<<" get lookAhead "<<token<<"("<<I2S[token]<<") "<<endl;
        return yyMeetError();
    }
}


     // #define SHOW_PRODUCER


      

void yyParser::reduce(int producer){
	    int cnt = proCnt[producer]; 
        int newSymbol = proGet[producer];
        YYSTYPE *proVal = new YYSTYPE[cnt+1];
        tokenPlace *proPlace = new tokenPlace[cnt+1];
        int *yy_sb = new int[cnt+1];

        for (int i = cnt; i > 0; --i){
            yy_sb[i] = yySymbolStack.top();
            yySymbolStack.pop();
            status.pop();
			proVal[i] = valStack.top();
            proPlace[i] = placeStack.top();
            placeStack.pop();
			valStack.pop();
        }
        yySymbolStack.push(newSymbol);
        #ifdef SHOW_PRODUCER
        cout<<I2S[newSymbol]<<"->";
        for (int i = 1; i <= cnt; ++i){
            cout<<I2S[yy_sb[i]]<<" ";
        }
        cout<<endl;
        #endif
        delete[] yy_sb;
        #ifdef DEBUG_YACC
        std::cout<<"begin action for now: "<<now<<"action: "<<producer<<endl;
		#endif
        proPlace[0] = proPlace[1];
        #include"undef.h"
		switch(producer){



            case 0:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 1:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 2:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 3:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 4:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 5:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 6:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 7:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 8:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 9:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 10:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 11:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 12:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 13:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                #define $$6 proPlace[6]
                #define $6 proVal[6]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                #undef $6

                #undef $$6

                break;

            case 14:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                #define $$6 proPlace[6]
                #define $6 proVal[6]

                #define $$7 proPlace[7]
                #define $7 proVal[7]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                #undef $6

                #undef $$6

                #undef $7

                #undef $$7

                break;

            case 15:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 16:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 17:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 18:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 19:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 20:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 21:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 22:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 23:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 24:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 25:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 26:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 27:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 28:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 29:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 30:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 31:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 32:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 33:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 34:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 35:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 36:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 37:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 38:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 39:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 40:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 41:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 42:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 43:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 44:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 45:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 46:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 47:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 48:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 49:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 50:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 51:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 52:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 53:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 54:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 55:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 56:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 57:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 58:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 59:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 60:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                break;

            case 61:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 62:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 63:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 64:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 65:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 66:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 67:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 68:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 69:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 70:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 71:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 72:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 73:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 74:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 75:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 76:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 77:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 78:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 79:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 80:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 81:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 82:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 83:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 84:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 85:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 86:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 87:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 88:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 89:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 90:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 91:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 92:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 93:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 94:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 95:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 96:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 97:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 98:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 99:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 100:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 101:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 102:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 103:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 104:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 105:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 106:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 107:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 108:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 109:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 110:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 111:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                break;

            case 112:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 113:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 114:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 115:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 116:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 117:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 118:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 119:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 120:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 121:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 122:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 123:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 124:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 125:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 126:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 127:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 128:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 129:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                break;

            case 130:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                break;

            case 131:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                #define $$6 proPlace[6]
                #define $6 proVal[6]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                #undef $6

                #undef $$6

                break;

            case 132:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 133:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 134:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 135:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 136:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 137:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 138:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 139:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 140:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 141:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 142:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 143:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 144:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 145:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                break;

            case 146:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 147:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 148:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                #define $$6 proPlace[6]
                #define $6 proVal[6]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                #undef $6

                #undef $$6

                break;

            case 149:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                #define $$6 proPlace[6]
                #define $6 proVal[6]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                #undef $6

                #undef $$6

                break;

            case 150:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                break;

            case 151:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 152:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 153:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 154:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 155:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 156:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 157:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 158:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 159:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 160:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 161:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 162:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 163:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 164:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 165:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 166:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 167:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 168:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 169:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 170:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 171:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 172:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 173:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 174:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 175:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 176:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 177:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 178:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 179:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 180:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 181:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 182:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 183:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 184:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 185:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 186:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 187:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 188:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 189:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 190:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 191:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 192:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 193:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 194:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 195:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 196:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 197:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 198:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 199:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 200:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 201:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 202:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 203:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 204:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 205:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 206:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 207:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 208:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 209:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 210:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 211:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 212:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 213:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 214:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 215:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 216:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                break;

            case 217:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                #define $$6 proPlace[6]
                #define $6 proVal[6]

                #define $$7 proPlace[7]
                #define $7 proVal[7]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                #undef $6

                #undef $$6

                #undef $7

                #undef $$7

                break;

            case 218:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                break;

            case 219:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                break;

            case 220:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                #define $$6 proPlace[6]
                #define $6 proVal[6]

                #define $$7 proPlace[7]
                #define $7 proVal[7]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                #undef $6

                #undef $$6

                #undef $7

                #undef $$7

                break;

            case 221:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                #define $$6 proPlace[6]
                #define $6 proVal[6]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                #undef $6

                #undef $$6

                break;

            case 222:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                #define $$6 proPlace[6]
                #define $6 proVal[6]

                #define $$7 proPlace[7]
                #define $7 proVal[7]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                #undef $6

                #undef $$6

                #undef $7

                #undef $$7

                break;

            case 223:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                #define $$6 proPlace[6]
                #define $6 proVal[6]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                #undef $6

                #undef $$6

                break;

            case 224:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                #define $$5 proPlace[5]
                #define $5 proVal[5]

                #define $$6 proPlace[6]
                #define $6 proVal[6]

                #define $$7 proPlace[7]
                #define $7 proVal[7]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                #undef $5

                #undef $$5

                #undef $6

                #undef $$6

                #undef $7

                #undef $$7

                break;

            case 225:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 226:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 227:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 228:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 229:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 230:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 231:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

            case 232:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 233:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 234:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                #define $$4 proPlace[4]
                #define $4 proVal[4]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                #undef $4

                #undef $$4

                break;

            case 235:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                #define $$3 proPlace[3]
                #define $3 proVal[3]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                #undef $3

                #undef $$3

                break;

            case 236:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                break;

            case 237:

                #define $$$ proPlace[0]
                #define $$ proVal[0]

                #define $$1 proPlace[1]
                #define $1 proVal[1]

                #define $$2 proPlace[2]
                #define $2 proVal[2]

                    

                    #undef $$

                    #undef $$$

                #undef $1

                #undef $$1

                #undef $2

                #undef $$2

                break;

		}
        #ifdef DEBUG
        cout<<"reduce get"<<newSymbol<<" namely "<<I2S[newSymbol]<<endl;
        #endif
        now = status.top();
                #ifdef DEBUG
        cout<<"goto "<<_GOTO[now][newSymbol-TNBound]<<endl;
        #endif
        #ifdef DEBUG_YACC
        cout<<"action ok"<<endl;
        #endif
        now = _GOTO[now][newSymbol-TNBound];
		status.push(now);
        placeStack.push(proPlace[0]);
		valStack.push(proVal[0]);
        delete[] proPlace;
		delete[] proVal;
}

char const *I2S[]={
"\\0",
"\\1",
"\\2",
"\\3",
"\\4",
"\\5",
"\\6",
"\\7",
"\\8",
"\\9",
"\\10",
"\\11",
"\\12",
"\\13",
"\\14",
"\\15",
"\\16",
"\\17",
"\\18",
"\\19",
"\\20",
"\\21",
"\\22",
"\\23",
"\\24",
"\\25",
"\\26",
"\\27",
"\\28",
"\\29",
"",
"",
" ",
"!",
"\"",
"#",
"$",
"%",
"&",
"'",
"(",
")",
"*",
"+",
",",
"-",
".",
"/",
"0",
"1",
"2",
"3",
"4",
"5",
"6",
"7",
"8",
"9",
":",
";",
"<",
"=",
">",
"?",
"@",
"A",
"B",
"C",
"D",
"E",
"F",
"G",
"H",
"I",
"J",
"K",
"L",
"M",
"N",
"O",
"P",
"Q",
"R",
"S",
"T",
"U",
"V",
"W",
"X",
"Y",
"Z",
"[",
"\\",
"]",
"^",
"_",
"`",
"a",
"b",
"c",
"d",
"e",
"f",
"g",
"h",
"i",
"j",
"k",
"l",
"m",
"n",
"o",
"p",
"q",
"r",
"s",
"t",
"u",
"v",
"w",
"x",
"y",
"z",
"{",
"|",
"}",
"~",
"",
"RETURN",
"CONTINUE",
"GOTO",
"WHILE",
"IF",
"DEFAULT",
"ELLIPSIS",
"DO",
"STRUCT",
"RIGHT_ASSIGN",
"IMAGINARY",
"ADD_ASSIGN",
"MUL_ASSIGN",
"DIV_ASSIGN",
"ELSE",
"OR_OP",
"VOLATILE",
"NE_OP",
"UNION",
"LEFT_ASSIGN",
"AUTO",
"GE_OP",
"BREAK",
"EQ_OP",
"SIZEOF",
"PTR_OP",
"AND_OP",
"STRING_LITERAL",
"DEC_OP",
"EXTERN",
"RESTRICT",
"SUB_ASSIGN",
"INC_OP",
"LEFT_OP",
"LE_OP",
"INT",
"RIGHT_OP",
"MOD_ASSIGN",
"REGISTER",
"CONSTANT",
"SHORT",
"AND_ASSIGN",
"XOR_ASSIGN",
"DOUBLE",
"OR_ASSIGN",
"TYPE_NAME",
"CASE",
"STATIC",
"SWITCH",
"UNSIGNED",
"TYPEDEF",
"INLINE",
"VOID",
"ENUM",
"CONST",
"CHAR",
"LONG",
"SIGNED",
"FLOAT",
"IDENTIFIER",
"BOOL",
"FOR",
"COMPLEX",
"declaration_list",
"external_declaration",
"translation_unit",
"jump_statement",
"iteration_statement",
"selection_statement",
"block_item",
"compound_statement",
"statement",
"initializer",
"type_name",
"identifier_list",
"constant_expression",
"assignment_expression",
"struct_or_union_specifier",
"struct_declarator_list",
"designation",
"initializer_list",
"parameter_declaration",
"parameter_list",
"assignment_operator",
"argument_expression_list",
"function_definition",
"start",
"enumerator",
"conditional_expression",
"exclusive_or_expression",
"enumerator_list",
"expression_statement",
"inclusive_or_expression",
"cast_expression",
"relational_expression",
"labeled_statement",
"primary_expression",
"struct_declaration",
"unary_expression",
"unary_operator",
"expression",
"equality_expression",
"logical_or_expression",
"postfix_expression",
"additive_expression",
"logical_and_expression",
"init_declarator",
"shift_expression",
"multiplicative_expression",
"declaration",
"block_item_list",
"type_qualifier",
"declaration_specifiers",
"designator_list",
"storage_class_specifier",
"designator",
"type_specifier",
"and_expression",
"struct_or_union",
"init_declarator_list",
"struct_declaration_list",
"enum_specifier",
"specifier_qualifier_list",
"struct_declarator",
"direct_abstract_declarator",
"function_specifier",
"abstract_declarator",
"declarator",
"direct_declarator",
"pointer",
"type_qualifier_list",
"parameter_type_list"};
int proCnt[238]={
1,1,1,1,3,1,4,3,4,3,3,2,2,6,7,1,3,1,2,2,2,2,4,1,1,1,1,1,1,1,4,1,3,3,3,1,3,3,1,3,3,1,3,3,3,3,1,3,3,1,3,1,3,1,3,1,3,1,3,1,5,1,3,1,1,1,1,1,1,1,1,1,1,1,1,3,1,2,3,1,2,1,2,1,2,1,2,1,3,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,4,2,1,1,1,2,3,2,1,2,1,1,3,1,2,3,4,5,5,6,2,1,3,1,3,1,1,1,1,2,1,1,3,5,4,4,6,6,5,4,3,4,4,3,1,2,2,3,1,2,1,3,1,3,2,2,1,1,3,1,2,1,1,2,3,2,3,3,4,3,4,2,3,3,4,1,3,4,1,2,3,4,2,1,2,3,2,1,1,1,1,1,1,3,4,3,2,3,1,2,1,1,1,2,5,7,5,5,7,6,7,6,7,3,2,2,2,3,1,2,1,1,4,3,1,2};
int proGet[238]={
214,224,224,224,224,231,231,231,231,231,231,231,231,231,231,212,212,226,226,226,226,226,226,227,227,227,227,227,227,221,221,236,236,236,236,232,232,232,235,235,235,222,222,222,222,222,229,229,229,245,245,217,217,220,220,233,233,230,230,216,216,204,204,211,211,211,211,211,211,211,211,211,211,211,228,228,203,237,237,240,240,240,240,240,240,240,240,247,247,234,234,242,242,242,242,242,244,244,244,244,244,244,244,244,244,244,244,244,244,244,244,205,205,205,246,246,248,248,225,250,250,250,250,206,206,251,251,251,249,249,249,249,249,218,218,215,215,239,239,239,253,255,255,256,256,256,256,256,256,256,256,256,256,256,256,256,257,257,257,257,258,258,259,259,210,210,209,209,209,202,202,201,201,254,254,254,252,252,252,252,252,252,252,252,252,252,252,200,200,200,208,208,208,208,207,241,241,243,243,199,199,199,199,199,199,223,223,223,198,198,238,238,197,197,219,219,196,196,196,195,195,195,195,195,195,194,194,194,194,194,193,193,192,192,213,213,191,191};

int _GOTO[400][69] = {
-1,237,260,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,250,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,263,-1,380,142,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,326,233,163,-1,329,3,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,326,233,163,-1,329,33,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,326,233,163,-1,329,382,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,95,210,-1,-1,253,297,301,-1,304,-1,324,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,357,-1,-1,-1,145,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,385,-1,-1,-1,145,-1,-1,279,12,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,390,-1,270,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,35,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,171,253,297,301,-1,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,326,233,163,-1,329,59,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,352,242,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,380,169,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,139,243,36,-1,-1,201,-1,130,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,26,-1,-1,304,-1,324,221,-1,-1,-1,218,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,37,253,297,301,-1,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,29,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,152,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,191,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,339,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,326,233,163,-1,329,345,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,331,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,42,253,297,301,-1,304,-1,150,221,342,351,306,218,225,256,-1,182,307,66,-1,380,175,-1,204,-1,203,310,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,-1,
-1,-1,-1,326,233,163,-1,329,298,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,394,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,326,233,163,-1,329,20,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,43,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,302,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,326,233,163,-1,329,1,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,206,-1,-1,304,-1,324,221,-1,-1,-1,218,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,46,-1,-1,304,-1,324,221,-1,-1,-1,218,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,297,-1,-1,304,-1,324,221,-1,-1,-1,218,48,-1,-1,-1,307,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,210,-1,-1,354,297,301,-1,304,-1,324,221,-1,351,-1,218,225,-1,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,297,-1,-1,304,-1,324,221,-1,-1,-1,218,-1,-1,-1,-1,116,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,396,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,210,-1,-1,253,297,301,-1,304,-1,324,221,-1,351,-1,218,225,80,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,297,55,-1,304,-1,324,221,-1,-1,-1,218,225,-1,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,297,-1,-1,304,-1,324,221,-1,-1,-1,218,-1,-1,-1,-1,49,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,297,305,-1,304,-1,324,221,-1,-1,-1,218,225,-1,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,326,233,163,-1,329,287,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,277,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,314,210,-1,-1,253,297,301,-1,304,-1,324,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,31,-1,-1,-1,178,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,39,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,380,169,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,25,-1,-1,-1,145,-1,-1,24,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,390,-1,270,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,187,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,190,-1,-1,-1,-1,-1,-1,-1,292,173,172,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,365,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,196,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,367,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,350,-1,-1,248,-1,-1,-1,352,242,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,380,169,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,356,
-1,-1,-1,326,233,163,-1,329,197,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,289,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,170,-1,-1,-1,-1,211,-1,296,-1,-1,273,343,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,118,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,309,-1,-1,-1,145,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,346,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,398,-1,380,175,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,25,-1,-1,-1,145,-1,-1,24,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,390,-1,270,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,139,-1,368,186,173,127,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,170,-1,-1,-1,-1,211,-1,296,-1,-1,273,300,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,8,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,178,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,187,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,190,-1,-1,-1,-1,-1,-1,-1,135,173,172,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,196,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,161,387,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,381,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,341,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,31,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,177,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,380,355,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,380,268,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,352,242,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,380,169,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,353,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,68,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,314,210,-1,-1,253,297,301,-1,304,-1,324,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,375,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,314,210,-1,-1,253,297,301,-1,304,-1,324,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,170,-1,-1,-1,-1,211,-1,296,-1,-1,273,188,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,370,-1,-1,331,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,332,351,306,218,225,256,-1,182,307,-1,-1,170,-1,-1,-1,-1,211,310,296,-1,-1,273,374,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,117,-1,-1,304,-1,324,221,-1,-1,-1,218,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,297,-1,-1,304,-1,324,221,-1,-1,-1,218,225,-1,-1,193,307,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,297,301,-1,304,-1,324,221,-1,351,-1,218,225,-1,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,183,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,140,-1,-1,252,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,151,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,313,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,170,-1,-1,-1,-1,211,-1,296,-1,149,273,343,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,380,230,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,199,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,135,173,172,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,314,210,-1,-1,253,297,301,-1,304,-1,324,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,297,301,-1,304,-1,324,221,-1,44,-1,218,225,-1,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,297,-1,-1,304,-1,324,221,-1,-1,-1,218,225,-1,-1,57,307,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,381,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,237,260,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,250,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,263,-1,380,142,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,274,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,250,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,263,-1,380,142,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,96,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,352,242,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,380,169,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,139,243,36,165,173,127,-1,130,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,140,-1,-1,166,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,326,233,163,328,329,330,-1,-1,-1,-1,331,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,164,344,380,175,-1,204,-1,203,310,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,181,-1,-1,-1,145,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,385,-1,-1,-1,145,-1,-1,279,388,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,390,-1,270,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,304,-1,189,221,-1,-1,-1,218,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,304,-1,395,221,-1,-1,-1,218,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,304,-1,267,221,-1,-1,-1,218,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
158,-1,-1,-1,-1,-1,-1,154,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,153,-1,380,175,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,45,-1,-1,304,-1,324,221,-1,-1,-1,218,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,326,233,163,-1,329,88,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,85,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,314,210,-1,-1,253,297,301,-1,304,-1,324,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,297,-1,-1,304,-1,324,221,-1,-1,-1,218,104,-1,-1,-1,307,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,282,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,312,-1,-1,-1,283,173,172,-1,-1,
-1,-1,-1,326,233,163,77,329,330,-1,-1,-1,-1,331,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,164,-1,380,175,-1,204,-1,203,310,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,326,233,163,-1,329,10,-1,-1,-1,-1,331,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,334,253,297,301,338,304,-1,150,221,342,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,196,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,69,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,191,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,165,173,172,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,289,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,170,-1,-1,-1,-1,211,-1,296,-1,-1,273,343,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,139,-1,155,-1,-1,201,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,28,-1,-1,-1,283,173,172,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,373,-1,-1,-1,297,301,-1,304,-1,324,221,-1,351,-1,218,225,-1,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,380,363,-1,204,-1,203,-1,296,-1,-1,273,-1,-1,-1,243,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,313,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,170,-1,-1,-1,-1,211,-1,296,-1,372,273,343,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,191,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,162,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,360,-1,-1,-1,-1,-1,-1,-1,53,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,-1,351,306,218,225,256,-1,182,307,-1,-1,-1,-1,-1,-1,-1,-1,310,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,61,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,60,-1,-1,331,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,332,351,306,218,225,256,-1,182,307,-1,-1,170,-1,-1,-1,-1,211,310,296,-1,-1,273,374,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,58,-1,-1,331,248,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,294,210,-1,-1,253,297,301,-1,304,-1,150,221,332,351,306,218,225,256,-1,182,307,-1,-1,170,-1,-1,-1,-1,211,310,296,-1,-1,273,374,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,297,-1,-1,304,-1,324,221,-1,-1,-1,218,225,-1,-1,288,307,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,297,-1,-1,304,-1,324,221,-1,-1,-1,218,225,-1,-1,238,307,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int ACTION[400][191] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,0,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-225,0,0,0,0,-225,0,-225,0,-225,-225,0,-225,0,0,0,0,0,0,0,0,0,0,0,0,0,-225,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-225,0,-225,-225,0,-225,-225,-225,-225,-225,-225,0,-225,-225,0,-225,0,0,0,-225,0,-225,0,-225,0,-225,0,-225,0,-225,0,0,-225,-225,-225,-225,0,-225,0,0,-225,0,0,-225,-225,-225,0,0,-225,0,-225,-225,-225,-225,-225,-225,-225,-225,-225,-225,-225,-225,-225,-225,-225,-225,-225,-225,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-221,0,0,0,0,-221,0,-221,0,-221,-221,0,-221,0,0,0,0,0,0,0,0,0,0,0,0,0,-221,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-221,0,-221,-221,0,-221,-221,-221,-221,-221,-221,0,-221,-221,0,-221,0,0,0,-221,0,-221,0,-221,0,-221,0,-221,0,-221,0,0,-221,-221,-221,-221,0,-221,0,0,-221,0,0,-221,-221,-221,0,0,-221,0,-221,-221,-221,-221,-221,-221,-221,-221,-221,-221,-221,-221,-221,-221,-221,-221,-221,-221,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-218,0,0,0,0,-218,0,-218,0,-218,-218,0,-218,0,0,0,0,0,0,0,0,0,0,0,0,0,-218,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-218,0,-218,-218,0,-218,-218,-218,-218,-218,-218,0,-218,-218,0,-218,0,0,0,-218,0,-218,0,-218,0,-218,0,-218,0,-218,0,0,-218,-218,-218,-218,0,-218,0,0,-218,0,0,-218,-218,-218,0,0,-218,0,-218,-218,-218,-218,-218,-218,-218,-218,-218,-218,-218,-218,-218,-218,-218,-218,-218,-218,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,282,0,262,294,291,317,318,186,0,321,0,0,0,0,0,0,0,0,0,0,0,0,0,0,385,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,323,0,266,0,0,0,0,0,0,0,0,0,0,324,0,319,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-181,-181,0,0,-181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-183,-183,0,0,-183,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-183,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-187,-187,0,0,-187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0,0,203,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,282,0,262,294,291,317,318,186,0,321,0,0,0,0,0,0,0,0,0,0,0,0,0,0,385,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,323,0,266,0,0,0,0,0,0,0,0,0,0,324,0,319,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-220,0,0,0,0,-220,0,-220,0,-220,-220,0,-220,0,0,0,0,0,0,0,0,0,0,0,0,0,-220,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-220,0,-220,-220,0,-220,-220,-220,-220,-220,-220,0,-220,-220,0,-220,0,0,0,-220,0,-220,0,-220,0,-220,0,-220,0,-220,0,0,-220,-220,-220,-220,0,-220,0,0,-220,0,0,-220,-220,-220,0,0,-220,0,-220,-220,-220,-220,-220,-220,-220,-220,-220,-220,-220,-220,-220,-220,-220,-220,-220,-220,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-17,0,0,-17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,139,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,258,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-180,-180,0,0,-180,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-180,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-186,-186,0,0,-186,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-186,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-182,-182,0,0,-182,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-182,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-185,-185,0,0,-185,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-185,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-177,-177,0,0,-177,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-177,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,282,0,262,294,291,317,318,186,0,321,0,0,0,0,0,0,0,0,0,0,0,0,0,0,385,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,323,0,266,0,0,0,0,0,0,0,0,0,0,324,0,319,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-207,0,0,0,0,-207,0,-207,0,-207,-207,0,-207,0,0,0,0,0,0,0,0,0,0,0,0,0,-207,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-207,0,-207,-207,0,-207,-207,-207,-207,-207,-207,0,-207,-207,0,-207,0,0,0,-207,0,-207,0,-207,0,-207,0,-207,0,-207,0,0,-207,-207,-207,-207,0,-207,0,0,-207,0,0,-207,-207,-207,0,0,-207,0,-207,-207,-207,-207,-207,-207,-207,-207,-207,-207,-207,-207,-207,-207,-207,-207,-207,-207,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-7,-7,0,-7,-7,-7,-7,-7,-7,-7,-7,0,0,0,0,0,0,0,0,0,0,-7,-7,-7,-7,-7,-7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-7,0,-7,-7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-7,-7,0,0,0,0,0,0,0,0,0,0,0,-7,0,-7,-7,-7,0,-7,0,-7,0,-7,0,-7,0,-7,0,-7,-7,0,-7,0,0,-7,-7,-7,-7,0,-7,-7,0,0,0,-7,-7,0,-7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,281,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-193,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-193,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-31,-31,0,0,-31,-31,-31,-31,-31,0,-31,0,0,0,0,0,0,0,0,0,0,-31,-31,-31,-31,-31,-31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-31,-31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-31,-31,0,0,0,0,0,0,0,0,0,0,0,-31,0,-31,-31,-31,0,-31,0,-31,0,-31,0,-31,0,-31,0,0,-31,0,0,0,0,-31,0,-31,-31,0,-31,-31,0,0,0,-31,-31,0,-31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,169,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,246,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,281,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,336,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-151,-151,0,0,-151,0,0,0,0,0,0,0,0,0,0,0,0,0,-151,-151,0,-151,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-151,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-151,0,0,0,0,0,0,0,0,0,0,0,0,-151,0,-151,0,0,0,0,0,-151,0,-151,0,-151,0,0,0,0,0,0,0,0,-151,-151,0,0,0,0,-151,0,0,-151,0,-151,0,0,-151,0,-151,0,-151,0,-151,-151,-151,-151,-151,-151,-151,-151,-151,-151,0,-151,0,-151,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,206,-176,0,0,-176,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,265,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-178,-178,0,0,-178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-222,0,0,0,0,-222,0,-222,0,-222,-222,0,-222,0,0,0,0,0,0,0,0,0,0,0,0,0,-222,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-222,0,-222,-222,0,-222,-222,-222,-222,-222,-222,0,-222,-222,0,-222,0,0,0,-222,0,-222,0,-222,0,-222,0,-222,0,-222,0,0,-222,-222,-222,-222,0,-222,0,0,-222,0,0,-222,-222,-222,0,0,-222,0,-222,-222,-222,-222,-222,-222,-222,-222,-222,-222,-222,-222,-222,-222,-222,-222,-222,-222,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,0,-25,0,-25,0,-25,-25,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,-25,-25,0,0,0,-25,0,0,0,0,0,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,99,0,0,203,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,48,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-184,-184,0,0,-184,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-184,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-166,0,0,-166,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-171,0,0,-171,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-23,-23,0,0,-23,-23,-23,-23,-23,0,-23,0,0,0,0,0,0,0,0,0,0,-23,-23,-23,-23,-23,-23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-23,-23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,28,-23,-23,0,0,0,0,0,0,0,0,0,0,0,-23,0,-23,-23,-23,0,-23,0,-23,0,-23,0,-23,0,-23,0,0,-23,0,0,0,0,-23,0,-23,-23,0,-23,-23,0,0,0,-23,-23,0,-23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,116,0,0,203,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-51,0,0,-51,0,0,-51,0,0,0,0,0,0,0,0,0,0,0,0,0,-51,-51,0,0,0,-51,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-51,-51,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-51,-51,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-51,0,115,0,0,0,0,0,111,0,0,-51,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-33,-33,0,0,-33,-33,-33,-33,-33,0,-33,0,0,0,0,0,0,0,0,0,0,-33,-33,-33,0,-33,-33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-33,-33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-33,-33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-33,0,-33,0,0,0,-33,0,-33,0,0,-33,0,0,0,0,0,0,-33,-33,0,-33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-35,-35,0,0,-35,-35,-35,-35,-35,0,-35,0,0,0,0,0,0,0,0,0,0,-35,-35,-35,0,-35,-35,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-35,-35,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-35,-35,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-35,0,-35,0,0,0,-35,0,-35,0,0,-35,0,0,0,0,0,0,-35,-35,0,-35,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,282,0,262,294,291,317,318,186,0,321,0,0,0,0,0,0,0,0,0,0,0,0,0,0,385,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,323,0,266,0,0,0,0,0,0,0,0,0,0,324,0,319,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-40,0,0,-40,0,113,-40,104,0,0,0,0,0,0,0,0,0,0,0,0,-40,-40,-40,0,-40,-40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-40,-40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-40,-40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-40,0,-40,0,0,0,-40,0,-40,0,0,-40,0,0,0,0,0,0,-40,-40,0,-40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,-37,0,0,-37,296,-37,-37,-37,0,100,0,0,0,0,0,0,0,0,0,0,-37,-37,-37,0,-37,-37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-37,-37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-37,-37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-37,0,-37,0,0,0,-37,0,-37,0,0,-37,0,0,0,0,0,0,-37,-37,0,-37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-11,-11,0,-11,-11,-11,-11,-11,-11,-11,-11,0,0,0,0,0,0,0,0,0,0,-11,-11,-11,-11,-11,-11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-11,0,-11,-11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-11,-11,0,0,0,0,0,0,0,0,0,0,0,-11,0,-11,-11,-11,0,-11,0,-11,0,-11,0,-11,0,-11,0,-11,-11,0,-11,0,0,-11,-11,-11,-11,0,-11,-11,0,0,0,-11,-11,0,-11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-10,-10,0,-10,-10,-10,-10,-10,-10,-10,-10,0,0,0,0,0,0,0,0,0,0,-10,-10,-10,-10,-10,-10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-10,0,-10,-10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-10,-10,0,0,0,0,0,0,0,0,0,0,0,-10,0,-10,-10,-10,0,-10,0,-10,0,-10,0,-10,0,-10,0,-10,-10,0,-10,0,0,-10,-10,-10,-10,0,-10,-10,0,0,0,-10,-10,0,-10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,316,0,0,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-8,-8,0,-8,-8,-8,-8,-8,-8,-8,-8,0,0,0,0,0,0,0,0,0,0,-8,-8,-8,-8,-8,-8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-8,0,-8,-8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-8,-8,0,0,0,0,0,0,0,0,0,0,0,-8,0,-8,-8,-8,0,-8,0,-8,0,-8,0,-8,0,-8,0,-8,-8,0,-8,0,0,-8,-8,-8,-8,0,-8,-8,0,0,0,-8,-8,0,-8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-48,0,0,-48,0,0,-48,0,0,0,0,0,0,0,0,0,0,0,0,0,-48,-48,398,0,229,-48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-48,-48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-48,-48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-48,0,-48,0,0,0,400,0,-48,0,0,-48,0,0,0,0,0,0,0,252,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,237,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-45,0,0,-45,0,0,-45,0,0,0,0,0,0,0,0,0,0,0,0,0,-45,-45,-45,0,-45,-45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-45,-45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-45,-45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-45,0,-45,0,0,0,-45,0,-45,0,0,-45,0,0,0,0,0,0,102,-45,0,338,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,92,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-224,0,0,0,0,-224,0,-224,0,-224,-224,0,-224,0,0,0,0,0,0,0,0,0,0,0,0,0,-224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-224,0,-224,-224,0,-224,-224,-224,-224,-224,-224,0,-224,-224,0,-224,0,0,0,-224,0,-224,0,-224,0,-224,0,-224,0,-224,0,0,-224,-224,-224,-224,0,-224,0,0,-224,0,0,-224,-224,-224,0,0,-224,0,-224,-224,-224,-224,-224,-224,-224,-224,-224,-224,-224,-224,-224,-224,-224,-224,-224,-224,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,42,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-197,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-197,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-197,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,112,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,39,181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,0,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-5,-5,0,-5,-5,-5,-5,-5,-5,-5,-5,0,0,0,0,0,0,0,0,0,0,-5,-5,-5,-5,-5,-5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-5,0,-5,-5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-5,-5,0,0,0,0,0,0,0,0,0,0,0,-5,0,-5,-5,-5,0,-5,0,-5,0,-5,0,-5,0,-5,0,-5,-5,0,-5,0,0,-5,-5,-5,-5,0,-5,-5,0,0,0,-5,-5,0,-5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,28,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-132,-132,-132,0,-132,0,0,0,0,0,0,0,0,0,0,0,0,0,-132,-132,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-132,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-132,0,-132,0,0,0,0,0,-132,0,-132,0,-132,0,0,0,0,0,0,0,0,-132,-132,0,0,0,0,-132,0,0,-132,0,-132,0,0,-132,0,-132,0,-132,0,-132,-132,-132,-132,-132,-132,-132,-132,-132,-132,-132,-132,0,-132,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,276,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,0,-25,0,-25,0,-25,-25,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,-25,-25,0,0,0,-25,0,0,0,0,0,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-164,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,175,0,0,0,0,0,0,0,285,0,0,286,287,0,322,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,142,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,35,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-155,-155,0,0,-155,0,0,0,0,0,0,0,0,0,0,0,0,0,-155,-155,0,-155,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-155,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-155,0,0,0,0,0,0,0,0,0,0,0,0,-155,0,-155,0,0,0,0,0,-155,0,-155,0,-155,0,0,0,0,0,0,0,0,-155,-155,0,0,0,0,-155,0,0,-155,0,-155,0,0,-155,0,-155,0,-155,0,-155,-155,-155,-155,-155,-155,-155,-155,-155,-155,0,-155,0,-155,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-15,-15,0,-15,-15,-15,-15,-15,-15,-15,-15,0,0,0,0,0,0,0,0,0,0,-15,-15,-15,-15,-15,-15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-15,0,-15,-15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-15,-15,0,0,0,0,0,0,0,0,0,0,0,-15,0,-15,-15,-15,0,-15,0,-15,0,-15,0,-15,0,-15,0,-15,-15,0,-15,0,0,-15,-15,-15,-15,0,-15,-15,0,0,0,-15,-15,0,-15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-212,0,0,0,0,-212,0,-212,0,-212,-212,0,-212,0,0,0,0,0,0,0,0,0,0,0,0,0,-212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-212,0,-212,-212,0,-212,-212,-212,-212,-212,-212,0,-212,-212,0,-212,0,0,0,0,0,-212,0,-212,0,-212,0,-212,0,-212,0,0,-212,-212,-212,-212,0,-212,0,0,-212,0,0,-212,-212,-212,0,0,-212,0,-212,-212,-212,-212,-212,-212,-212,-212,-212,-212,-212,-212,-212,-212,-212,-212,-212,-212,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,282,0,262,294,291,317,318,186,0,321,0,0,0,0,0,0,0,0,0,0,0,0,0,0,385,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,323,0,266,0,0,0,0,0,0,0,0,0,0,324,0,319,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-210,0,-210,0,0,-210,0,-210,0,-210,-210,0,-210,0,0,0,0,0,0,0,0,0,0,0,0,0,-210,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-210,0,-210,-210,0,-210,-210,-210,-210,-210,-210,0,-210,-210,0,-210,0,0,0,-210,0,-210,0,-210,0,-210,0,-210,0,-210,0,0,-210,-210,-210,-210,0,-210,0,0,-210,0,0,-210,-210,-210,0,0,-210,0,-210,-210,-210,-210,-210,-210,-210,-210,-210,-210,-210,-210,-210,-210,-210,-210,-210,-210,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-59,0,0,-59,0,0,0,0,0,0,0,0,0,0,0,0,0,-59,-59,0,0,0,-59,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-59,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-59,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-59,0,0,0,0,0,0,0,0,0,0,103,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-216,0,0,0,0,-216,0,-216,-216,-216,-216,0,-216,0,0,0,0,0,0,0,0,0,0,0,0,0,-216,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-216,0,-216,-216,0,-216,-216,-216,-216,-216,-216,0,-216,-216,0,-216,0,0,0,-216,0,-216,0,-216,0,-216,0,-216,0,-216,0,0,-216,-216,-216,-216,0,-216,0,0,-216,0,0,-216,-216,-216,0,0,-216,0,-216,-216,-216,-216,-216,-216,-216,-216,-216,-216,-216,-216,-216,-216,-216,-216,-216,-216,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,285,0,0,286,287,227,322,0,292,0,0,309,0,0,236,328,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,221,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,282,0,262,294,291,317,318,186,0,321,0,0,0,0,0,0,0,0,0,0,0,0,0,0,385,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,323,0,266,0,0,0,0,0,0,0,0,0,0,324,0,319,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,88,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-228,0,0,0,0,-228,0,-228,0,-228,-228,0,-228,0,0,0,0,0,0,0,0,0,0,0,0,0,-228,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-228,0,-228,-228,0,-228,-228,-228,-228,-228,-228,0,-228,-228,0,-228,0,0,0,-228,0,-228,0,-228,0,-228,0,-228,0,-228,0,0,-228,-228,-228,-228,0,-228,0,0,-228,0,0,-228,-228,-228,0,0,-228,0,-228,-228,-228,-228,-228,-228,-228,-228,-228,-228,-228,-228,-228,-228,-228,-228,-228,-228,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,282,0,262,294,291,317,318,186,0,321,0,0,0,0,0,0,0,0,0,0,0,0,0,0,385,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,323,0,266,0,0,0,0,0,0,0,0,0,0,324,0,319,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,57,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-190,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-190,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-190,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,260,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-227,0,0,0,0,-227,0,-227,0,-227,-227,0,-227,0,0,0,0,0,0,0,0,0,0,0,0,0,-227,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-227,0,-227,-227,0,-227,-227,-227,-227,-227,-227,0,-227,-227,0,-227,0,0,0,-227,0,-227,0,-227,0,-227,0,-227,0,-227,0,0,-227,-227,-227,-227,0,-227,0,0,-227,0,0,-227,-227,-227,0,0,-227,0,-227,-227,-227,-227,-227,-227,-227,-227,-227,-227,-227,-227,-227,-227,-227,-227,-227,-227,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-61,0,0,-61,0,0,0,0,0,0,0,0,0,0,0,0,0,-61,-61,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-61,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-61,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,203,0,0,0,0,0,0,0,0,0,0,0,0,0,0,341,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-229,0,0,0,0,-229,0,-229,0,-229,-229,0,-229,0,0,0,0,0,0,0,0,0,0,0,0,0,-229,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-229,0,-229,-229,0,-229,-229,-229,-229,-229,-229,0,-229,-229,0,-229,0,0,0,-229,0,-229,0,-229,0,-229,0,-229,0,-229,0,0,-229,-229,-229,-229,0,-229,0,0,-229,0,0,-229,-229,-229,0,0,-229,0,-229,-229,-229,-229,-229,-229,-229,-229,-229,-229,-229,-229,-229,-229,-229,-229,-229,-229,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,282,0,262,294,291,317,318,186,0,321,0,0,0,0,0,0,0,0,0,0,0,0,0,0,385,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,323,0,266,0,0,0,0,0,0,0,0,0,0,324,0,319,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-41,0,0,-41,0,113,-41,104,0,0,0,0,0,0,0,0,0,0,0,0,-41,-41,-41,0,-41,-41,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-41,-41,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-41,-41,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-41,0,-41,0,0,0,-41,0,-41,0,0,-41,0,0,0,0,0,0,-41,-41,0,-41,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-13,-13,0,-13,-13,-13,-13,-13,-13,-13,-13,0,0,0,0,0,0,0,0,0,0,-13,-13,-13,-13,-13,-13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-13,0,-13,-13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-13,-13,0,0,0,0,0,0,0,0,0,0,0,-13,0,-13,-13,-13,0,-13,0,-13,0,-13,0,-13,0,-13,0,-13,-13,0,-13,0,0,-13,-13,-13,-13,0,-13,-13,0,0,0,-13,-13,0,-13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,51,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,53,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-198,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-198,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-198,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-12,-12,0,-12,-12,-12,-12,-12,-12,-12,-12,0,0,0,0,0,0,0,0,0,0,-12,-12,-12,-12,-12,-12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-12,0,-12,-12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-12,-12,0,0,0,0,0,0,0,0,0,0,0,-12,0,-12,-12,-12,0,-12,0,-12,0,-12,0,-12,0,-12,0,-12,-12,0,-12,0,0,-12,-12,-12,-12,0,-12,-12,0,0,0,-12,-12,0,-12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,282,0,262,294,291,317,318,186,0,321,0,0,0,0,0,0,0,0,0,0,0,0,0,0,385,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,323,0,266,0,0,0,0,0,0,0,0,0,0,324,0,319,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,-38,0,0,-38,296,-38,-38,-38,0,100,0,0,0,0,0,0,0,0,0,0,-38,-38,-38,0,-38,-38,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-38,-38,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-38,-38,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-38,0,-38,0,0,0,-38,0,-38,0,0,-38,0,0,0,0,0,0,-38,-38,0,-38,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-21,-21,0,0,-21,-21,-21,-21,-21,0,-21,0,0,0,0,0,0,0,0,0,0,-21,-21,-21,-21,-21,-21,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-21,-21,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-21,-21,0,0,0,0,0,0,0,0,0,0,0,-21,0,-21,-21,-21,0,-21,0,-21,0,-21,0,-21,0,-21,0,0,-21,0,0,0,0,-21,0,-21,-21,0,-21,-21,0,0,0,-21,-21,0,-21,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-74,0,0,0,0,-74,0,-74,0,-74,-74,0,-74,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-74,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-74,0,0,-74,-74,0,0,0,-74,0,0,0,0,0,0,-74,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-74,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-73,0,0,0,0,-73,0,-73,0,-73,-73,0,-73,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-73,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-73,0,0,-73,-73,0,0,0,-73,0,0,0,0,0,0,-73,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-73,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-72,0,0,0,0,-72,0,-72,0,-72,-72,0,-72,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-72,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-72,0,0,-72,-72,0,0,0,-72,0,0,0,0,0,0,-72,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-72,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-69,0,0,0,0,-69,0,-69,0,-69,-69,0,-69,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-69,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-69,0,0,-69,-69,0,0,0,-69,0,0,0,0,0,0,-69,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-69,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-154,-154,0,0,-154,0,0,0,0,0,0,0,0,0,0,0,0,0,-154,-154,0,-154,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-154,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-154,0,0,0,0,0,0,0,0,0,0,0,0,-154,0,-154,0,0,0,0,0,-154,0,-154,0,-154,0,0,0,0,0,0,0,0,-154,-154,0,0,0,0,-154,0,0,-154,0,-154,0,0,-154,0,-154,0,-154,0,-154,-154,-154,-154,-154,-154,-154,-154,-154,-154,0,-154,0,-154,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-70,0,0,0,0,-70,0,-70,0,-70,-70,0,-70,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-70,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-70,0,0,-70,-70,0,0,0,-70,0,0,0,0,0,0,-70,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-70,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-66,0,0,0,0,-66,0,-66,0,-66,-66,0,-66,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-66,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-66,0,0,-66,-66,0,0,0,-66,0,0,0,0,0,0,-66,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-66,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,270,-174,0,0,-174,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,228,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-65,0,0,0,0,-65,0,-65,0,-65,-65,0,-65,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-65,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-65,0,0,-65,-65,0,0,0,-65,0,0,0,0,0,0,-65,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-65,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,73,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,0,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-68,0,0,0,0,-68,0,-68,0,-68,-68,0,-68,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-68,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-68,0,0,-68,-68,0,0,0,-68,0,0,0,0,0,0,-68,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-68,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-27,0,0,0,0,-27,0,-27,0,-27,-27,0,-27,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-27,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-27,0,0,-27,-27,0,0,0,-27,0,0,0,0,0,0,-27,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-27,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,0,-25,0,-25,0,-25,-25,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,-25,-25,0,0,0,-25,0,0,0,0,0,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-90,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-90,0,158,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-129,-129,-129,0,-129,0,0,0,0,0,0,0,0,0,0,0,0,0,-129,-129,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-129,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-129,0,-129,0,0,0,0,0,-129,0,-129,0,-129,0,0,0,0,0,0,0,0,-129,-129,0,0,0,0,-129,0,0,-129,0,-129,0,0,-129,0,-129,0,-129,0,-129,-129,-129,-129,-129,-129,-129,-129,-129,-129,-129,-129,0,-129,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-114,-114,-114,0,-114,0,0,0,0,0,0,0,0,0,0,0,0,0,-114,-114,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-114,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,387,0,0,0,0,0,0,0,0,0,0,0,0,-114,0,-114,0,0,0,0,0,-114,0,-114,0,-114,0,0,0,0,0,0,0,0,-114,-114,0,0,0,0,-114,0,0,-114,0,-114,0,0,-114,0,-114,0,-114,0,-114,-114,-114,-114,-114,-114,-114,-114,-114,-114,-114,-114,0,-114,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,169,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,246,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,281,0,77,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,206,-175,0,0,-175,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,265,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-138,0,0,0,0,-138,0,-138,-138,-138,-138,-138,-138,0,0,0,0,0,0,0,0,0,0,0,0,-138,-138,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-138,0,-138,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-138,0,0,0,0,0,0,0,0,0,-138,0,-138,0,0,0,0,0,-138,0,-138,0,-138,0,0,0,-138,0,0,-138,-138,-138,-138,0,-138,0,0,-138,0,0,-138,-138,-138,0,0,-138,0,-138,0,-138,0,-138,-138,-138,-138,-138,-138,-138,-138,-138,-138,-138,-138,0,-138,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,370,0,181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,263,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,228,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-195,0,0,0,0,-195,0,-195,0,-195,-195,0,-195,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-195,0,0,-195,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-195,0,0,-195,-195,0,0,0,-195,0,0,0,0,0,0,-195,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-195,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-26,0,0,0,0,-26,0,-26,0,-26,-26,0,-26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-26,0,0,-26,-26,0,0,0,-26,0,0,0,0,0,0,-26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-26,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-188,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-188,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-188,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,359,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,362,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,175,0,0,0,0,0,0,0,285,0,0,286,287,0,322,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,365,0,0,0,0,0,0,142,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,348,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,350,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,282,0,262,294,291,317,318,186,0,321,0,0,0,0,0,0,0,0,0,0,0,0,0,0,385,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,323,0,266,0,0,0,0,0,0,0,0,0,0,324,0,319,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,372,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,0,0,0,0,0,0,0,0,0,0,322,0,0,0,0,309,0,0,0,0,367,0,0,199,0,363,0,0,0,248,0,0,213,180,142,208,195,312,240,0,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-30,-30,0,0,-30,-30,-30,-30,-30,0,-30,0,0,0,0,0,0,0,0,0,0,-30,-30,-30,379,-30,-30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-30,-30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-30,-30,0,0,0,0,0,0,0,0,0,0,0,304,0,133,129,127,0,-30,0,-30,0,126,0,-30,0,-30,0,0,-30,0,0,0,0,123,0,-30,-30,0,-30,300,0,0,0,122,121,0,120,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,130,0,0,203,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,177,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-237,0,0,0,0,0,0,0,0,0,0,0,0,-237,0,-237,0,0,0,0,0,-237,0,-237,0,-237,0,0,0,0,0,0,0,0,-237,-237,0,0,0,0,-237,0,0,-237,0,-237,0,0,-237,0,-237,0,-237,0,-237,-237,-237,-237,-237,-237,-237,-237,-237,-237,0,-237,0,-237,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-236,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-236,0,-236,0,0,0,0,0,-236,0,-236,0,-236,0,0,0,0,0,0,0,0,-236,-236,0,0,0,0,-236,0,0,-236,0,-236,0,0,-236,0,-236,0,-236,0,-236,-236,-236,-236,-236,-236,-236,-236,-236,-236,0,-236,0,-236,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-173,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-119,0,0,0,0,0,0,0,0,0,0,-119,0,-119,0,0,0,0,0,-119,0,-119,0,0,0,0,0,0,0,0,0,0,0,-119,0,0,0,0,-119,0,0,0,0,-119,0,0,-119,0,-119,0,0,0,-119,0,0,-119,-119,-119,-119,-119,-119,-119,0,-119,0,-119,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,281,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,0,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-189,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-189,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-189,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-130,-130,-130,0,-130,0,0,0,0,0,0,0,0,0,0,0,0,0,-130,-130,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-130,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-130,0,-130,0,0,0,0,0,-130,0,-130,0,-130,0,0,0,0,0,0,0,0,-130,-130,0,0,0,0,-130,0,0,-130,0,-130,0,0,-130,0,-130,0,-130,0,-130,-130,-130,-130,-130,-130,-130,-130,-130,-130,-130,-130,0,-130,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-159,-159,0,0,-159,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-159,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-159,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-160,-160,0,0,-160,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-160,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-160,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-203,0,0,0,0,-203,0,-203,0,-203,-203,0,-203,0,0,0,0,0,0,0,0,0,0,0,0,0,-203,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-203,0,-203,-203,0,-203,-203,-203,-203,-203,-203,0,-203,-203,0,-203,0,0,0,-203,0,-203,0,-203,0,-203,0,-203,0,-203,0,0,-203,-203,-203,-203,0,-203,0,0,-203,0,0,-203,-203,-203,0,0,-203,0,-203,-203,-203,-203,-203,-203,-203,-203,-203,-203,-203,-203,-203,-203,-203,-203,-203,-203,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-213,0,0,0,0,-213,0,-213,0,-213,-213,0,-213,0,0,0,0,0,0,0,0,0,0,0,0,0,-213,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-213,0,-213,-213,0,-213,-213,-213,-213,-213,-213,0,-213,-213,0,-213,0,0,0,0,0,-213,0,-213,0,-213,0,-213,0,-213,0,0,-213,-213,-213,-213,0,-213,0,0,-213,0,0,-213,-213,-213,0,0,-213,0,-213,-213,-213,-213,-213,-213,-213,-213,-213,-213,-213,-213,-213,-213,-213,-213,-213,-213,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,218,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,137,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,169,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,246,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,281,0,93,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,214,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,270,-169,181,0,-169,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,228,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-123,-123,-123,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-123,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-123,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,0,0,0,0,0,0,0,0,0,0,322,0,0,0,0,309,0,0,0,0,367,0,0,199,0,363,0,0,0,248,0,0,213,180,142,208,195,312,240,-123,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,10,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,370,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,228,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,148,-143,0,0,-143,0,0,0,0,0,0,0,0,0,0,0,0,0,-143,-143,0,-143,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,147,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-143,0,0,0,0,0,0,0,0,0,0,0,0,-143,0,-143,0,0,0,0,0,-143,0,-143,0,-143,0,0,0,0,0,0,0,0,-143,-143,0,0,0,0,-143,0,0,-143,0,-143,0,0,-143,0,-143,0,-143,0,-143,-143,-143,-143,-143,-143,-143,-143,-143,-143,0,-143,0,-143,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-140,0,0,0,0,-140,0,-140,-140,-140,-140,-140,-140,0,0,0,0,0,0,0,0,0,0,0,0,-140,-140,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-140,0,-140,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-140,0,0,0,0,0,0,0,0,0,-140,0,-140,0,0,0,0,0,-140,0,-140,0,-140,0,0,0,-140,0,0,-140,-140,-140,-140,0,-140,0,0,-140,0,0,-140,-140,-140,0,0,-140,0,-140,0,-140,0,-140,-140,-140,-140,-140,-140,-140,-140,-140,-140,-140,-140,0,-140,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,370,0,181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,263,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,228,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-149,-149,0,0,-149,0,0,0,0,0,0,0,0,0,0,0,0,0,-149,-149,0,-149,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-149,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-149,0,0,0,0,0,0,0,0,0,0,0,0,-149,0,-149,0,0,0,0,0,-149,0,-149,0,-149,0,0,0,0,0,0,0,0,-149,-149,0,0,0,0,-149,0,0,-149,0,-149,0,0,-149,0,-149,0,-149,0,-149,-149,-149,-149,-149,-149,-149,-149,-149,-149,0,-149,0,-149,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-76,0,0,-76,0,0,0,0,0,0,0,0,0,0,0,0,0,-76,-76,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-76,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,148,-142,0,0,-142,0,0,0,0,0,0,0,0,0,0,0,0,0,-142,-142,0,-142,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,147,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-142,0,0,0,0,0,0,0,0,0,0,0,0,-142,0,-142,0,0,0,0,0,-142,0,-142,0,-142,0,0,0,0,0,0,0,0,-142,-142,0,0,0,0,-142,0,0,-142,0,-142,0,0,-142,0,-142,0,-142,0,-142,-142,-142,-142,-142,-142,-142,-142,-142,-142,0,-142,0,-142,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,272,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,273,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-157,-157,181,0,-157,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-157,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,175,0,0,0,0,0,0,0,0,0,0,0,0,0,322,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,142,0,0,0,0,-157,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-42,0,0,-42,0,0,-42,0,0,0,0,0,0,0,0,0,0,0,0,0,-42,-42,-42,0,-42,-42,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-42,-42,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-42,-42,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-42,0,-42,0,0,0,-42,0,-42,0,0,-42,0,0,0,0,0,0,102,-42,0,338,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,250,0,0,-53,0,0,-53,0,0,0,0,0,0,0,0,0,0,0,0,0,-53,-53,0,0,0,-53,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-53,-53,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-53,-53,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-53,0,0,0,0,0,0,0,0,0,0,-53,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-112,-112,-112,0,-112,0,0,0,0,0,0,0,0,0,0,0,0,0,-112,-112,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-112,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-112,0,-112,0,0,0,0,0,-112,0,-112,0,-112,0,0,0,0,0,0,0,0,-112,-112,0,0,0,0,-112,0,0,-112,0,-112,0,0,-112,0,-112,0,-112,0,-112,-112,-112,-112,-112,-112,-112,-112,-112,-112,-112,-112,0,-112,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,149,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-167,0,0,-167,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-88,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-88,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-120,-120,-120,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-120,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-120,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-120,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-22,-22,0,0,-22,-22,-22,-22,-22,0,-22,0,0,0,0,0,0,0,0,0,0,-22,-22,-22,-22,-22,-22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-22,-22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-22,-22,0,0,0,0,0,0,0,0,0,0,0,-22,0,-22,-22,-22,0,-22,0,-22,0,-22,0,-22,0,-22,0,0,-22,0,0,0,0,-22,0,-22,-22,0,-22,-22,0,0,0,-22,-22,0,-22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,245,0,0,0,0,0,0,0,0,0,0,0,0,0,0,242,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-162,0,0,0,0,-162,0,-162,-162,-162,-162,-162,-162,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-162,0,-162,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-162,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-162,0,0,0,0,0,0,0,-162,0,0,-162,-162,0,-162,0,-162,0,0,0,0,0,0,-162,0,0,0,0,0,0,0,-162,0,0,0,0,0,0,-162,0,0,0,0,-162,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,68,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,279,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-44,0,0,-44,0,0,-44,0,0,0,0,0,0,0,0,0,0,0,0,0,-44,-44,-44,0,-44,-44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-44,-44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-44,-44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-44,0,-44,0,0,0,-44,0,-44,0,0,-44,0,0,0,0,0,0,102,-44,0,338,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-101,-101,-101,0,-101,0,0,0,0,0,0,0,0,0,0,0,0,0,-101,-101,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-101,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-101,0,-101,0,0,0,0,0,-101,0,-101,0,-101,0,0,0,0,0,0,0,0,-101,-101,0,0,0,0,-101,0,0,-101,0,-101,0,0,-101,0,-101,0,-101,0,-101,-101,-101,-101,-101,-101,-101,-101,-101,-101,-101,-101,0,-101,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-161,0,0,0,0,-161,0,-161,-161,-161,-161,-161,-161,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-161,0,-161,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-161,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-161,0,0,0,0,0,0,0,-161,0,0,-161,-161,0,-161,0,-161,0,0,0,0,0,0,-161,0,0,0,0,0,0,0,-161,0,0,0,0,0,0,-161,0,0,0,0,-161,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-208,0,0,0,0,-208,0,-208,0,-208,-208,0,-208,0,0,0,0,0,0,0,0,0,0,0,0,0,-208,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-208,0,-208,-208,0,-208,-208,-208,-208,-208,-208,0,-208,-208,0,-208,0,0,0,-208,0,-208,0,-208,0,-208,0,-208,0,-208,0,0,-208,-208,-208,-208,0,-208,0,0,-208,0,0,-208,-208,-208,0,0,-208,0,-208,-208,-208,-208,-208,-208,-208,-208,-208,-208,-208,-208,-208,-208,-208,-208,-208,-208,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-103,-103,-103,0,-103,0,0,0,0,0,0,0,0,0,0,0,0,0,-103,-103,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-103,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-103,0,-103,0,0,0,0,0,-103,0,-103,0,-103,0,0,0,0,0,0,0,0,-103,-103,0,0,0,0,-103,0,0,-103,0,-103,0,0,-103,0,-103,0,-103,0,-103,-103,-103,-103,-103,-103,-103,-103,-103,-103,-103,-103,0,-103,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-89,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-89,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-108,-108,-108,0,-108,0,0,0,0,0,0,0,0,0,0,0,0,0,-108,-108,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-108,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-108,0,-108,0,0,0,0,0,-108,0,-108,0,-108,0,0,0,0,0,0,0,0,-108,-108,0,0,0,0,-108,0,0,-108,0,-108,0,0,-108,0,-108,0,-108,0,-108,-108,-108,-108,-108,-108,-108,-108,-108,-108,-108,-108,0,-108,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,-174,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-82,-82,-82,0,-82,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-82,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-82,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,-82,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-80,-80,-80,0,-80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,-80,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,0,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-34,-34,0,0,-34,-34,-34,-34,-34,0,-34,0,0,0,0,0,0,0,0,0,0,-34,-34,-34,0,-34,-34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-34,-34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-34,-34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-34,0,-34,0,0,0,-34,0,-34,0,0,-34,0,0,0,0,0,0,-34,-34,0,-34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-98,-98,-98,0,-98,0,0,0,0,0,0,0,0,0,0,0,0,0,-98,-98,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-98,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-98,0,-98,0,0,0,0,0,-98,0,-98,0,-98,0,0,0,0,0,0,0,0,-98,-98,0,0,0,0,-98,0,0,-98,0,-98,0,0,-98,0,-98,0,-98,0,-98,-98,-98,-98,-98,-98,-98,-98,-98,-98,-98,-98,0,-98,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-54,0,0,-54,0,0,0,0,0,0,0,0,0,0,0,0,0,-54,-54,0,0,0,-54,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-54,230,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-54,-54,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-54,0,0,0,0,0,0,0,0,0,0,-54,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-121,-121,-121,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-121,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-121,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,0,0,0,0,0,0,0,0,0,0,322,0,0,0,0,309,0,0,0,0,367,0,0,199,0,363,0,0,0,248,0,0,213,180,142,208,195,312,240,-121,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-97,-97,-97,0,-97,0,0,0,0,0,0,0,0,0,0,0,0,0,-97,-97,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-97,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-97,0,-97,0,0,0,0,0,-97,0,-97,0,-97,0,0,0,0,0,0,0,0,-97,-97,0,0,0,0,-97,0,0,-97,0,-97,0,0,-97,0,-97,0,-97,0,-97,-97,-97,-97,-97,-97,-97,-97,-97,-97,-97,-97,0,-97,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-199,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-199,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-199,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-115,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-115,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,0,0,0,0,285,0,0,286,287,0,322,0,292,0,0,309,0,0,0,328,367,0,0,199,0,363,0,0,0,248,0,0,213,180,142,208,195,312,240,221,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-152,-152,0,0,-152,0,0,0,0,0,0,0,0,0,0,0,0,0,-152,-152,0,-152,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-152,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-152,0,0,0,0,0,0,0,0,0,0,0,0,-152,0,-152,0,0,0,0,0,-152,0,-152,0,-152,0,0,0,0,0,0,0,0,-152,-152,0,0,0,0,-152,0,0,-152,0,-152,0,0,-152,0,-152,0,-152,0,-152,-152,-152,-152,-152,-152,-152,-152,-152,-152,0,-152,0,-152,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-145,-145,0,0,-145,0,0,0,0,0,0,0,0,0,0,0,0,0,-145,-145,0,-145,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-145,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-145,0,0,0,0,0,0,0,0,0,0,0,0,-145,0,-145,0,0,0,0,0,-145,0,-145,0,-145,0,0,0,0,0,0,0,0,-145,-145,0,0,0,0,-145,0,0,-145,0,-145,0,0,-145,0,-145,0,-145,0,-145,-145,-145,-145,-145,-145,-145,-145,-145,-145,0,-145,0,-145,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-18,-18,0,390,-18,-18,-18,-18,-18,109,-18,0,0,0,0,0,0,0,0,0,0,-18,-18,-18,-18,-18,-18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,108,0,-18,-18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-18,-18,0,0,0,0,0,0,0,0,0,0,0,-18,0,-18,-18,-18,0,-18,0,-18,0,-18,0,-18,0,-18,0,107,-18,0,106,0,0,-18,114,-18,-18,0,-18,-18,0,0,0,-18,-18,0,-18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-141,-141,-141,0,-141,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-141,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-141,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-141,0,-141,0,0,0,0,0,-141,0,-141,0,-141,0,0,0,0,0,0,0,0,-141,-141,0,0,0,0,-141,0,0,-141,0,-141,0,0,-141,0,-141,0,-141,0,-141,-141,-141,-141,-141,-141,-141,-141,-141,-141,-141,-141,0,-141,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-2,-2,0,-2,-2,-2,-2,-2,-2,-2,-2,0,0,0,0,0,0,0,0,0,0,-2,-2,-2,-2,-2,-2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-2,0,-2,-2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-2,-2,0,0,0,0,0,0,0,0,0,0,0,-2,0,-2,-2,-2,0,-2,0,-2,0,-2,0,-2,0,-2,0,-2,-2,0,-2,0,0,-2,-2,-2,-2,0,-2,-2,0,0,0,-2,-2,0,-2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-94,-94,-94,0,-94,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-94,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-94,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-94,0,-94,0,0,0,0,0,-94,0,-94,0,-94,0,0,0,0,0,0,0,0,-94,-94,0,0,0,0,-94,0,0,-94,0,-94,0,0,-94,0,-94,0,-94,0,-94,-94,-94,-94,-94,-94,-94,-94,-94,-94,-94,-94,0,-94,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-24,0,0,0,0,-24,0,-24,0,-24,-24,0,-24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-24,0,0,-24,-24,0,0,0,-24,0,0,0,0,0,0,-24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-24,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-209,0,-209,0,0,-209,0,-209,0,-209,-209,0,-209,0,0,0,0,0,0,0,0,0,0,0,0,0,-209,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-209,0,-209,-209,0,-209,-209,-209,-209,-209,-209,0,-209,-209,0,-209,0,0,0,-209,0,-209,0,-209,0,-209,0,-209,0,-209,0,0,-209,-209,-209,-209,0,-209,0,0,-209,0,0,-209,-209,-209,0,0,-209,0,-209,-209,-209,-209,-209,-209,-209,-209,-209,-209,-209,-209,-209,-209,-209,-209,-209,-209,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-39,0,0,-39,0,113,-39,104,0,0,0,0,0,0,0,0,0,0,0,0,-39,-39,-39,0,-39,-39,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-39,-39,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-39,-39,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-39,0,-39,0,0,0,-39,0,-39,0,0,-39,0,0,0,0,0,0,-39,-39,0,-39,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-93,-93,-93,0,-93,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-93,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-93,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-93,0,-93,0,0,0,0,0,-93,0,-93,0,-93,0,0,0,0,0,0,0,0,-93,-93,0,0,0,0,-93,0,0,-93,0,-93,0,0,-93,0,-93,0,-93,0,-93,-93,-93,-93,-93,-93,-93,-93,-93,-93,-93,-93,0,-93,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-144,-144,0,0,-144,0,0,0,0,0,0,0,0,0,0,0,0,0,-144,-144,0,-144,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-144,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-144,0,0,0,0,0,0,0,0,0,0,0,0,-144,0,-144,0,0,0,0,0,-144,0,-144,0,-144,0,0,0,0,0,0,0,0,-144,-144,0,0,0,0,-144,0,0,-144,0,-144,0,0,-144,0,-144,0,-144,0,-144,-144,-144,-144,-144,-144,-144,-144,-144,-144,0,-144,0,-144,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-87,-87,-87,0,-87,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-87,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-87,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-87,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-215,0,0,0,0,-215,0,-215,-215,-215,-215,0,-215,0,0,0,0,0,0,0,0,0,0,0,0,0,-215,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-215,0,-215,-215,0,-215,-215,-215,-215,-215,-215,0,-215,-215,0,-215,0,0,0,-215,0,-215,0,-215,0,-215,0,-215,0,-215,0,0,-215,-215,-215,-215,0,-215,0,0,-215,0,0,-215,-215,-215,0,0,-215,0,-215,-215,-215,-215,-215,-215,-215,-215,-215,-215,-215,-215,-215,-215,-215,-215,-215,-215,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,279,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-204,0,0,0,0,-204,0,-204,0,-204,-204,0,-204,0,0,0,0,0,0,0,0,0,0,0,0,0,-204,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-204,0,-204,-204,0,-204,-204,-204,-204,-204,-204,0,-204,-204,0,-204,0,0,0,-204,0,-204,0,-204,0,-204,0,-204,0,-204,0,0,-204,-204,-204,-204,0,-204,0,0,-204,0,0,-204,-204,-204,0,0,-204,0,-204,-204,-204,-204,-204,-204,-204,-204,-204,-204,-204,-204,-204,-204,-204,-204,-204,-204,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-106,-106,-106,0,-106,0,0,0,0,0,0,0,0,0,0,0,0,0,-106,-106,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-106,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-106,0,-106,0,0,0,0,0,-106,0,-106,0,-106,0,0,0,0,0,0,0,0,-106,-106,0,0,0,0,-106,0,0,-106,0,-106,0,0,-106,0,-106,0,-106,0,-106,-106,-106,-106,-106,-106,-106,-106,-106,-106,-106,-106,0,-106,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-96,-96,-96,0,-96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-96,0,-96,0,0,0,0,0,-96,0,-96,0,-96,0,0,0,0,0,0,0,0,-96,-96,0,0,0,0,-96,0,0,-96,0,-96,0,0,-96,0,-96,0,-96,0,-96,-96,-96,-96,-96,-96,-96,-96,-96,-96,-96,-96,0,-96,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-231,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-231,0,-231,0,0,0,0,0,-231,0,-231,0,-231,0,0,0,0,0,0,0,0,-231,-231,0,0,0,0,-231,0,0,-231,0,-231,0,0,-231,0,-231,0,-231,0,-231,-231,-231,-231,-231,-231,-231,-231,-231,-231,0,-231,0,-231,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-46,0,0,-46,0,0,-46,0,0,0,0,0,0,0,0,0,0,0,0,0,-46,-46,-46,0,-46,-46,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-46,-46,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-46,-46,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-46,0,-46,0,0,0,-46,0,-46,0,0,-46,0,0,0,0,0,0,102,-46,0,338,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-102,-102,-102,0,-102,0,0,0,0,0,0,0,0,0,0,0,0,0,-102,-102,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-102,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-102,0,-102,0,0,0,0,0,-102,0,-102,0,-102,0,0,0,0,0,0,0,0,-102,-102,0,0,0,0,-102,0,0,-102,0,-102,0,0,-102,0,-102,0,-102,0,-102,-102,-102,-102,-102,-102,-102,-102,-102,-102,-102,-102,0,-102,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,0,0,0,0,0,0,0,0,0,0,322,0,0,0,0,309,0,0,0,0,367,0,0,199,0,363,0,0,0,248,0,0,213,180,142,208,195,312,240,0,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-79,0,-79,0,0,-79,0,-79,0,-79,-79,0,-79,0,0,0,0,0,0,0,0,0,0,0,0,0,-79,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-79,0,-79,-79,0,-79,-79,-79,-79,-79,-79,0,-79,-79,0,-79,0,0,0,0,0,-79,0,-79,0,-79,0,-79,0,-79,0,0,-79,-79,-79,-79,0,-79,0,0,-79,0,0,-79,-79,-79,0,0,-79,0,-79,-79,-79,-79,-79,-79,-79,-79,-79,-79,-79,-79,-79,-79,-79,-79,-79,-79,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-163,0,0,132,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-86,-86,-86,0,-86,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-86,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-86,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,-86,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,370,0,181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,228,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-95,-95,-95,0,-95,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-95,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-95,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-95,0,-95,0,0,0,0,0,-95,0,-95,0,-95,0,0,0,0,0,0,0,0,-95,-95,0,0,0,0,-95,0,0,-95,0,-95,0,0,-95,0,-95,0,-95,0,-95,-95,-95,-95,-95,-95,-95,-95,-95,-95,-95,-95,0,-95,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-105,-105,-105,0,-105,0,0,0,0,0,0,0,0,0,0,0,0,0,-105,-105,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-105,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-105,0,-105,0,0,0,0,0,-105,0,-105,0,-105,0,0,0,0,0,0,0,0,-105,-105,0,0,0,0,-105,0,0,-105,0,-105,0,0,-105,0,-105,0,-105,0,-105,-105,-105,-105,-105,-105,-105,-105,-105,-105,-105,-105,0,-105,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-109,-109,-109,0,-109,0,0,0,0,0,0,0,0,0,0,0,0,0,-109,-109,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-109,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-109,0,-109,0,0,0,0,0,-109,0,-109,0,-109,0,0,0,0,0,0,0,0,-109,-109,0,0,0,0,-109,0,0,-109,0,-109,0,0,-109,0,-109,0,-109,0,-109,-109,-109,-109,-109,-109,-109,-109,-109,-109,-109,-109,0,-109,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-233,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-233,0,-233,0,0,0,0,0,-233,0,-233,0,-233,0,0,0,0,0,0,0,0,-233,-233,0,0,0,0,-233,0,0,-233,0,-233,0,0,-233,0,-233,0,-233,0,-233,-233,-233,-233,-233,-233,-233,-233,-233,-233,0,-233,0,-233,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,193,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,161,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-56,0,0,-56,0,0,0,0,0,0,0,0,0,0,0,0,0,-56,-56,0,0,0,-56,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-56,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,378,-56,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-56,0,0,0,0,0,0,0,0,0,0,-56,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,380,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,279,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,0,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-58,0,0,-58,0,0,0,0,0,0,0,0,0,0,0,0,0,-58,-58,0,0,0,-58,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-58,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-58,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-58,0,0,0,0,0,0,0,0,0,0,103,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-14,-14,0,-14,-14,-14,-14,-14,-14,-14,-14,0,0,0,0,0,0,0,0,0,0,-14,-14,-14,-14,-14,-14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-14,0,-14,-14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-14,-14,0,0,0,0,0,0,0,0,0,0,0,-14,0,-14,-14,-14,0,-14,0,-14,0,-14,0,-14,0,-14,0,-14,-14,0,-14,0,0,-14,-14,-14,-14,0,-14,-14,0,0,0,-14,-14,0,-14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-107,-107,-107,0,-107,0,0,0,0,0,0,0,0,0,0,0,0,0,-107,-107,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-107,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-107,0,-107,0,0,0,0,0,-107,0,-107,0,-107,0,0,0,0,0,0,0,0,-107,-107,0,0,0,0,-107,0,0,-107,0,-107,0,0,-107,0,-107,0,-107,0,-107,-107,-107,-107,-107,-107,-107,-107,-107,-107,-107,-107,0,-107,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-226,0,0,0,0,-226,0,-226,0,-226,-226,0,-226,0,0,0,0,0,0,0,0,0,0,0,0,0,-226,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-226,0,-226,-226,0,-226,-226,-226,-226,-226,-226,0,-226,-226,0,-226,0,0,0,-226,0,-226,0,-226,0,-226,0,-226,0,-226,0,0,-226,-226,-226,-226,0,-226,0,0,-226,0,0,-226,-226,-226,0,0,-226,0,-226,-226,-226,-226,-226,-226,-226,-226,-226,-226,-226,-226,-226,-226,-226,-226,-226,-226,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1000000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,0,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,98,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-78,0,-78,0,0,-78,0,-78,0,-78,-78,0,-78,0,0,0,0,0,0,0,0,0,0,0,0,0,-78,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-78,0,-78,-78,0,-78,-78,-78,-78,-78,-78,0,-78,-78,0,-78,0,0,0,0,0,-78,0,-78,0,-78,0,-78,0,-78,0,0,-78,-78,-78,-78,0,-78,0,0,-78,0,0,-78,-78,-78,0,0,-78,0,-78,-78,-78,-78,-78,-78,-78,-78,-78,-78,-78,-78,-78,-78,-78,-78,-78,-78,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-234,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-234,0,-234,0,0,0,0,0,-234,0,-234,0,-234,0,0,0,0,0,0,0,0,-234,-234,0,0,0,0,-234,0,0,-234,0,-234,0,0,-234,0,-234,0,-234,0,-234,-234,-234,-234,-234,-234,-234,-234,-234,-234,0,-234,0,-234,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,320,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,85,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-29,0,0,0,0,-29,0,-29,0,-29,-29,0,-29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-29,0,0,-29,-29,0,0,0,-29,0,0,0,0,0,0,-29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-29,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-19,-19,0,0,-19,-19,-19,-19,-19,0,-19,0,0,0,0,0,0,0,0,0,0,-19,-19,-19,-19,-19,-19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-19,-19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-19,-19,0,0,0,0,0,0,0,0,0,0,0,-19,0,-19,-19,-19,0,-19,0,-19,0,-19,0,-19,0,-19,0,0,-19,0,0,0,0,-19,0,-19,-19,0,-19,-19,0,0,0,-19,-19,0,-19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-81,-81,-81,0,-81,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-81,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-81,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-81,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,270,39,181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,228,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-196,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-196,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-196,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,279,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-133,-133,-133,0,-133,0,0,0,0,0,0,0,0,0,0,0,0,0,-133,-133,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-133,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,233,0,0,0,0,0,0,0,0,0,0,0,0,-133,0,-133,0,0,0,0,0,-133,0,-133,0,-133,0,0,0,0,0,0,0,0,-133,-133,0,0,0,0,-133,0,0,-133,0,-133,0,0,-133,0,-133,0,-133,0,-133,-133,-133,-133,-133,-133,-133,-133,-133,-133,-133,-133,0,-133,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-110,-110,-110,0,-110,0,0,0,0,0,0,0,0,0,0,0,0,0,-110,-110,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-110,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-110,0,-110,0,0,0,0,0,-110,0,-110,0,-110,0,0,0,0,0,0,0,0,-110,-110,0,0,0,0,-110,0,0,-110,0,-110,0,0,-110,0,-110,0,-110,0,-110,-110,-110,-110,-110,-110,-110,-110,-110,-110,-110,-110,0,-110,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-232,0,-232,0,0,0,0,0,-232,0,-232,0,-232,0,0,0,0,0,0,0,0,-232,-232,0,0,0,0,-232,0,0,-232,0,-232,0,0,-232,0,-232,0,-232,0,-232,-232,-232,-232,-232,-232,-232,-232,-232,-232,0,-232,0,-232,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-146,-146,0,0,-146,0,0,0,0,0,0,0,0,0,0,0,0,0,-146,-146,0,-146,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-146,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-146,0,0,0,0,0,0,0,0,0,0,0,0,-146,0,-146,0,0,0,0,0,-146,0,-146,0,-146,0,0,0,0,0,0,0,0,-146,-146,0,0,0,0,-146,0,0,-146,0,-146,0,0,-146,0,-146,0,-146,0,-146,-146,-146,-146,-146,-146,-146,-146,-146,-146,0,-146,0,-146,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,225,282,0,262,294,291,317,318,186,0,321,215,0,201,0,0,0,0,0,175,0,384,0,247,0,385,0,285,0,0,286,287,227,322,0,292,0,0,309,0,0,236,328,367,0,0,199,0,363,323,223,266,248,334,220,213,180,142,208,195,312,240,324,235,319,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-63,0,0,-63,0,0,0,0,0,0,0,0,0,0,0,0,0,-63,-63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,210,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,281,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,169,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,246,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,281,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-28,0,0,0,0,-28,0,-28,0,-28,-28,0,-28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-28,0,0,-28,-28,0,0,0,-28,0,0,0,0,0,0,-28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-28,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,377,0,0,0,0,0,0,0,0,0,0,0,0,0,0,157,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-126,0,0,0,0,0,0,0,0,0,0,0,0,0,124,-126,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,392,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-4,-4,0,-4,-4,-4,-4,-4,-4,-4,-4,0,0,0,0,0,0,0,0,0,0,-4,-4,-4,-4,-4,-4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-4,0,-4,-4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-4,-4,0,0,0,0,0,0,0,0,0,0,0,-4,0,-4,-4,-4,0,-4,0,-4,0,-4,0,-4,0,-4,0,-4,-4,0,-4,0,0,-4,-4,-4,-4,0,-4,-4,0,0,0,-4,-4,0,-4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,394,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-217,0,0,0,0,-217,0,-217,0,-217,-217,0,-217,0,0,0,0,0,0,0,0,0,0,0,0,0,-217,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-217,0,-217,-217,0,-217,-217,-217,-217,-217,-217,0,-217,-217,0,-217,0,0,0,5,0,-217,0,-217,0,-217,0,-217,0,-217,0,0,-217,-217,-217,-217,0,-217,0,0,-217,0,0,-217,-217,-217,0,0,-217,0,-217,-217,-217,-217,-217,-217,-217,-217,-217,-217,-217,-217,-217,-217,-217,-217,-217,-217,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-43,0,0,-43,0,0,-43,0,0,0,0,0,0,0,0,0,0,0,0,0,-43,-43,-43,0,-43,-43,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-43,-43,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-43,-43,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-43,0,-43,0,0,0,-43,0,-43,0,0,-43,0,0,0,0,0,0,102,-43,0,338,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-118,0,0,0,0,0,0,0,0,0,0,-118,0,-118,0,0,0,0,0,-118,0,-118,0,0,0,0,0,0,0,0,0,0,0,-118,0,0,0,0,-118,0,0,0,0,-118,0,0,-118,0,-118,0,0,0,-118,0,0,-118,-118,-118,-118,-118,-118,-118,0,-118,0,-118,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,94,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,394,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-90,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-90,0,158,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,0,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,95,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-62,0,0,-62,0,0,0,0,0,0,0,0,0,0,0,0,0,-62,-62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,241,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-32,-32,0,0,-32,-32,-32,-32,-32,0,-32,0,0,0,0,0,0,0,0,0,0,-32,-32,-32,0,-32,-32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-32,-32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-32,-32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-32,0,-32,0,0,0,-32,0,-32,0,0,-32,0,0,0,0,0,0,-32,-32,0,-32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-206,0,0,0,0,-206,0,-206,0,-206,-206,0,-206,0,0,0,0,0,0,0,0,0,0,0,0,0,-206,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-206,0,-206,-206,0,-206,-206,-206,-206,-206,-206,0,-206,-206,0,-206,0,0,0,-206,0,-206,0,-206,0,-206,0,-206,0,-206,0,0,-206,-206,-206,-206,0,-206,0,0,-206,0,0,-206,-206,-206,0,0,-206,0,-206,-206,-206,-206,-206,-206,-206,-206,-206,-206,-206,-206,-206,-206,-206,-206,-206,-206,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-67,0,0,0,0,-67,0,-67,0,-67,-67,0,-67,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-67,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-67,0,0,-67,-67,0,0,0,-67,0,0,0,0,0,0,-67,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-67,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-122,-122,-122,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-122,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-122,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-122,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-47,0,0,-47,0,0,-47,0,0,0,0,0,0,0,0,0,0,0,0,0,-47,-47,398,0,229,-47,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-47,-47,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-47,-47,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-47,0,-47,0,0,0,400,0,-47,0,0,-47,0,0,0,0,0,0,0,252,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,360,0,0,203,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-71,0,0,0,0,-71,0,-71,0,-71,-71,0,-71,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-71,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-71,0,0,-71,-71,0,0,0,-71,0,0,0,0,0,0,-71,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-71,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-6,-6,0,-6,-6,-6,-6,-6,-6,-6,-6,0,0,0,0,0,0,0,0,0,0,-6,-6,-6,-6,-6,-6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-6,0,-6,-6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-6,-6,0,0,0,0,0,0,0,0,0,0,0,-6,0,-6,-6,-6,0,-6,0,-6,0,-6,0,-6,0,-6,0,-6,-6,0,-6,0,0,-6,-6,-6,-6,0,-6,-6,0,0,0,-6,-6,0,-6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-49,0,0,-49,0,0,-49,0,0,0,0,0,0,0,0,0,0,0,0,0,-49,-49,398,0,229,-49,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-49,-49,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-49,-49,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-49,0,-49,0,0,0,400,0,-49,0,0,-49,0,0,0,0,0,0,0,252,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-60,0,0,-60,0,0,0,0,0,0,0,0,0,0,0,0,0,-60,-60,0,0,0,196,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-60,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-60,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,110,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,-36,0,0,-36,296,-36,-36,-36,0,100,0,0,0,0,0,0,0,0,0,0,-36,-36,-36,0,-36,-36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-36,-36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-36,-36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-36,0,-36,0,0,0,-36,0,-36,0,0,-36,0,0,0,0,0,0,-36,-36,0,-36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-100,-100,-100,0,-100,0,0,0,0,0,0,0,0,0,0,0,0,0,-100,-100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-100,0,-100,0,0,0,0,0,-100,0,-100,0,-100,0,0,0,0,0,0,0,0,-100,-100,0,0,0,0,-100,0,0,-100,0,-100,0,0,-100,0,-100,0,-100,0,-100,-100,-100,-100,-100,-100,-100,-100,-100,-100,-100,-100,0,-100,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-91,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-91,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,250,0,0,-52,0,0,-52,0,0,0,0,0,0,0,0,0,0,0,0,0,-52,-52,0,0,0,-52,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-52,-52,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-52,-52,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-52,0,0,0,0,0,0,0,0,0,0,-52,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-104,-104,-104,0,-104,0,0,0,0,0,0,0,0,0,0,0,0,0,-104,-104,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-104,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-104,0,-104,0,0,0,0,0,-104,0,-104,0,-104,0,0,0,0,0,0,0,0,-104,-104,0,0,0,0,-104,0,0,-104,0,-104,0,0,-104,0,-104,0,-104,0,-104,-104,-104,-104,-104,-104,-104,-104,-104,-104,-104,-104,0,-104,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-124,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-124,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-117,0,0,0,0,0,0,0,0,0,0,-117,0,-117,0,0,0,0,0,-117,0,-117,0,0,0,0,0,0,0,0,0,0,0,-117,0,0,0,0,-117,0,0,0,0,-117,0,0,-117,0,-117,0,0,0,-117,0,0,-117,-117,-117,-117,-117,-117,-117,0,-117,0,-117,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-77,0,0,0,0,0,0,0,0,0,0,0,0,0,-77,-77,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-77,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-77,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-9,-9,0,-9,-9,-9,-9,-9,-9,-9,-9,0,0,0,0,0,0,0,0,0,0,-9,-9,-9,-9,-9,-9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-9,0,-9,-9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-9,-9,0,0,0,0,0,0,0,0,0,0,0,-9,0,-9,-9,-9,0,-9,0,-9,0,-9,0,-9,0,-9,0,-9,-9,0,-9,0,0,-9,-9,-9,-9,0,-9,-9,0,0,0,-9,-9,0,-9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,91,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,90,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,83,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,0,-25,0,-25,0,-25,-25,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,-25,-25,0,0,0,-25,0,0,0,0,0,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,282,0,262,294,291,317,318,186,0,321,0,0,0,0,0,0,0,0,0,0,0,0,0,0,385,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,323,0,266,0,0,0,0,0,0,0,0,0,0,324,0,319,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-139,0,0,0,0,-139,0,-139,-139,-139,-139,-139,-139,0,0,0,0,0,0,0,0,0,0,0,0,-139,-139,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-139,0,-139,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-139,0,0,0,0,0,0,0,0,0,-139,0,-139,0,0,0,0,0,-139,0,-139,0,-139,0,0,0,-139,0,0,-139,-139,-139,-139,0,-139,0,0,-139,0,0,-139,-139,-139,0,0,-139,0,-139,0,-139,0,-139,-139,-139,-139,-139,-139,-139,-139,-139,-139,-139,-139,0,-139,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-2,-2,0,-2,0,-2,-2,-2,-2,-2,-2,0,0,0,0,0,0,0,0,0,0,84,-2,-2,-2,-2,-2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-2,0,0,-2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-2,0,0,0,0,0,0,0,0,0,0,0,0,-2,0,-2,-2,-2,0,-2,0,-2,0,-2,0,-2,0,-2,0,-2,-2,0,-2,0,0,-2,-2,-2,-2,0,-2,-2,0,0,0,-2,-2,0,-2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-30,-30,0,0,-30,-30,-30,-30,-30,0,-30,0,0,0,0,0,0,0,0,0,0,-30,-30,-30,-30,-30,-30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-30,-30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-30,-30,0,0,0,0,0,0,0,0,0,0,0,-30,0,-30,-30,-30,0,-30,0,-30,0,-30,0,-30,0,-30,0,0,-30,0,0,0,0,-30,0,-30,-30,0,-30,-30,0,0,0,-30,-30,0,-30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,41,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-205,0,0,0,0,-205,0,-205,0,-205,-205,0,-205,0,0,0,0,0,0,0,0,0,0,0,0,0,-205,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-205,0,-205,-205,0,-205,-205,-205,-205,-205,-205,0,-205,-205,0,-205,0,0,0,-205,0,-205,0,-205,0,-205,0,-205,0,-205,0,0,-205,-205,-205,-205,0,-205,0,0,-205,0,0,-205,-205,-205,0,0,-205,0,-205,-205,-205,-205,-205,-205,-205,-205,-205,-205,-205,-205,-205,-205,-205,-205,-205,-205,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-3,-3,0,-3,-3,-3,-3,-3,-3,-3,-3,0,0,0,0,0,0,0,0,0,0,-3,-3,-3,-3,-3,-3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-3,0,-3,-3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-3,-3,0,0,0,0,0,0,0,0,0,0,0,-3,0,-3,-3,-3,0,-3,0,-3,0,-3,0,-3,0,-3,0,-3,-3,0,-3,0,0,-3,-3,-3,-3,0,-3,-3,0,0,0,-3,-3,0,-3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-211,0,0,0,0,-211,0,-211,0,-211,-211,0,-211,0,0,0,0,0,0,0,0,0,0,0,0,0,-211,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-211,0,-211,-211,0,-211,-211,-211,-211,-211,-211,0,-211,-211,0,-211,0,0,0,0,0,-211,0,-211,0,-211,0,-211,0,-211,0,0,-211,-211,-211,-211,0,-211,0,0,-211,0,0,-211,-211,-211,0,0,-211,0,-211,-211,-211,-211,-211,-211,-211,-211,-211,-211,-211,-211,-211,-211,-211,-211,-211,-211,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-201,0,0,0,0,-201,0,-201,0,-201,-201,0,-201,0,0,0,0,0,0,0,0,0,0,0,0,0,-201,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-201,0,-201,-201,0,-201,-201,-201,-201,-201,-201,0,-201,-201,0,-201,0,0,0,-201,0,-201,0,-201,0,-201,0,-201,0,-201,0,0,-201,-201,-201,-201,0,-201,0,0,-201,0,0,-201,-201,-201,0,0,-201,0,-201,-201,-201,-201,-201,-201,-201,-201,-201,-201,-201,-201,-201,-201,-201,-201,-201,-201,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-214,0,0,0,0,-214,0,-214,0,-214,-214,0,-214,0,0,0,0,0,0,0,0,0,0,0,0,0,-214,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-214,0,-214,-214,0,-214,-214,-214,-214,-214,-214,0,-214,-214,0,-214,0,0,0,0,0,-214,0,-214,0,-214,0,-214,0,-214,0,0,-214,-214,-214,-214,0,-214,0,0,-214,0,0,-214,-214,-214,0,0,-214,0,-214,-214,-214,-214,-214,-214,-214,-214,-214,-214,-214,-214,-214,-214,-214,-214,-214,-214,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-75,0,0,-75,0,0,0,0,0,0,0,0,0,0,0,0,0,-75,-75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,65,0,0,203,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-92,-92,-92,0,-92,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-92,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-92,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-92,0,-92,0,0,0,0,0,-92,0,-92,0,-92,0,0,0,0,0,0,0,0,-92,-92,0,0,0,0,-92,0,0,-92,0,-92,0,0,-92,0,-92,0,-92,0,-92,-92,-92,-92,-92,-92,-92,-92,-92,-92,-92,-92,0,-92,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-202,0,0,0,0,-202,0,-202,0,-202,-202,0,-202,0,0,0,0,0,0,0,0,0,0,0,0,0,-202,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-202,0,-202,-202,0,-202,-202,-202,-202,-202,-202,0,-202,-202,0,-202,0,0,0,-202,0,-202,0,-202,0,-202,0,-202,0,-202,0,0,-202,-202,-202,-202,0,-202,0,0,-202,0,0,-202,-202,-202,0,0,-202,0,-202,-202,-202,-202,-202,-202,-202,-202,-202,-202,-202,-202,-202,-202,-202,-202,-202,-202,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-150,-150,0,0,-150,0,0,0,0,0,0,0,0,0,0,0,0,0,-150,-150,0,-150,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-150,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-150,0,0,0,0,0,0,0,0,0,0,0,0,-150,0,-150,0,0,0,0,0,-150,0,-150,0,-150,0,0,0,0,0,0,0,0,-150,-150,0,0,0,0,-150,0,0,-150,0,-150,0,0,-150,0,-150,0,-150,0,-150,-150,-150,-150,-150,-150,-150,-150,-150,-150,0,-150,0,-150,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-179,-179,0,0,-179,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-179,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-200,0,0,0,0,-200,0,-200,0,-200,-200,0,-200,0,0,0,0,0,0,0,0,0,0,0,0,0,-200,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-200,0,-200,-200,0,-200,-200,-200,-200,-200,-200,0,-200,-200,0,-200,0,0,0,-200,0,-200,0,-200,0,-200,0,-200,0,-200,0,0,-200,-200,-200,-200,0,-200,0,0,-200,0,0,-200,-200,-200,0,0,-200,0,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,337,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-230,0,0,0,0,-230,0,-230,0,-230,-230,0,-230,0,0,0,0,0,0,0,0,0,0,0,0,0,-230,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-230,0,-230,-230,0,-230,-230,-230,-230,-230,-230,0,-230,-230,0,-230,0,0,0,-230,0,-230,0,-230,0,-230,0,-230,0,-230,0,0,-230,-230,-230,-230,0,-230,0,0,-230,0,0,-230,-230,-230,0,0,-230,0,-230,-230,-230,-230,-230,-230,-230,-230,-230,-230,-230,-230,-230,-230,-230,-230,-230,-230,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,203,0,0,0,0,0,0,0,0,0,0,0,0,0,23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,203,0,0,0,0,0,0,0,0,0,0,0,0,0,0,82,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,370,0,181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,209,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,228,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,80,282,0,262,294,291,317,318,186,0,321,215,0,201,0,0,0,0,0,175,0,384,0,247,0,385,0,285,0,0,286,287,227,322,0,292,0,0,309,0,0,236,328,367,0,0,199,0,363,323,223,266,248,334,220,213,180,142,208,195,312,240,324,235,319,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-219,0,0,0,0,-219,0,-219,0,-219,-219,0,-219,0,0,0,0,0,0,0,0,0,0,0,0,0,-219,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-219,0,-219,-219,0,-219,-219,-219,-219,-219,-219,0,-219,-219,0,-219,0,0,0,-219,0,-219,0,-219,0,-219,0,-219,0,-219,0,0,-219,-219,-219,-219,0,-219,0,0,-219,0,0,-219,-219,-219,0,0,-219,0,-219,-219,-219,-219,-219,-219,-219,-219,-219,-219,-219,-219,-219,-219,-219,-219,-219,-219,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-235,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-235,0,-235,0,0,0,0,0,-235,0,-235,0,-235,0,0,0,0,0,0,0,0,-235,-235,0,0,0,0,-235,0,0,-235,0,-235,0,0,-235,0,-235,0,-235,0,-235,-235,-235,-235,-235,-235,-235,-235,-235,-235,0,-235,0,-235,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-156,-156,0,0,-156,0,0,0,0,0,0,0,0,0,0,0,0,0,-156,-156,0,-156,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-156,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-156,0,0,0,0,0,0,0,0,0,0,0,0,-156,0,-156,0,0,0,0,0,-156,0,-156,0,-156,0,0,0,0,0,0,0,0,-156,-156,0,0,0,0,-156,0,0,-156,0,-156,0,0,-156,0,-156,0,-156,0,-156,-156,-156,-156,-156,-156,-156,-156,-156,-156,0,-156,0,-156,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-147,-147,0,0,-147,0,0,0,0,0,0,0,0,0,0,0,0,0,-147,-147,0,-147,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-147,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-147,0,0,0,0,0,0,0,0,0,0,0,0,-147,0,-147,0,0,0,0,0,-147,0,-147,0,-147,0,0,0,0,0,0,0,0,-147,-147,0,0,0,0,-147,0,0,-147,0,-147,0,0,-147,0,-147,0,-147,0,-147,-147,-147,-147,-147,-147,-147,-147,-147,-147,0,-147,0,-147,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-170,0,0,-170,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,76,0,0,326,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-50,0,0,-50,0,0,-50,0,0,0,0,0,0,0,0,0,0,0,0,0,-50,-50,0,0,0,-50,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-50,-50,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-50,-50,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-50,0,115,0,0,0,0,0,111,0,0,-50,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-165,0,0,-165,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-57,0,0,-57,0,0,0,0,0,0,0,0,0,0,0,0,0,-57,-57,0,0,0,-57,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-57,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,378,-57,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-57,0,0,0,0,0,0,0,0,0,0,-57,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-83,-83,-83,0,-83,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-83,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-83,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-83,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-194,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-194,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,0,-25,0,-25,0,-25,-25,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,217,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,-25,-25,0,0,0,-25,0,0,0,0,0,0,-25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,277,0,0,282,0,262,294,291,317,318,186,0,321,0,0,0,0,0,0,0,0,0,0,0,0,0,0,385,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,323,0,266,0,0,0,0,0,0,0,0,0,0,324,0,319,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-16,0,0,-16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-153,-153,0,0,-153,0,0,0,0,0,0,0,0,0,0,0,0,0,-153,-153,0,-153,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-153,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-153,0,0,0,0,0,0,0,0,0,0,0,0,-153,0,-153,0,0,0,0,0,-153,0,-153,0,-153,0,0,0,0,0,0,0,0,-153,-153,0,0,0,0,-153,0,0,-153,0,-153,0,0,-153,0,-153,0,-153,0,-153,-153,-153,-153,-153,-153,-153,-153,-153,-153,0,-153,0,-153,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-111,-111,-111,0,-111,0,0,0,0,0,0,0,0,0,0,0,0,0,-111,-111,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-111,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-111,0,-111,0,0,0,0,0,-111,0,-111,0,-111,0,0,0,0,0,0,0,0,-111,-111,0,0,0,0,-111,0,0,-111,0,-111,0,0,-111,0,-111,0,-111,0,-111,-111,-111,-111,-111,-111,-111,-111,-111,-111,-111,-111,0,-111,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-85,-85,-85,0,-85,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-85,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-85,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-85,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,175,0,0,0,0,0,0,0,0,0,0,0,0,0,322,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,142,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,393,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-99,-99,-99,0,-99,0,0,0,0,0,0,0,0,0,0,0,0,0,-99,-99,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-99,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-99,0,-99,0,0,0,0,0,-99,0,-99,0,-99,0,0,0,0,0,0,0,0,-99,-99,0,0,0,0,-99,0,0,-99,0,-99,0,0,-99,0,-99,0,-99,0,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,0,-99,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,72,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,349,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,175,0,0,0,0,0,0,0,285,0,0,286,287,0,322,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,71,0,0,0,0,0,0,142,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-168,0,0,-168,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,370,0,181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,228,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,66,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-113,-113,-113,0,-113,0,0,0,0,0,0,0,0,0,0,0,0,0,-113,-113,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-113,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-113,0,-113,0,0,0,0,0,-113,0,-113,0,-113,0,0,0,0,0,0,0,0,-113,-113,0,0,0,0,-113,0,0,-113,0,-113,0,0,-113,0,-113,0,-113,0,-113,-113,-113,-113,-113,-113,-113,-113,-113,-113,-113,-113,0,-113,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,185,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,0,0,0,0,0,0,0,0,0,0,322,0,0,0,0,309,0,0,0,0,367,0,0,199,0,363,0,0,0,248,0,0,213,180,142,208,195,312,240,0,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-55,0,0,-55,0,0,0,0,0,0,0,0,0,0,0,0,0,-55,-55,0,0,0,-55,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-55,230,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-55,-55,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-55,0,0,0,0,0,0,0,0,0,0,-55,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,-172,181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-137,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-137,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,370,0,181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,209,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,228,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-64,0,0,0,0,-64,0,-64,0,-64,-64,0,-64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-64,0,0,-64,-64,0,0,0,-64,0,0,0,0,0,0,-64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-64,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-131,-131,-131,0,-131,0,0,0,0,0,0,0,0,0,0,0,0,0,-131,-131,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-131,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-131,0,-131,0,0,0,0,0,-131,0,-131,0,-131,0,0,0,0,0,0,0,0,-131,-131,0,0,0,0,-131,0,0,-131,0,-131,0,0,-131,0,-131,0,-131,0,-131,-131,-131,-131,-131,-131,-131,-131,-131,-131,-131,-131,0,-131,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-84,-84,-84,0,-84,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-84,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-84,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,247,0,0,0,0,0,0,0,0,227,322,0,0,0,0,309,0,0,236,0,367,0,0,199,0,363,0,223,0,248,334,220,213,180,142,208,195,312,240,-84,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-135,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-135,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-223,0,0,0,0,-223,0,-223,0,-223,-223,0,-223,0,0,0,0,0,0,0,0,0,0,0,0,0,-223,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-223,0,-223,-223,0,-223,-223,-223,-223,-223,-223,0,-223,-223,0,-223,0,0,0,-223,0,-223,0,-223,0,-223,0,-223,0,-223,0,0,-223,-223,-223,-223,0,-223,0,0,-223,0,0,-223,-223,-223,0,0,-223,0,-223,-223,-223,-223,-223,-223,-223,-223,-223,-223,-223,-223,-223,-223,-223,-223,-223,-223,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-116,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-116,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,87,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-191,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-191,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,0,0,0,0,0,0,0,0,0,0,322,0,0,0,0,309,0,0,0,0,367,0,0,199,0,363,0,0,0,248,0,0,213,180,142,208,195,312,240,0,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-158,-158,181,0,-158,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-158,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,175,0,0,0,0,0,0,0,0,0,0,0,0,0,322,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,142,0,0,0,0,-158,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,168,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,160,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,55,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,169,0,0,0,0,0,0,0,0,0,0,0,0,0,0,144,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,246,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,0,0,0,0,285,0,0,286,287,0,322,0,292,0,0,309,0,0,0,328,367,0,0,199,0,363,0,0,0,248,0,0,213,180,142,208,195,312,240,221,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-148,-148,0,0,-148,0,0,0,0,0,0,0,0,0,0,0,0,0,-148,-148,0,-148,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-148,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-148,0,0,0,0,0,0,0,0,0,0,0,0,-148,0,-148,0,0,0,0,0,-148,0,-148,0,-148,0,0,0,0,0,0,0,0,-148,-148,0,0,0,0,-148,0,0,-148,0,-148,0,0,-148,0,-148,0,-148,0,-148,-148,-148,-148,-148,-148,-148,-148,-148,-148,0,-148,0,-148,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,215,0,201,0,0,0,0,0,175,0,384,0,0,0,0,0,285,0,0,286,287,0,322,0,292,0,0,309,0,0,0,328,367,0,0,199,0,363,0,0,0,248,0,0,213,180,142,208,195,312,240,221,235,0,259,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,79,0,0,203,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-20,-20,0,0,-20,-20,-20,-20,-20,0,-20,0,0,0,0,0,0,0,0,0,0,-20,-20,-20,-20,-20,-20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-20,-20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-20,-20,0,0,0,0,0,0,0,0,0,0,0,-20,0,-20,-20,-20,0,-20,0,-20,0,-20,0,-20,0,-20,0,0,-20,0,0,0,0,-20,0,-20,-20,0,-20,-20,0,0,0,-20,-20,0,-20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,203,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-238,0,0,0,0,0,0,0,0,0,0,0,0,-238,0,-238,0,0,0,0,0,-238,0,-238,0,-238,0,0,0,0,0,0,0,0,-238,-238,0,0,0,0,-238,0,0,-238,0,-238,0,0,-238,0,-238,0,-238,0,-238,-238,-238,-238,-238,-238,-238,-238,-238,-238,0,-238,0,-238,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,267,0,0,0,0,224,0,216,0,135,145,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,285,0,0,286,287,0,0,0,292,0,0,0,0,0,0,328,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,221,0,0,0};
