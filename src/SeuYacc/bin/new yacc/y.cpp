#include "y.tab.h"
//#include "lex.yy.c"
#include<iostream>
#include<deque>
#include<stack>
#include<vector>
#include<bits/stdc++.h>
#include<cstring>
typedef int YYSPTYPE;
using std::stack;



class yaccParser{
private:
	std::stack<int> valStack;
public:
	void yyparse(){

	}

};



// The following are from the yacc file
int vTail = 0;
char outputBuff1[5000],outputBuff2[5000];
const char* outputName =NULL;
const char* inputName = NULL;
int debugFlag = 0;
typedef struct _visualToken{
    int id;
    int value; 

}visualToken;

void translateProducer(char in[]){
int i;
char tmp[2] ={'\0','\0'};
	outputBuff1[0]='\0';
	for (i = 0 ; i < strlen(in); ++i){
		switch (in[i]){
			case '<':
				strcat(outputBuff1,"&lt;");
				break;
			case '>':
				strcat(outputBuff1,"&gt;");
				break;
			case '&':
				strcat(outputBuff1,"&amp;");
				break;
			case '\"':
				strcat(outputBuff1,"&quot;");
				break;
			case ' ':
				strcat(outputBuff1,"&emsp;");
				break;
			default:
				tmp[0] = in[i];
				strcat(outputBuff1,tmp);
		}
	} 
}
void translateText(){
int i;
char tmp[2] ={'\0','\0'};
	outputBuff2[0]='\0';
	for (i = 0 ; i < yyleng; ++i){
		switch (yytext[i]){
			case '<':
				strcat(outputBuff2,"&lt;");
				break;
			case '>':
				strcat(outputBuff2,"&gt;");
				break;
			case '&':
				strcat(outputBuff2,"&amp;");
				break;
			case '\"':
				strcat(outputBuff2,"&quot;");
				break;
			case ' ':
				strcat(outputBuff2,"&emsp;");
				break;
			default:
				tmp[0] = yytext[i];
				strcat(outputBuff2,tmp);
		}
	} 
}


int addItem(int c){
	if (c =='#'){
		return 0;
	}
	translateProducer(I2S[c]);
	translateText();
	fprintf(yyout,"\"%d\"[ shape = \"rect\" label =<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\"><tr><td bgcolor=\"grey\"  align = \"center\">Token:%s</td></tr><tr><td align=\"left\">Value:%s</td></tr></table>>];\n",vTail,outputBuff1,outputBuff2); 
	++vTail;
	return 0;
	/*fprintf(yyout,"%d:\"%s\"\n",vTail++,yytext);*/
}
int addEdge(int a,int b){
	fprintf(yyout,"\"%d\" -- \"%d\"\n",a,b);
	return 0;
}
int addRItem(visualToken token){
	translateProducer(I2S[token.value]);
	if (strcmp(I2S[token.value],"translation_unit")==0){
		fprintf(yyout,"\"%d\"[ shape = \"rect\" label =<<table border=\"0\" cellborder=\"5\" cellspacing=\"0\" cellpadding=\"4\"><tr><td bgcolor=\"black\" align = \"center\"><font color = \"white\">Symbol:%s</font></td></tr></table>>];\n",token.id,outputBuff1);	
	} else {
		fprintf(yyout,"\"%d\"[ shape = \"rect\" label =<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\"><tr><td>Symbol:%s</td></tr></table>>];\n",token.id,outputBuff1);	
	}
	++vTail;
}


int valTop = -1;
int sTop = -1;
int sSt[10000];
visualToken valSt[10000];

void yyparse(){
int c ;
int now;
visualToken token,newtoken;
	sSt[++sTop] = 0;
	do{
		c = yyLex();
	}while(c==' ');
	/*printf("get %s for %s\n",I2S[c],yytext);*/
	token.value = c;
	while(1){
		now = sSt[sTop];
		if (debugFlag)
			printf("now at %d\n",now);
		if (ACTION[now][c]==acc){
			if (debugFlag)
				printf("success! \n");
			return ;
		} 
		if (ACTION[now][c]>0){
			token.id = vTail;
			addItem(c);
			valSt[++valTop] = token;
		/*	addItemEdge(token);*/
			if (debugFlag)
				printf("moving for %s \n",I2S[c]);
			sSt[++sTop]=ACTION[now][c]-1;
			if (debugFlag){
				printf("move to %d\n",ACTION[now][c]-1);
				printf("move ok\n");
			}	
			if (yyEOF == 2){
				break; 
			}
			do{
				c = yyLex();
			}while(c==' ');
			token.value = c;
			/*printf("get %s for %s\n",I2S[c],yytext);*/
		} else if (ACTION[now][c]<0) {
			int oldNow = now;
			int newSymbol;
			if (debugFlag){
				printf("reducing\n");
				printf("reduce get %s \n",I2S[proGet[(-ACTION[oldNow][c])-1]]);
			}
			sTop-=proCnt[(-ACTION[oldNow][c])-1];
			newSymbol = proGet[(-ACTION[oldNow][c])-1];
			newtoken.value = newSymbol;
			newtoken.id = vTail;
			addRItem(newtoken);
			for (int i = 0 ; i < proCnt[(-ACTION[oldNow][c])-1];++i){
				/*addEdge(newtoken,valSt[valTop]);*/
				addEdge(newtoken.id,valSt[valTop].id);
				--valTop;
			}
			valSt[++valTop]=newtoken;
			now = sSt[sTop];
			sSt[++sTop] = _GOTO[now][newSymbol-TNBound]; 
		} else {
			if (debugFlag)
				printf("ERROR!!!!\n");
			return;
		}
	}
	if (debugFlag)
		printf("meet end of file!\n");
	return ;
}

void invalidInput(){
	printf("expected input format: \n");
	printf("-debug: ouput debug message to log \n");
	printf("-o fileName[without extension]:	syntax tree output file name,default is the same with inputFile \n");
	printf("-f fileName[with extension]: input file Name\n");
	system("pause");
}

int main(int argc, char const *argv[]){
char temp[100];
	for (int i = 1; i < argc; ++i){
		if (argv[i][0] != '-'){
			printf("expected input flag start with \'-\' but get :%s\n ",argv[i]);
			invalidInput();
			return 1;
		}
		if (strcmp(argv[i],"-debug")==0){
			debugFlag = 1;
			continue;
		}
		if (strcmp(argv[i],"-o")==0){
			if (i < argc){
				outputName = argv[++i];
			}
			continue;
		}
		if (strcmp(argv[i],"-f")==0){
			if (i < argc){
				inputName = argv[++i];
			}
			continue;
		}	
		printf("unexpected argument %s \n",argv[i]);
		invalidInput();
	}
	if (inputName == NULL){
		printf("you must set the input fileName\n!");
		invalidInput();
	}
	if (outputName == NULL){
		outputName = inputName;
	}
	if (debugFlag){
		freopen("debug.log","w",stdout);
	}
	yyin = fopen(inputName,"r");
	if (yyin == NULL){
		fprintf(stderr,"file  %s open failed!!\n",inputName);
	}
	sprintf(temp,"%s.dot",outputName);
	yyout = fopen(temp,"w");
	if (yyin == NULL){
		fprintf(stderr,"file  %s open failed!!\n",inputName);
		return 0;
	}
	if (yyout == NULL){
		fprintf(stderr,"file  %s open failed!!\n",temp);		
		return 0;
	}
	fprintf(yyout,"graph{\n");
	fprintf(stderr,"yacc parsing begin!\n");
	yyparse();
	fprintf(yyout,"}\n");
	fprintf(stderr,"yacc parsing finished!\n");
	fclose(yyout);
	sprintf(temp,"dot %s.dot -Tsvg -o %s.svg",outputName,outputName);
	system(temp);
	sprintf(temp,"dot %s.dot -Tpng -o %s.png",outputName,outputName);
	system(temp);
	if (debugFlag){
		fclose(stdout);
	}
	if (debugFlag){
		fprintf(stderr,"press any key to finished...\n");	
	}
	system("pause");
}
    
    