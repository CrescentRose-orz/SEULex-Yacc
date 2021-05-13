/*
    created by CrescentRose 2021/5/7
    update: 2021/5/13 11:24
    Lexscanner
    Main class

*/
#include<string>
#include<bits/stdc++.h>
#include<thread>
#include<unordered_map>
#include"Logger.h"
#include"threadpool.hpp"
using namespace std;
#define fin lfile 

// transform NFA DFA miniDFA Logger visualGraph
const string partName[5] = {"unknow","statement part","rule part","function part",""};
enum lexState{
    STATEMENT = 1,RULE,FUNCTIONS
};
class Lex{
private:
    int lineCnt = 1; //line count, for logging the place where error occurs.
    int state = 0;
    vector<string> rawRE;   //store the name of the preDefined RE
    vector<string> stdRE;   //
    map<string,string> preDefine;
    string codeBuff;
    string funCodeBuff;
    Logger logger;
    FILE *lexFile;
    ifstream lfile;
    int lexReady;
    vector<std::thread> threadPool;
    lexState _state;
    int checkNewLine(string &);
    void scanStatement();    // todo : exception 
    void copyStatement();
    void eraseComments();
    string readRE();
    void readName_RE();
    bool checkSelfDef();

    void scanRules();
    string readAction();
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
    lfile.open(lexFileName,ios::in);
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
    logger.init();
    if (!lexReady){
        logger.error("try to start before input file is ready!","Lex::start()",1);
        return ;
    }
    logger.start("Scanning lex file");
    try{
        cout<<"ok"<<endl;
        scanStatement();
        cout<<"ok2"<<endl;
        scanRules();
        cout<<"ok3"<<endl;
        scanAuxiliaryFunction();
    } catch(exception e){
        logger.error("Exception occured ","parsing lex file",lineCnt);
        logger.close();
        return ;
    }
    logger.close();
    cout<<"got code:";
    cout<<codeBuff<<endl;
    return;
}
/*
 possible status:
 %{

 %}

NAME + RE

%%
*/

int Lex::checkNewLine(string &head){           // todo: head is not required anymore
char c;
streampos _pos = fin.tellg();
    ++lineCnt;
    fin.get(c);
    while (c==' '){fin.get(c);}
    if(!fin.eof()){
        if (c == '%'){
            head += c;
            fin.get(c);
            head += c;
            switch (c){
                case '{':
                    copyStatement();   //todo: check if scanner is in statement part;
                    head.clear();
                    return checkNewLine(head);
                    break;
                case '%':
                    return 0;
                    break;
                default :
                    logger.error("format error ",partName[state],lineCnt);  
                    //todo: throw an exception
                    break;                 
            }
        } else if (c == '/'){
            fin.get(c);
            if (c=='*'){
                logger.customMSG("comments detected");
                eraseComments();
                head.clear();
                return checkNewLine(head);
            } else {
                logger.error("format error ",partName[state],lineCnt);                         
            }

            // todo : throw an exception
        } else {
            head+=c;
            fin.seekg(_pos);
            logger.customMSG("RE detected");
            return 1;
        }
    } else {
        logger.error("file not completed ",partName[state],lineCnt);  
        //todo : throw exception                   
    }
    return 0;
}

void Lex::eraseComments(){                     // lineCnt ok;
    char c;
    logger.start("erase comment");
    fin.get(c);
    while (!fin.eof()){
        if (c == '*'){
            fin.get(c);
            if (c == '/'){
                logger.end("erase comment");
                while(!fin.eof()&&c!='\n'){
                    fin.get(c);
                }
                break;
            } 
        } else {
            if (c=='\n')
                ++lineCnt;
            }
            fin.get(c);
        }
    }

}

void Lex::scanStatement(){    
bool end = 0;
int lineStatus;
string s;
    logger.start("Scanning statement");
    state = STATEMENT;
    lineStatus = checkNewLine(s);
    while(lineStatus){
        readName_RE();
        lineStatus = checkNewLine(s);
    }
    logger.end("Scanning statement");
}

void Lex::copyStatement(){
    char c;
    logger.start("copy statement");
    fin.get(c);
    while (!fin.eof()){
        if (c == '%'){
            fin.get(c);
            if (c == '}'){
                logger.end("copy statement");
                while(!fin.eof()&&c!='\n'){
                    fin.get(c);
                }
                break;
            } else {
                codeBuff+='%';
            }
        } else {
            codeBuff+=c;
            fin.get(c);            
        }
    }

}

// support regular name only      todo: escape.. etc;
void Lex::readName_RE(){
string name,RE;
    fin>>name;
    rawRE.emplace_back(name);
    RE = readRE();
    preDefine[name]=RE;
    cout<<"get "<<RE<<" "<<"for "<<name<<endl;
}

string Lex::readRE(){
string rt;
    fin>>rt;
    return rt;
}
void Lex::scanRules(){
int lineStatus;
string s;
    logger.start("Scanning Rules");
    state = RULE;
    lineStatus = checkNewLine(s);
    while(lineStatus){
        
        lineStatus = checkNewLine(s);
    }
    logger.end("Scanning Rules");
}

void Lex::scanAuxiliaryFunction(){
string tmp;
    logger.start("Scanning AuxiliaryFunctions");
    state = FUNCTIONS;
    fin >> tmp;
    while (!fin.eof()){
        funCodeBuff += tmp;
        fin >> tmp;
    }
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
       // cin>>fileName;
        fileName="lex.l";
        if (fileName == "exit"){
            return 0;
        }
        input = fopen(fileName.c_str(),"r");
        if (input == NULL){
            cout<<"file \""<<fileName <<"\"error"<<endl;
        }
    }
    fclose(input);
Lex lextest(fileName,"testLogger");
    lextest.start();

    return 0;
}