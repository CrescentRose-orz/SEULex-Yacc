#include<iostream>
#include<string>
#include"y.tab.h"
using std::cin;
using std::string;
using std::endl;
using std::cout;
int yyLex();
extern FILE* yyin;
extern int yyEOF;
int main(){
string fileName("test.txt");
	yyin = NULL;
	cout<<"please input fileName"<<endl;
//	cin>>fileName;
	yyin = fopen(fileName.c_str(),"r");
	while(yyin == NULL){
		cout<<"file open failed ,please retype fileName:"<<endl;
		cin>>fileName;
		yyin = fopen(fileName.c_str(),"r");
	}
	while (!yyEOF){
		cout<<"get token:"<<I2S[yyLex()];
		cout<<"        for text:\""<<yytext<<"\""<<endl;
		system("pause");
	}
}
