#pragma once
#include<bits/stdc++.h>
#include"RE.hpp"
#include<action.hpp>
#include"visualFA.hpp"
#define eps 256
#define VISUAL
#define dev
using namespace std;
const int NFA_t =258;
enum operandType{opNFA = 0,opNum,opString};

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

class operand {
public:
    operandType type;
    string content;
    int num;
    operand(){
        type = opNFA;
    }
    operand(int num){
        this -> type = opNum;
        this -> num = num;
    }
    operand(string content){
        this -> type = opString;
        this -> content = content;
    }
};

class NFA_Node{
private:
    multimap<int,int> state;
    action act;
    bool hasAction;
    #ifdef VISUAL
    visualFA<int> &fa; 
    vector<pair<int,string>> edge;
    #endif
public:
    int idx;
    #ifdef VISUAL
    NFA_Node(visualFA<int> &_fa):fa(_fa){
        hasAction = -1;
        idx = -1;
    }
    NFA_Node(action act,visualFA<int> &_fa):act(act),fa(_fa){
        hasAction = 1;
        idx = -1;
    }
    void update(){
        for (auto &p:edge){
            fa.addEdge(idx, p.first, p.second);
        }
    }
    #else
    NFA_Node(){
        hasAction = -1;
        idx = -1;
    }
    NFA_Node(action act):act(act){
        hasAction = 1;
        idx = -1;
    }
    #endif
    void setAction(action act){
        this->act = act;
        if (hasAction) return;
        #ifdef VISUAL
        fa.addNode(idx);
        #endif
        hasAction = 1;
    }
    void addMultiTrans(int target,int l,int r){
        for (int i = l; i <= r; ++i){
            addTrans(target,i);
        }
    }
    void addTrans(int target,int c){
        auto pr = state.equal_range(c);
        for (auto iter = pr.first; iter!=pr.second;++iter){
            if (iter->second == target){
                return;
            }
        }
        state.insert(c,target);
        #ifdef VISUAL
        string s;        
        s += (c == eps)?'\238':(char)c;
        if( idx != -1){
            fa.addEdge(idx , target, s);
        } else {
            edge.push_back(make_pair(target,s));
        }
        #endif
    }
   // int transfer(int input);
    bool valid(){
        return hasAction == -1 ? 0 : 1;
    }
};


class NFA_Cluster:public operand{
public:    
    int head;
    int tail;
    static bool checkBracket(string &bracket,int l,int r){
        int last = l - 1;
        if (l>r || bracket[r] =='-'){
            return 0;
        }
        for (int i = l; i <= r;++i){
            if (bracket[i] =='-'){
                if (i - 1 <= last){
                    return 0;
                }
                last = i + 1;
            }
        }
        return true;
    }
    NFA_Cluster(int head){
        this->head = head;
        this->tail = -1;
    }
    NFA_Cluster(int head,int tail){
        this->head = head;
        this->tail = tail;
    }
    //single char,.
    NFA_Cluster(NFA &buff,char c){
        tail = buff.add();
        head = buff.add();
        {                    
            lock_guard<mutex> lock(buff.Wrlock);
            if (c!='.'){
                buff[head].addTrans(tail,c);
            } else {
                buff[head].addMultiTrans(tail,0,255);
            }
        }
    }
    // '|' '^'
    NFA_Cluster(NFA &buff,char op ,NFA_Cluster a ,NFA_Cluster b ){
        switch(op){
            case '|':{
                head = buff.add();
                tail = buff.add();
                {
                    lock_guard<mutex> lock(buff.Wrlock);
                    buff[head].addTrans(a.head,eps);
                    buff[head].addTrans(b.head,eps);  
                    buff[a.tail].addTrans(tail,eps);
                    buff[b.tail].addTrans(tail,eps);
                }
                return ;
            }
            break;
            case '^':{
                head = a.head;
                tail = b.tail;
                {                
                    lock_guard<mutex> lock(buff.Wrlock);
                    buff[a.tail].addTrans(b.head,eps);
                }
                return;
            }
            break;
            default:
                throw invalid_argument("only  accept '^' or '|', but found " + op);
            
        }
    }
    // multi {}
    NFA_Cluster(NFA &buff,int l,int r,NFA_Cluster a ){
        throw invalid_argument("multi{} this function is not available");
        if (l < 0 || r < l){
            throw invalid_argument("error, {l,r} must have l >= 0 && r >= l");
        }
        // //NFA_Node _tail;
        // vector<NFA_Node> _heads(r+1);
        // tail = buff.add(_tail);
        // for (int i = l; i <= r; ++i){
        //     _heads[i].addTrans(tail,);
        // }


    }
    //single op : * ? +
    NFA_Cluster(NFA &buff,char op ,NFA_Cluster a ){
        NFA_Node _head(buff.vNFA),_tail(buff.vNFA);
        head = buff.add();
        tail = buff.add();      
        switch(op){
            case '*':
                tail  = buff.add(_tail);            
                _head.addTrans(a.head,eps);
                _head.addTrans(tail,eps);
                head = buff.add(_head);
                {
                    lock_guard<mutex> lock(buff.Wrlock);
                    buff.pool[a.tail].addTrans(tail,eps);
                    buff.pool[a.tail].addTrans(a.head,eps);
                }                
                break;
            case '?':
                #ifndef dev
                tail  = buff.add(_tail);            
                _head.addTrans(a.head,eps);
                _head.addTrans(tail,eps);
                head = buff.add(_head);
                _tail.addTrans(head,eps);
                {
                    lock_guard<mutex> lock(buff.Wrlock);
                    buff.pool[a.tail].addTrans(tail,eps);
                }             
                #else
                tail  = a.tail;
                head = a.head;
                {
                    lock_guard<mutex> lock(buff.Wrlock);
                    buff[head].addTrans(tail,eps);
                }            
                #endif        
                break;
            case '+':
                tail  = buff.add(_tail);            
                _head.addTrans(a.head,eps);
                head = buff.add(_head);
                {
                    lock_guard<mutex> lock(buff.Wrlock);
                    buff.pool[a.tail].addTrans(tail,eps);
                    buff.pool[a.tail].addTrans(a.head,eps);
                }       
                break;
            default:
                // todo: throw exception 
                break;
        }
    }
   //quotation 

    NFA_Cluster(NFA &buff ,string &quotation , int l ,int r ){ //[l,r]

        NFA_Cluster &&rt = NFA_Cluster::createEmpty(buff);
        NFA_Node _head(buff.vNFA),_tail(buff.vNFA);      
        bool trans;
        for (int i = l ; i < r; ++i){
            if (trans){
                int tailIdx = buff.add();
                {
                    lock_guard<mutex> lock(buff.Wrlock);
                    buff[rt.tail].addTrans(tailIdx,RE::trans(quotation[i]));
                }
                rt.tail = tailIdx;
                trans = false;
            
            }
            if (quotation[i] =='\\'){
                trans = true;
                continue;
            }
            int tailIdx = buff.add();
            {
                lock_guard<mutex> lock(buff.Wrlock);
                buff.pool[rt.tail].addTrans(tailIdx,quotation[i]);            
            }
            rt.tail = tailIdx;
        }
        this -> head = rt.head;
        this -> tail = rt.tail;
    }
    //bracket todo 【】
    static NFA_Cluster getBracket(NFA &buff,string &bracket,int l,int r){
        if (l == r){
            throw invalid_argument("empty bracket is not allowed");
        }
        NFA_Cluster rt(buff.add(),buff.add());
        if (!NFA_Cluster::checkBracket(bracket,l,r)){
            stringstream s;
            s<<"syntax error in bracket expression "<<bracket.substr(l,r-l+1);
            throw invalid_argument(s.str());
        }
        for (int i = l; i <= r; ++i){
            if (bracket[i] != '-'){
                lock_guard<mutex> lock(buff.Wrlock);
                buff[rt.head].addTrans(rt.tail,bracket[i]);
            } else {
                lock_guard<mutex> lock(buff.Wrlock);
                buff[rt.head].addMultiTrans(rt.tail,bracket[i-1],bracket[i+1]);
            }
        }
    }
    //todo{}
    static NFA_Cluster getBrace(NFA &buff,string &bracket,int l ,int r){
        NFA_Cluster rt(0,0);
        return rt;
    }
    static NFA_Cluster createEmpty(NFA &buff){
        int && idx = buff.add();
        return NFA_Cluster(idx,idx);
    }

    static NFA_Cluster createSingle(NFA &buff,NFA_Node node){
        int && idx = buff.add(node);
        return NFA_Cluster(idx,idx);
    }





static NFA_Cluster RE2NFA_Cluster(string RE,NFA &buff){
//stack<operand*> operandStack;
stack<NFA_Cluster> operandStack;
stack<RE_operator> operatorStack; 
RE_operator newOp('.');
int i = 0,j;
    RE += '$';
    //operatorStack.push('`');
    NFA_Cluster &&head = NFA_Cluster::createEmpty(buff);
    while(i<RE.size()){
        switch(RE[i]){
            case ')': 
                while((char)operatorStack.top() != '('){
                    cal(buff,operandStack,operatorStack.top());
                    operatorStack.pop();
                }
                operatorStack.pop();
                ++i;
                break;
            case '[': //todo
                for (j = i + 1; RE[j] != ']';++j);
                if (j == i + 1){
                    i = j + 1;
                    continue;
                }
                operandStack.push(NFA_Cluster::getBracket(buff,RE,i+1,j-1));
                i = j + 1;
                break;
            case '\"':
                for (j = i + 1; RE[j] !='"'&&j < RE.size();++j);
                if (j == RE.size()){
                    stringstream s;
                    s<<"\" didn't match in "<<RE;
                    throw invalid_argument(s.str());
                }
                operandStack.push(NFA_Cluster(buff,RE,i+1,j-1));
                break;
            case '{': //todo
                throw invalid_argument("not supported yet");
                // if (RE::newPri[RE[i]] > RE::oldPri[operatorStack.top()]){
                //     operatorStack.push(RE[i]);
                //     break;
                // }
                // break;
                newOp = RE_operator('{',0,0);
            case '(':
            case '|': 
            case '*':
            case '?':
            case '+': 
            case '^': 
                newOp = RE_operator(RE[i]);
                while (RE::newPri[newOp] < RE::oldPri[operatorStack.top()]){
                    NFA_Cluster::cal(buff,operandStack,operatorStack.top());
                    operatorStack.pop();
                } 
                operatorStack.push(RE[i]);
                break;
            case '.':
                operandStack.push(NFA_Cluster('.'));
                break;
            default: 
                //operand* p = new NFA_Cluster(RE[i]);
                //operandStack.push(p);
                operandStack.push(NFA_Cluster(RE[i]));
                break;
        }
    }
    if (operandStack.size() != 1){
        stringstream s;
        s <<"expeceted 1 operand left but found"<<operandStack.size()<< "operands";
        throw invalid_argument(s.str());
    }
    return operandStack.top();
}

static NFA_Cluster RE2NFA(string RE,NFA &buff,action _action){
    NFA_Cluster &&head = NFA_Cluster::RE2NFA_Cluster(RE[0]=='^'?RE.substr(1,RE.size()-(RE[RE.size()-1]=='$'?2:1)):RE,buff);
    int nhead = buff.add(),ntail = buff.add();
    {
        lock_guard<mutex> lock(buff.Wrlock);
        buff[nhead].addTrans(head.head,'\n');
    }
    if (RE[0] != '^'){
        lock_guard<mutex> lock(buff.Wrlock);
        buff[nhead].addTrans(head.head,eps);
    } 
    head.head = nhead;
    {
        lock_guard<mutex> lock(buff.Wrlock);
        buff[ntail].setAction(_action);
        buff[head.tail].addTrans(ntail,'\n');
    }

    if (RE[RE.size()-1] != '$'){
        lock_guard<mutex> lock(buff.Wrlock);
        buff[head.tail].addTrans(ntail,eps);
    }  
    head.tail = ntail;  
    return head;
}



static NFA_Cluster cal(NFA &buff,stack<NFA_Cluster> &operandStack,RE_operator op){
NFA_Cluster operand1 = operandStack.top();
    operandStack.pop();
NFA_Cluster operand2 = operandStack.top();
    switch(op.op){
        case '|':
        case '^':
            operandStack.pop();        
            return NFA_Cluster(buff,op,operand1,operand2);
        case '?':
        case '+':
        case '*':
            return NFA_Cluster(buff,op,operand1);
        case '{':
            throw invalid_argument("{ not supported yet");
        default:
            stringstream s;
            s << "unknown operator occurs" << op.op<<" founded";
            throw invalid_argument(s.str());
    };
}



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
————————————————
版权声明：本文为CSDN博主「求术学技」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/dinghuican/article/details/24620523





*/


};

class NFA{
private:
    int tmp;
public:
    mutex Wrlock;
    int tail = 0;
    vector<NFA_Node> pool;
    NFA(){
        add();
    }
    NFA_Node& operator [](int i){
        return pool[i];
    }
    int add(){
        unique_lock<mutex> lock(Wrlock);
        pool.emplace_back(NFA_Node(vNFA));
        pool[tail].idx = tail;
        return tail++;       
    }
    int add(NFA_Node node){
        unique_lock<mutex> lock(Wrlock);
        pool.emplace_back(node);
        pool[tail].idx = tail;
        pool[tail].update();
        return tail++;
    }
    int head(){
        return 0;
    }
    void addRE(string &RE,action _action){
        NFA_Cluster &&rt = NFA_Cluster::RE2NFA(RE,*this,_action);
        {
            lock_guard<mutex> lock(Wrlock);
            pool[0].addTrans(rt.head,eps);
        }
    }
    #ifdef VISUAL       
    visualFA<int> vNFA;
    #endif
};
