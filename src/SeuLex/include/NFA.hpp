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
        string s;
        s += c == eps?'\238':(char)c;
        #ifdef VISUAL
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

class NFA{
private:
    int tmp;
public:
    mutex Wrlock;
    vector<NFA_Node> pool;
    int head = 0;
    int tail = 0;
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
    visualFA<int> vNFA;
    #ifdef VISUAL    
    #endif
};

class NFA_Cluster:public operand{
public:    
    int head;
    int tail;
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
    // '\' '^'
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

    NFA_Cluster(NFA &buff ,string &quotation , int l ,int r ){ //[l,r)
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
    //bracket todo
    NFA_Cluster(NFA &buff,string bracket){
        
    }
    //todo
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

};

NFA_Cluster RE2NFA_Cluster(string RE,NFA &buff){
//stack<operand*> operandStack;
stack<NFA_Cluster> operandStack;
stack<char> operatorStack; 
int i = 0,j;
    RE += '$';
    operatorStack.push('`');
    NFA_Cluster &&head = NFA_Cluster::createEmpty(buff);
    while(i<RE.size()){
        switch(RE[i]){
            case '(': //todo
                for (j = i + 1; RE[j] != ')';++j);
                if (j == i + 1){
                    i = j + 1;
                    continue;
                }
                operandStack.push(RE2NFA_Cluster(RE.substr(i+1,j-i-1),buff));
                //operand* p;
                //*p = RE2NFA_Cluster(RE.substr(i+1,j-i-1),buff);
                //operandStack.push(p);
                i = j + 1;
                break;
            case '[': //todo
                for (j = i + 1; RE[j] != ']';++j);
                if (j == i + 1){
                    i = j + 1;
                    continue;
                }
               // operand* p;
                //*p = NFA_Cluster::getBracket(buff,)//RE2NFA_Cluster(RE.substr(i+1,j-i-1),buff);
                //operandStack.push(p);
                operandStack.push(NFA_Cluster::getBracket(buff,));
                i = j + 1;
                break;                
                break;
            case '*':
            case '?':
            case '+':  //todo
                if (RE::newPri[RE[i]] > RE::oldPri[operatorStack.top()]){
                    operatorStack.push(RE[i]);
                    break;
                }
                break;
            case '^': //todo
                if (RE::newPri[RE[i]] > RE::oldPri[operatorStack.top()]){
                    operatorStack.push(RE[i]);
                    break;
                }
                break;
            case '{': //todo
                if (RE::newPri[RE[i]] > RE::oldPri[operatorStack.top()]){
                    operatorStack.push(RE[i]);
                    break;
                }
                break;
            case '.':
                operandStack.push(NFA_Cluster('.'));
                break;
            case '|': //todo
                if (RE::newPri[RE[i]] > RE::oldPri[operatorStack.top()]){
                    operandStack.push(RE[i]);
                    break;
                }
                break;
            default: 
                //operand* p = new NFA_Cluster(RE[i]);
                //operandStack.push(p);
                operandStack.push(NFA_Cluster(RE[i]));
                break;
        }


    }
    NFA_Node nhead;
    nhead.addTrans(head.tail,'\n');
    head.head = buff.add(nhead);
    return head;

}

NFA_Cluster RE2NFA(string RE,NFA &buff){
    NFA_Cluster &&head = RE2NFA_Cluster(RE[0]=='^'?RE.substr(1,RE.size()-(RE[RE.size()-1]=='$'?2:1)):RE,buff);
    int nhead = buff.add();
    buff[nhead].addTrans(head.head,'\n');
    if (RE[0] != '^'){
        buff[nhead].addTrans(head.head,eps);
    } 
    head.head = nhead;
    /*
    todo : $ at  tail;






    */
    return head;
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