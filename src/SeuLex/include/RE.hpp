#include<bits/stdc++.h>

using namespace std;

/*
。 * 【】 ^ $ {} \ + ? | "" / {}
.[] {name} \ "" 
* $ ^ {}
*/
class RE{
public:
    static char transChar[256];   
    static int Newpri[256];
    static int Oldpri[256];
    //static void buildGraph(vector<string> &,vector<string,string> &);
    //static void checkCircle();
    static char trans(char a){
        return transChar[a];
    }
    static string unfoldRE(const string &raw,map<string,string> &preDefine){
        string newRE;
        int last = 0,pos,tail,Ql,Qr = 0;
        int idx = 0;
        bool trans = false;
        bool brace = false, bracket = false , parenthese = false ,starting = true;
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
                    newRE +="+\\";
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
                case ')':
                    newRE +=']';
                    ++idx;
                    parenthese = false;
                default:
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
        return newRE;
    }
        /*
        string newRE;
        int last = 0,pos,tail,Ql,Qr = 0;
        while ((Ql = raw.find('"',Qr + 1)) != string::npos){
            if(Ql != 0 && raw[Ql-1] =='\\'){
                Qr = Ql + 1;
                continue;
            }
            
            if ((Qr = raw.find('"',Ql + 1)) == string::npos){
                string tmp(" '\"' expected in " );
                tmp += raw;
                throw invalid_argument(tmp);
            }
            break;
        }
        while ((pos = raw.find('{',last))!=string::npos){
            if (pos != 0 && raw[pos - 1] == '\\'){
                last = pos + 1;
                continue;
            }
            while (Ql != string::npos && pos > Qr){
                if ((Ql = raw.find('"',Qr)) != string::npos){
                    if ((Qr = raw.find('"',Ql + 1)) == string::npos){
                        string tmp(" '\"' expected in " );
                        tmp += raw;
                        throw invalid_argument(tmp);
                    }
                }                
            }

            if (Ql < pos && pos < Qr){
                newRE.append(raw,last,Qr - last + 1);
                last = Qr + 1;
                continue;
            } else {
                newRE.append(raw,last,pos - last );
            }
            if ((tail = raw.find('}',pos + 1)) == string ::npos){
                string tmp(" '}' expected in " );
                tmp += raw;
                throw invalid_argument(tmp);
            } 
            if (raw.at(pos + 1)<='9'&&raw.at(pos + 1)>='0'){
                newRE += '(';
                newRE.append(raw,pos,tail - pos + 1);
                newRE += ')';
                last = tail + 1;
            } else {
                string &&name = raw.substr(pos + 1,tail - pos - 1);
                if (!preDefine.count(name)){
                    string tmp("undefined identifier ");
                    tmp += name;
                    tmp +=" occurs in RE ";
                    tmp += raw;
                    throw invalid_argument(tmp);
                }
                newRE.append(preDefine[name]);
                last = tail + 1;
            }
        }
        if (last < raw.size()){
            newRE.append(raw,last,raw.size());
        }
        return newRE;
    }*/
    // static string prepareRE(string RE){
    //     string rt;
    //     int j = 0,i = 0;
    //      rt.resize(RE.length()<<1);
    //     if (RE[i] == '^'){
    //         rt += RE[i++];
    //     }
    //     // if (RE[i] == '\\'){
    //     //     rt+=RE[i++];
    //     // }
    //     // rt+=RE[i++];
    //     while(i < RE.length()){
    //         switch (RE[i]){
    //             case '\\':
    //                 rt += '^';
    //                 rt += RE[i++];
    //                 rt += RE[i++];        // todo: exception 
    //                 break;
    //             case '{':

    //                 break;

    //             case '[':
    //                 rt += '^';
    //                 rt += RE[i];
    //                 while (RE[i]!=']'){
    //                     rt += RE[++i];
    //                 }
    //                 break;
    //             case '?':
    //                 ++i;
    //                 break;
    //             case '+':
    //                 ++i;
    //                 break;
    //             case '*':
    //                 ++i;
    //                 break;
    //             case '|':
    //                 ++i;
    //                 break;
    //             default:
    //                 rt += '^';
    //                 rt += RE[i++];
    //                 break;
    //         }
    //     }
    //     return rt;
    // }
    static void init(){
        for (int i = 0; i < 256; ++i){
            transChar[i] = i;
        }
        transChar['n'] = '\n';
        transChar['r'] = '\r';
      //  transChar['\\'] = '\\';
      //  memset(Newpri,-1,sizeof(Newpri));
      //  memset(Oldpri,-1,sizeof(Oldpri));
        
    }

    // static string repeat(string re,int x,int y){
    //     string tmp;
    //     --x;
    //     while(x--){
    //         tmp.append(re);
    //     }
    //     if (x!=-1){

    //     }
    // }

    // static string getAll(){
    //     string tmp;
    //     char c = 0;
    //     tmp.resize(256);
    //     tmp += c;
    //     for (c = 1; c < 255; ++c){
    //         if (c == '\n'){
    //             continue;
    //         }
    //         tmp += '|';
    //         tmp += c;
    //     }
    //     tmp += '|';
    //     tmp += c;
    //     return tmp;
    // }



};


char RE::transChar[256];   
int RE::Newpri[256];
int RE::Oldpri[256];

