/*
    created by CrescentRose 2021/5/7
    update: 2021/5/13 11:24
    Lexscanner
    Main class

*/
#include"CONSTANT.h"
#include<string>
#include<bits/stdc++.h>
#include<thread>
#include<unordered_map>
#include"Logger.h"
#include"threadpool.hpp"
#include "graph.h"
#include "action.hpp"
#include "RE.h"
#include "NFA.h"
#include "DFA.h"
using namespace std;


inline bool space(char &c){
    return c==0x20||c==0x09;
}

inline bool enter(char &c){
    return c=='\r';
}

inline void trim(string &s) 
{
    if (s.empty()) {
        return ;
    }
    s.erase(0,s.find_first_not_of(" ""\x20""\x09""\n""\r"));
    s.erase(s.find_last_not_of(" ""\x20""\x09""\n""\r") + 1);
}

const string partName[5] = {"unknow","statement part","rule part","function part",""};
enum lexState{
    STATEMENT = 1,RULE,FUNCTIONS
};



class Lex{
private:
    int threadCnt = 8;
    int lineCnt = 1; //line count, for logging the place where error occurs.
    int state = 0;
    vector<string> rawRE;   //store the name of the preDefined RE
    vector<string> targetRE;   //store the RE that we need
    mutex REwr;
    vector<action> Action;
    map<string,string> preDefine;
    string codeBuff;
    string funCodeBuff;
    Logger logger;
    FILE *lexFile;
    ifstream lfile;
    int lexReady;
    lexState _state;

    NFA _NFA;
    DFA _DFA;
    void initAll();
    int checkNewLine();
    void scanStatement();    // todo : exception 
    void copyStatement();
    void eraseComments();
    string readRE();
    void readName_RE();

    void scanRules();
    void readRE_action();
    string readAction();

    void scanAuxiliaryFunction();

    void handlePredefinedStatement();

    void unfoldAllRE();

    void generateAction(fstream &fout,int flag);

public:
    //USE DEFAULT CONSTRUCTOR IS NOT PREFFERED
    Lex(); 
    Lex(string);
    Lex(string,string);
    ~Lex();    
    bool setInputFile(string);
    void start(int flag = 0);
    void print();
    static bool checkFileName(string fileName);
};



