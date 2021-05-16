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
#include "graph.h"
#include "action.hpp"
#include "RE.hpp"
using namespace std;
#define fin lfile 

// transform NFA DFA miniDFA Logger visualGraph

inline bool space(char &c){
    return c==0x20||c==0x09;
}

inline bool enter(char &c){
    return c=='\r';
}

const string partName[5] = {"unknow","statement part","rule part","function part",""};
enum lexState{
    STATEMENT = 1,RULE,FUNCTIONS
};



class Lex{
private:
    int lineCnt = 1; //line count, for logging the place where error occurs.
    int state = 0;
    vector<string> rawRE;   //store the name of the preDefined RE
    vector<string> targetRE;   //store the RE that we need
    vector<action> Action;
    map<string,string> preDefine;
    string codeBuff;
    string funCodeBuff;
    Logger logger;
    FILE *lexFile;
    ifstream lfile;
    int lexReady;
    lexState _state;
    threadpool threadPool;

    int checkNewLine(string &);
    void scanStatement();    // todo : exception 
    void copyStatement();
    void eraseComments();
    string readRE();
    void readName_RE();
    void handlePredefinedStatement();

    void scanRules();
    void readRE_action();
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
    lfile.open(lexFileName,ios::binary);
        if (!lfile.is_open()){
            cerr<<"an error occurs, FILE open failed!"<<endl;
            system("pause");    
            lexReady = 0;
            return 1;
        }
    lexReady = 1;
    return 0;
}

Lex::Lex():logger(),threadPool(8){
    lexReady = 0;
}

Lex::Lex(string lexFileName,string loggerFileName):logger(loggerFileName),threadPool(8){
    lexReady = 0;
    setInputFile(lexFileName);
}

Lex::Lex(string loggerFileName):logger(loggerFileName),threadPool(8){
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
    RE::init();
    if (!lexReady){
        logger.error("try to start before input file is ready!","Lex::start()",1);
        return ;
    }
    logger.start("Scanning lex file");
    try{
        scanStatement();
       // threadPool.commit(std::bind(&Lex::handlePredefinedStatement,this));
        handlePredefinedStatement();
        scanRules();
        scanAuxiliaryFunction();
        fin.close();
        cout<<"file close ok"<<endl;
    }catch (invalid_argument e){
        logger.customMSG(e.what());
        cerr<<e.what()<<endl;
        logger.error("invalid input, program ended ","parsing lex file",lineCnt);
        logger.close();
        return ;
    } 
    catch(exception e){
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
    //++lineCnt;
    fin.get(c);
    while (!fin.eof()&&(space(c)||c=='\r'||c=='\n')){
        _pos = fin.tellg();
        if (c=='\n') ++lineCnt;
        fin.get(c);
    }
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

            // todo : throw an exceptiond
        } else {
            head+=c;
            // cout<<"now "<<fin.tellg()<<" to "<<_pos<<endl;
            //cout<<"found "<<c<<"with head"<<head<<endl;
            fin.seekg(_pos);
            logger.customMSG("RE detected");
            // cout<<"will read "<<(char)fin.get()<<endl;
            // cout<<"now "<<fin.tellg()<<endl;
            return 1;
        }
    } else {
        logger.error("file not completed ",partName[state],lineCnt);  
        //todo : throw exception                   
    }
    return 0;
}

void Lex::eraseComments(){                     // lineCnt ok;
    string tmp;
    char c;
    logger.start("erase comment");
    fin.get(c);
    tmp+=c;
    while (!fin.eof()){
        if (c == '*'){
            fin.get(c);
            tmp+=c;
            if (c == '/'){
                logger.end("erase comment");
                while(!fin.eof()&&c!='\n'){
                    fin.get(c);
                    tmp+=c;
                }
                ++lineCnt;
                logger.customMSG(tmp);
                break;
            } 
        } else {
            if (c=='\n'){
                ++lineCnt;
            }
            fin.get(c);
            tmp+=c;
        }
    }
    logger.customMSG(tmp);
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
      //  system("pause");       
        s.clear();
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
                string tmp;
                while(!fin.eof()&&c!='\n'){
                    fin.get(c);
                    tmp+=c;
                }
                tmp+=" is wasted";
                logger.customMSG(tmp);
                return ;
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
/*
    update:2021/5/17 0:11
        escape ok
        todo: exceptions
*/
void Lex::readName_RE(){
string name;
    char c = fin.get();
    while (space(c)){
        c = fin.get();
    }
    while (!space(c) &&c!='\r'&&c!='\n'){
        name += c;
        //cout<<"appending "<<c<<" ascll "<<(short)c<<endl;
        c = fin.get();
    }
    rawRE.emplace_back(name);
    string &&RE = readRE();
    preDefine[name] = RE;
    cout<<"get "<<preDefine[name]<<" "<<"for "<<name<<endl;
    while (c !='\n'){
        c = fin.get();
    }
}

string Lex::readRE(){
string rt;
int flag1 = 0,flag2 = 0,flag3 = 0,flag = 0; // ( [ {
bool tran = 0; 
    char c = fin.get();
    while (space(c)){
        c = fin.get();
    }
    while (!space(c)&&c!='\r' || flag || flag1 || flag2 || flag3){
        if (tran){
            tran = 0;
            rt += RE::trans(c);
            c = fin.get();
            continue;
        }
        rt += c;
        /*
            [] > "" 

            todo : {content} 
            content:
                {a,b} a,b is integer 
                {NAME}
                otherwise : exception
        */
        if (flag2){
            if (c == ']'){
                --flag2;
            }
            c = fin.get();
            continue;
        }
        if (c == '"'){
            flag ^= 1;
        }
        if (!flag){
            switch (c){
                case '{':
                    ++flag3;
                    break;
                case '}':
                    --flag3;
                    break;
                case '[':
                    ++flag2;
                    break;
                case ']':
                    --flag2;
                    break;
                case '(':
                    ++flag1;
                    break;
                case ')':
                    --flag1;
                    break;
                case '\\':
                    tran = 1;
                    break;
            }
        }
        c = fin.get();
    }
    return rt;
}

void Lex::handlePredefinedStatement(){
    // map<string,string>::iterator iter = preDefine.begin();
    // for (;iter!=preDefine.end();++iter){

    // }
    mappingGraph<string> g;
    for (auto &s:rawRE){
        g.add_node(s);
        for (auto &target:rawRE){
            if (s == target){
                continue;
            }
            cout<<s<<" "<<target<<" processing"<<endl;
            cout<<preDefine[s]<<" "<<preDefine[target]<<endl;
            int idx = preDefine[s].find(target);
            while(idx != string::npos){
                cout<<"possible match!"<<endl;
                if (idx == 0 || preDefine[s][idx-1]!='{' || preDefine[s][idx+target.length()] !='}'){ // todo: may be fooled when meet {{}} ( illegal )
                    idx = preDefine[s].find(target, idx + 1 );
                    continue;
                }
                g.add_edge(target,s);
                cout<<s<<" need "<<target<<" to be done first!"<<endl;
                idx = s.find(target, idx + 1) ;
            }
            cout<<s<<" "<<target<<" ok!"<<endl;
        }
    }
    bool hasCircle;
    vector<string> &&order = g.topSort(hasCircle);
    cout<<"topSort ok"<<endl;
    if (hasCircle){
        cerr<<" circle found!"<<endl;
        logger.error("self define problem found","statement processing",lineCnt);       //todo: lineCnt is not correct
        throw invalid_argument("self define problem found");
    }
    cout<<"prefered order"<<": ";
    for (auto &s:order){
        cout<<s<<" "<<endl;
    }
    cout<<endl;
    return ;
}

void Lex::scanRules(){
int lineStatus;
string s;
    logger.start("Scanning Rules");
    state = RULE;
    lineStatus = checkNewLine(s);
    while(lineStatus){
        readRE_action();
        lineStatus = checkNewLine(s);
    }
    logger.end("Scanning Rules");
}

void Lex::readRE_action(){
string &&RE = readRE(),act;
char c;
int cnt = 0;    
    targetRE.emplace_back(RE);

    c = fin.get();
    while (space(c)){
        c = fin.get();
    }
    if (c == '\n' || c == '\r'){
        throw invalid_argument("Error : get end of line when scaning actions\n");
    }
    if (c != '{'){
        cerr<<"get RE"<<RE<<endl;
        string s ="Error : '{' not found when scaning actions\n instead get ";
        s += c;
        s += '\n';
        throw invalid_argument(s);        
    }
    cnt = 0;
    c = fin.get();
    while (c!='}'||cnt){
        if (!space(c)){
           if (c!='\n'){
               if (c!='\r'){
                    act += c;
                    if (c == '"'){
                        cnt ^= 1;
                    }
               }               
           } else{
               ++lineCnt;
           }
        }
        c = fin.get();
    }
    //act = act.substr(0,act.size()-1);
    Action.emplace_back(Action.size(),act);
    cout<<"get "<<RE<<" "<<"for "<<act<<endl;
    while (c !='\n'){
        c = fin.get();
    }    
}

void Lex::scanAuxiliaryFunction(){
string tmp;
char buff[1000];
    logger.start("Scanning AuxiliaryFunctions");
    state = FUNCTIONS;
    fin.getline(buff,1000);
    while (!fin.eof()){
        funCodeBuff += tmp;
        fin.getline(buff,1000);
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
Lex lextest(fileName,"testLogger.txt");
    lextest.start();
    system("pause");
    return 0;
}