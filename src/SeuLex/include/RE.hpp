#ifndef RE_H
#define RE_H
#include<bits/stdc++.h>
using namespace std;
class RE_operator{
public:
    char op;
    int l,r;
    RE_operator(char c){
        op = c;
        l = 0,r = 0;
    }
    RE_operator(char c,int l,int r){
        op = c;
        this->l = l;
        this->r = r;
    }
    // operator int(){
    //     return (int)op;
    // }
    operator char(){
        return op;
    }
};



class RE{
public:
    static char transChar[256];   
    static int newPri[256];
    static int oldPri[256];
    //static void buildGraph(vector<string> &,vector<string,string> &);
    //static void checkCircle();
    static char trans(int &idx,string s){
        ++idx;
        if (idx < s.size()){
            throw invalid_argument("char expected after '\\");
        }
        return transChar[s[idx]];
    }
    static char trans(char c){
        return transChar[c];
    }
    static int skipFirst(string & newRE,const string &raw,int &idx,map<string,string> &preDefine){
        int rt = 0;
        cout<<"skipping first start at"<< idx<<raw[idx]<<endl;
        if (raw[idx] == '^'){
            if (idx){
                string tmp = "'^' should be at the begin of  a line in" + raw;
                throw invalid_argument(tmp);
            }
            ++idx;
            if (idx == raw.size()){
                throw invalid_argument("the RE statement after '^' shouldn't be empty in " + raw);
            }
        }
        switch(raw[idx]){
            case '[':{
                    int tail = idx;
                    while (tail < raw.size()&&raw[tail] != ']'){
                        ++tail;
                    }
                    if (tail == raw.size()){
                        throw invalid_argument(" missing ']' expected in " + raw);
                    }
                    newRE += raw.substr(idx,tail - idx + 1);
                    idx = tail + 1;
                }
                break;
            case '{':{
                    int tail = idx;
                    while (tail < raw.size()&&raw[tail] != '}'){
                        ++tail;
                    }
                    if (tail == raw.size()&& raw[raw.size()-1] != '}'){
                        throw invalid_argument(" missing '}' expected in " + raw);
                    }
                    string &&name = raw.substr(idx + 1,tail - idx - 1);
                    idx = tail + 1;
                    if (preDefine.count(name)){
                        newRE +='(';
                        newRE += preDefine[name];
                        newRE +=')';
                    } else {
                        throw invalid_argument(name + "not found int predefined statement in" + raw );
                    }
                }
                break;
            case '\"':{
                    cout<<"\""<<" detected"<<endl;
                    int tail = idx + 1;
                    while (tail < raw.size()&&raw[tail] != '\"'){
                        ++tail;
                    }
                    if (tail == raw.size() && raw[raw.size()-1] != '\"'){
                        cout<<raw.size()<<" "<<tail<<endl;
                        throw invalid_argument(" missing '\"' expected in " + raw);
                    }
                    newRE += raw.substr(idx,tail - idx + 1);
                    idx = tail + 1;
                }
                break;
            case '(':{
                    ++idx;
                    newRE += '(';
                    rt += 1 + skipFirst(newRE,raw,idx,preDefine);
                }
                break;            
            case '?':
            case '|':
            case '+':
            case '*':
            case '$':
            case ')':
            case ']':
            case '}':
           // case '.'
                throw invalid_argument("char expected before operator" + raw[idx]);
                break;
            default:
                cout<<"get default"<<endl;
                if (raw[idx] =='\\'){
                    newRE +='\\';
                    newRE += raw[++idx];
                    ++idx;
                    //newRE+=(trans(idx,raw));
                } else {
                    newRE+=raw[idx++];
                }
        }
        cout<<"ended at "<< idx<<raw[idx]<<endl;
            return rt;
    }
    static string unfoldRE(const string &raw,map<string,string> &preDefine){
        string newRE;
        int last = 0,pos,tail,Ql,Qr = 0;
        int idx = 0;
        bool trans = false;
        bool brace = false, bracket = false;
        int  parenthese = skipFirst(newRE,raw,idx,preDefine);
        cout<<"now idx "<<idx<<" "<<raw.size()<<endl;
        while (idx < raw.size()){
            if (trans){
                newRE += raw[idx++];
                trans = 0;
                continue;
            }
            if (brace){
                if (raw[idx] == '"'){
                    brace =false;
                }                
                newRE += raw[idx++];
                continue;
            } 
            if (bracket){
                if (raw[idx] == ']'){
                    bracket =false;
                }                
                newRE += raw[idx++];
                continue;
            }
            switch (raw[idx]){
                case '{':
                    if (raw.at(idx+1)<='9'&&0<=raw.at(idx+1)){
                        newRE += raw[idx];
                        while (raw[idx]!='}'){
                            if (++idx<raw.size()){
                                newRE += raw[idx];
                            } else {
                                throw invalid_argument("'}' expected in "+raw);
                            }
                        }
                    } else {
                        // append
                        string name;
                        int pos;
                        newRE += '^';
                        if ((pos = raw.find('}',idx)) == string::npos){
                            string tmp(" '}' expected in ");
                            tmp += raw;
                            throw invalid_argument(tmp);
                            //throw invalid_argument("} expecte")
                        }
                        name = raw.substr(idx+1,pos-idx-1);
                        if (!preDefine.count(name)){
                            string tmp("undefined identifier ");
                            tmp += name;
                            tmp +=" occurs in RE ";
                            tmp += raw;
                            throw invalid_argument(tmp);                        
                        }
                        newRE +='(';
                        newRE .append(preDefine[name]);
                        newRE +=')';
                        idx = pos + 1;
                    }
                    break;
                case '[':
                    newRE += "^[";
                    bracket = true;
                    ++idx;
                    break;
                case '"':
                    newRE +="^\"";
                    brace = true;
                    ++idx;
                    break;
                case '\\':
                    newRE +="^\\";
                    trans = true;
                    ++idx;
                    break;
                case '*':
                    newRE += '*';
                    ++idx;
                    break;
                case '?':
                    newRE +='?';
                    ++idx;
                    break;
                case '$':
                    newRE +='$';
                    ++idx;
                    break;
                case '(':
                    newRE +="^(";
                    ++idx;
                    parenthese += skipFirst(newRE,raw,idx,preDefine);
                    ++parenthese;
                    break;
                case ')':
                    newRE +=')';
                    ++idx;
                    --parenthese ;
                    cout<<") detected "<<endl;
                    break;
                case '|':
                    newRE +='|';
                    ++idx;
                    parenthese += skipFirst(newRE,raw,idx,preDefine);               
                    break; 
                case '+':
                    newRE +='+';
                    ++idx;
                    break;
                default:
                    cout<<"ex default at "<<idx<<raw[idx]<<endl;
                    newRE += '^';
                    newRE += raw[idx++];
            }
        }
        if (bracket){
            string tmp(" ']' expected in ");
            tmp += raw;
            throw invalid_argument(tmp);
        }
        if (brace){
            string tmp(" '\"' expected in ");
            tmp += raw;
            throw invalid_argument(tmp);
        }
        if (parenthese){
            cout<<newRE<<endl;
            cout<<parenthese<<endl;
            string tmp(" ) expected in ");
            tmp += raw;
            throw invalid_argument(tmp);

        }
        return newRE;
    }                                                       
    static void init(){
        for (int i = 0; i < 256; ++i){
            transChar[i] = i;
        }
        transChar['n'] = '\n';
        transChar['r'] = '\r';
        memset(newPri,0,sizeof(newPri));
        memset(oldPri,0,sizeof(newPri));    
        newPri['|'] = 1;    
        newPri['^'] = 3;
        newPri['+'] = newPri['*'] = newPri['?'] = 5;
        newPri['-'] = newPri['$'] = -1;
        oldPri['|'] = 2;    
        oldPri['^'] = 4;
        oldPri['+'] = oldPri['*'] = oldPri['?'] = 6;

      //  transChar['\\'] = '\\';
      //  memset(newPri,-1,sizeof(newPri));
      //  memset(oldPri,-1,sizeof(oldPri));
        
    }
};

char RE::transChar[256];   
int RE::newPri[256];
int RE::oldPri[256];

#endif