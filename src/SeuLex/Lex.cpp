#include<bits/stdc++.h>
#include<thread>
#include"Logger.h"
#include"threadpool.hpp"
using namespace std;


// transform NFA DFA miniDFA Logger visualGraph
class Lex{
private:
    int lineCnt = 1; //line count, for logging the place where error occurs.
    int state = 0;
    vector<string> rawRE;
    vector<string> stdRE;
    string codeBuff;
    Logger logger;
    FILE *lexFile;
    int lexReady;
    vector<std::thread> threadPool;

    void scanStatement();
    void scanRules();
    void scanAuxiliaryFunction();

public:
    //USE DEFAULT CONSTRUCTOR IS NOT PREFFERED
    Lex(); 
    Lex(string);
    Lex(string,string);
    ~Lex();    
    bool setInputFile(string);
    void start();
    void print();
    static bool checkFileName(string fileName);
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

bool Lex::checkFileName(string fileName){
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

/*
    Initialization ended


*/

/*
    Scanning lex file
*/
void Lex::start(){
    if (!lexReady){
        logger.error("try to start before input file is ready!","Lex::start()");
        return ;
    }
    logger.start("Scanning lex file");
    scanStatement();
    scanRules();
    scanAuxiliaryFunction();
}

void Lex::scanStatement(){
    logger.start("Scanning Statement");
    getline(LexFile,)
    logger.end("Scanning statement");
}

void Lex::scanRules(){
    logger.start("Scanning Rules");

    logger.end("Scanning Rules");
}

void Lex::scanAuxiliaryFunction(){
    logger.start("Scanning AuxiliaryFunctions");

    logger.end("Scanning AuxiliaryFunctions");    
}
/*
    Scanning lex file ended
*/

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