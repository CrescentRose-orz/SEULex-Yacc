/*
    created by CrescentRose 2021/5/7
    update: 2021/5/13 11:24
    Lexscanner
    Main class

*/
#include"Lex.h"

/*

For initializing SEULex
    constructor && destructor
*/
void Lex::initAll(){
    RE::init();
    logger.init();
    _NFA.logger = logger;
    _DFA.logger = logger;
}
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

Lex::Lex():logger(),_NFA(logger),_DFA(logger){
    lexReady = 0;
}

Lex::Lex(string lexFileName,string loggerFileName):logger(loggerFileName),_NFA(logger),_DFA(logger){
    lexReady = 0;
    setInputFile(lexFileName);
}

Lex::Lex(string loggerFileName):logger(loggerFileName),_NFA(logger),_DFA(logger){
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
//shared_timed_mutex a;
/*
    Initialization ended


*/

/*
    Scanning lex file
*/

int Lex::checkNewLine(){       
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
            fin.get(c);
            switch (c){
                case '{':
                    copyStatement();   //todo: check if scanner is in statement part;
                    return checkNewLine();
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
                #ifdef DEBUG
                logger.customMSG("comments detected");
                #endif
                eraseComments();
                return checkNewLine();
            } else {
                logger.error("format error ",partName[state],lineCnt);                         
            }

            // todo : throw an exceptiond
        } else {
            fin.seekg(_pos);
            //logger.customMSG("RE detected");
            return 1;
        }
    } else {
        logger.error("file not completed ",partName[state],lineCnt);  
        throw invalid_argument("lex file not completed! %%expected");                  
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
                #ifdef DEBUG
                logger.customMSG(tmp);
                #endif
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
    #ifdef DEBUG
    logger.customMSG(tmp);
    #endif
}

void Lex::scanStatement(){    
bool end = 0;
int lineStatus;
    logger.start("Scanning statement");
    state = STATEMENT;
    lineStatus = checkNewLine();
    while(lineStatus){
        readName_RE();  
        lineStatus = checkNewLine();
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
        c = fin.get();
    }
    rawRE.emplace_back(name);
    string &&RE = readRE();
    preDefine[name] = RE;
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

void Lex::scanRules(){
int lineStatus;
    logger.start("Scanning Rules");
    state = RULE;
    lineStatus = checkNewLine();
    while(lineStatus){
        readRE_action();
        lineStatus = checkNewLine();
    }
    logger.end("Scanning Rules");
}
void Lex::readRE_action(){
string &&RE = readRE(),act;
char c,lastc;
int qcnt = 0,ccnt = 0;    
    targetRE.emplace_back(RE);
    c = fin.get();
    while (space(c)){
        c = fin.get();
    }
    lastc= 0;
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
    qcnt = ccnt = 0;
    c = fin.get();
    while ((c!='}')||qcnt||ccnt){
        if (!space(c)){
           if (c!='\n'){
               if (c!='\r'){
                    act += c;
                    if (c == '"'){
                        qcnt ^= 1;
                    }
                    if (c == '\''){
                        ccnt ^= 1;
                    }
               }               
           } else{
               ++lineCnt;
           }
        }
        lastc = c;
        c = fin.get();
    }
    trim(act);
    Action.emplace_back(Action.size() + 1,act);
    while (c !='\n'){
        c = fin.get();
    }    
}

void Lex::scanAuxiliaryFunction(){
    logger.start("Scanning AuxiliaryFunctions");
    state = FUNCTIONS;
    funCodeBuff.clear();
    char c = fin.get();
    while (!fin.eof()){
        funCodeBuff += c;
        c = fin.get();
    }
    cout<<endl;
    logger.end("Scanning AuxiliaryFunctions");    
}
/*
    Scanning lex file ended
*/

void Lex::handlePredefinedStatement(){
    logger.start("handlePredefinedStatement");
    mappingGraph<string> g;
    logger.start("build graph");
    for (auto &s:rawRE){
        g.add_node(s);
        for (auto &target:rawRE){
            if (s == target){
                continue;
            }
            int idx = preDefine[s].find(target);
            while(idx != string::npos){
                if (idx == 0 || preDefine[s][idx-1]!='{' || preDefine[s][idx+target.length()] !='}'){ // todo: may be fooled when meet {{}} ( illegal )
                    idx = preDefine[s].find(target, idx + 1 );
                    continue;
                }
                g.add_edge(target,s);
                idx = s.find(target, idx + 1) ;
            }
        }
    }
    logger.end("build graph");
    logger.start("topsort");
    bool hasCircle;
    vector<string> &&order = g.topSort(hasCircle);
    if (hasCircle){
        cerr<<" circle found!"<<endl;
        logger.error("self define problem found","statement processing",lineCnt);       //todo: lineCnt is not correct
        throw invalid_argument("self define problem found");
    }
    for (auto &s:order){
        string raw = preDefine[s];
        string &&newRE = RE::unfoldRE(raw,preDefine);
        preDefine[s] = newRE;
        #ifdef DEBUG
        logger.customMSG(raw);
        logger.customMSG(newRE);
        #endif
    }
    logger.end("topsort");
    logger.end("handlePredefinedStatement");
    return ;
}

void Lex::unfoldAllRE(){

    logger.start("unfold all RE");
    #ifdef USE_MULTITHREAD

    {
    threadpool tp(threadCnt);
    #endif
    for (int i = 0; i < targetRE.size(); ++i){
        string s = targetRE[i];
        #ifdef USE_MULTITHREAD
        tp.commit(bind([this](int i,string s){
            string &&rt = RE::unfoldRE(s,this->preDefine);
            //cout<<"got result "<<rt<<endl;
            std::unique_lock<std::mutex> lock{ this->REwr};
            this->targetRE[i] = rt;
        },i,s));
        #else
        string &&rt = RE::unfoldRE(s,preDefine);
        targetRE[i] = rt;
        #ifdef DEBUG
        logger.customMSG("unfold :" + rt);
        #endif
        #endif
    }
    #ifdef USE_MULTITHREAD
    }
    #endif
    #ifdef DEBUG
    for (auto &s:targetRE){
        logger.customMSG(s);
    }
    #endif
    logger.end("unfold all RE");
}

void Lex::generateAction(fstream &fout,int flag){
    fout<<endl;
    if (flag){
        fout<<"int doAction(int idx){"<<endl;
    }else{
    fout<<"     int idx = actIdx;"<<endl;
    }
    fout<<R"(
            switch(idx){)"<<'\n';
    for (int i = 0; i <Action.size();++i){
        fout<<R"(
            case )"<<i + 1<<":\n";
        fout<<"                 {"<<Action[i].getCode()<<"}\n";
        fout<<"                 break;";
    }
    fout<<"\n        }";
    if (!flag){
        fout<<"\n   return yyLex();";
        fout<<"\n       }";
    }
    fout<<"\n}";
}

void Lex::start(int flag){
    threadpool threadPool;    
    initAll();
    logger.start("main");
    if (!lexReady){
        logger.error("try to start before input file is ready!","Lex::start()",1);
        return ;
    }
    logger.start("Scanning lex file");
    try{
        logger.start("readin&prepare");
        scanStatement();
        #ifdef USE_MULTITHREAD
            handlePredefinedStatement();
        //    threadPool.commit(std::bind(&Lex::handlePredefinedStatement,this));
        #else
            handlePredefinedStatement();
        #endif
        scanRules();
        scanAuxiliaryFunction();
        fin.close();
        cout<<"file close ok"<<endl;
        unfoldAllRE();
        cout<<"unfold ok,start with RE2NFA"<<endl;
        logger.end("readin&prepare");
        logger.start("create NFA with REs");
        logger.save();
        for (int i = 0; i < targetRE.size();++i){
            _NFA.addRE(targetRE[i],Action[i]);
        }
        logger.end("create NFA with REs");
        cout<<"NFA created!"<<endl;
        fstream fout;
        fout.open("NFA.dot",ios::out);
        _NFA.vFA.print(fout);
        logger.start("create DFA with NFA");
        _DFA.NFA2DFA(_NFA);
        logger.end("create DFA with NFA");
        {
            stringstream ss;
            ss<<"total "<<_DFA.pool.size()<<" nodes"<<endl;
            logger.customMSG(ss.str());
        }
        fout.open("DFA.dot",ios::out);

        logger.start("minimizing dfa");
        DFA &&miniDFA = _DFA.minimize();
        stringstream ss;
        ss<<"minimzed DFA:"<<miniDFA.pool.size()<<" nodes";
        logger.customMSG(ss.str());
        logger.end("minimizing dfa");
        miniDFA.vFA.print(fout);
        logger.start("Generating code");
        fout.open("lex.yy.c",ios::out);
        if (flag){
            fout<<R"(
#define return(x) printf(#x)
        )";
        } else {
            fout<<R"(
#include"y.tab.h"
            )";
        }
        fout<<R"(
#include <string.h>
#define ECHO printf("%s",yytext)
#define error(x) printf(x)
#define INF 0x7fff;
)"<<codeBuff<<R"(
int yyleng;
char yytext[1024];
FILE* yyin;
FILE* yyout;
long yypos;
int yyEOF;

int input(){
    int c = fgetc(yyin);
    ++yypos;
    if (c == -1)
        return 0;
    return c;
}

)"<<endl;
        trim(funCodeBuff);
        fout<<funCodeBuff<<endl;
        fout<<R"(/*----function defined by lex.l*/)"<<endl;
        if (flag){
            generateAction(fout,flag);
        }
        //_DFA.generateCode(fout,flag);
        //for (auto iter = miniDFA[0].stateBegin();iter!=miniDFA[0].stateEnd();++iter){
        //     cout<<"edge "<<iter->first<<" to "<<iter->second<<endl;
        // }
        // cout<<miniDFA[0].act.getIdx();
        // system("pause");
        miniDFA.generateCode(fout,flag);
        if (!flag){
            generateAction(fout,flag);
        } 
        if (flag){
            fout<<R"(
int main(){
    int i;
    char name[1000];
    printf("input file name:\n");
    scanf("%s",name); 
    yyin = fopen(name,"r");
    while (yyEOF != 1){
        yyLex();
        printf(" get: %s\n",yytext);                   
    }

    return 0;
}
            )"<<endl;
        }
        fout.close();
        logger.end("Generating code");       
        logger.end("main"); 
    }catch (invalid_argument e){
        logger.customMSG(e.what());
        cerr<<e.what()<<endl;
        logger.error("invalid input, program ended ","parsing lex file",lineCnt);
        logger.close();
        fstream fout;
        fout.open("NFA.dot",ios::out);
        _NFA.vFA.print(fout);
            fout.open("DFA.dot",ios::out);
        _DFA.vFA.print(fout);
        return ;
    } 
    catch(exception e){
        logger.save();
        cerr<<"unknow exception occurs"<<endl;
        system("pause");
        fstream fout;
        fout.open("output.dot",ios::out);
        _NFA.vFA.print(fout);
        logger.error("Exception occured ","parsing lex file",lineCnt);
        logger.customMSG(e.what());
        logger.close();
        return ;
    }
    #ifdef USE_MULTITHREAD

    #endif
    logger.save();
    logger.close();
    system("pause");
    return;
}


