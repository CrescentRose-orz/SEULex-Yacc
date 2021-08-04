#include"RE.h"

/*
。 * 【】 ^ $ {} \ + ? | "" / {}
.[] {name} \ "" 
* $ ^ {}
*/



    RE_operator::RE_operator(char c){
        op = c;
        l = 0,r = 0;
    }
    RE_operator::RE_operator(char c,int l,int r){
        op = c;
        this->l = l;
        this->r = r;
    }
    // operator int(){
    //     return (int)op;
    // }
    RE_operator::operator char(){
        return op;
    }


    char RE::transChar[256];   
    int RE::newPri[256];
    int RE::oldPri[256];
    //  void buildGraph(vector<string> &,vector<string,string> &);
    //  void checkCircle();
    char RE::trans(int &idx,string s){
        ++idx;
        if (idx < s.size()){
            throw invalid_argument("char expected after '\\");
        }
        return transChar[s[idx]];
    }
    char RE::trans(char c){
        return RE::transChar[c];
    }
    int RE::skipFirst(string & newRE,const string &raw,int &idx,map<string,string> &preDefine){
        int rt = 0;
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
                    int tail = idx + 1;
                    while (tail < raw.size()&&raw[tail] != '\"'){
                        ++tail;
                    }
                    if (tail == raw.size() && raw[raw.size()-1] != '\"'){

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
                if (raw[idx] =='\\'){
                    newRE +='\\';
                    newRE += raw[++idx];
                    ++idx;
                    //newRE+=(trans(idx,raw));
                } else {
                    newRE+=raw[idx++];
                }
        }
            return rt;
    }
    string RE::unfoldRE(const string &raw,map<string,string> &preDefine){
        string newRE;
        int last = 0,pos,tail,Ql,Qr = 0;
        int idx = 0;
        bool trans = false;
        bool brace = false, bracket = false;
        int  parenthese = skipFirst(newRE,raw,idx,preDefine);
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
    //   string prepareRE(string RE){
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
    /*
/*
. 匹配任何单个字符，除\n.                       

- 表示匹配范围，如：a-z，表示匹配a-z之间的任何字符

* 匹配前面表达式的零个或多个拷贝。

[] 匹配括号内的任意字符的字符类，第一个符号是"^"，表示匹配除括号中的字符以外的任意字符。

^ 作为正则表达式的第一个字符，匹配行的开头。   例：^ab.,表示以ab开头的字符串.

$ 作为正则表达式的最后一字符，匹配行的结尾。


\ 用于转义字符                                                                      OK

+ 匹配前面表达式一次或多次出现。                                                      OK

? 匹配前面表达式零次或1次出现。                                                       OK

| 匹配前面表达式或随后表达式                                                          OK

"" 引号中的每个字符解释为字面意思


{} 指示一个模式可能出现的次数


    */
    void RE::init(){
        for (int i = 0; i < 256; ++i){
            transChar[i] = i;
        }
        transChar['n'] = '\n';
        transChar['r'] = '\r';
        transChar['t'] = '\t';
        transChar['f'] = '\f';
        transChar['b'] = '\b';
        transChar['a'] = '\a';
        transChar['v'] = '\v';
        memset(newPri,0,sizeof(newPri));
        memset(oldPri,0,sizeof(newPri));    
        newPri['|'] = 1;    
        newPri['^'] = 3;
        newPri['+'] = newPri['*'] = newPri['?'] = newPri['{'] =5;
        newPri['-'] = newPri['$'] = -1;
        newPri['('] = 11;
        oldPri['('] = 0;
        oldPri['|'] = 2;    
        oldPri['^'] = 4;
        oldPri['+'] = oldPri['*'] = oldPri['?'] = oldPri['{'] = 6;
        newPri['$'] = 0;

      //  transChar['\\'] = '\\';
      //  memset(newPri,-1,sizeof(newPri));
      //  memset(oldPri,-1,sizeof(oldPri));
        
    }

    //   string repeat(string re,int x,int y){
    //     string tmp;
    //     --x;
    //     while(x--){
    //         tmp.append(re);
    //     }
    //     if (x!=-1){

    //     }
    // }

    //   string getAll(){
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