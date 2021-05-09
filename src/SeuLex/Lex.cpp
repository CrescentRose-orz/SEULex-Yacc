#include<bits/stdc++.h>
#include"Logger.h"
using namespace std;


// transform NFA DFA miniDFA Logger visualGraph
class Lex{
private:
    int lineCnt = 1; //line count, for logging the place where error occurs.
    int state = 0;
    vector<string> rawRE;
    vector<string> stdRE;
    Logger logger;
    void scanStatement();
    void scanRules();
    void scanAuxiliaryFunction();
    FILE *lexFile;

    int lexReady;
public:
    Lex(); //USE DEFAULT CONSTRUCTOR IS NOT PREFFERED
    Lex(string);
    Lex(string,string);
    bool setInputFile(string);
    ~Lex();
    void start();
    void print();
};

/*

For initializing SEULex
    constructor && destructor
*/

bool Lex::setInputFile(string lexFileName){
    if (lexReady){
        cerr<<"lex file already opened!"<<endl;
        return 1;
    }
    lexFile = fopen(lexFileName.c_str(),"r");
    if (lexFile == NULL){
        cerr<<"an error occurs, FILE open failed!"<<endl;
        system("pause");    
        lexReady = 0;
        return 1;
    }
    lexReady = 1;
    return 0;
}

Lex::Lex():logger(){
    lexReady = 0;
}

Lex::Lex(string lexFileName,string loggerFileName):logger(loggerFileName){
    lexReady = 0;
    setInputFile(lexFileName);
}

Lex::Lex(string loggerFileName):logger(loggerFileName){
    lexReady = 0;
}

Lex::~Lex(){
    fclose(lexFile);
    logger.close();
}

/*
    Initialization ended


*/

void Lex::start(){
    if (!lexReady){
        logger.error("try to start before input file is ready!");
        return ;
    }
}

bool checkFileName(string fileName){
    if (fileName.substr(fileName.find_last_of('.'))!=string(".l")){
        cerr<<fileName<<" is not a valid lex file!"<<endl;
        return 0;
    } 
    FILE *tmp;
    if ((tmp = fopen(fileName.c_str(),"r")) == NULL){
        cerr<<"cannot open file "<<fileName<<endl;
        return 0;
    }
    fclose(tmp);
    return 1;
}

int main(){
string fileName;
FILE *input = NULL;
    while (input == NULL){
        cout<<"Please enter fileName(end with .l) , or exit with EXIT"<<endl;
        cin>>fileName;
        if (fileName == "exit"){
            return 0;
        }
        input = fopen(fileName.c_str(),"r");
        if (input == NULL){
            cout<<"file \""<<fileName <<"\"error"<<endl;
        }
    }



}