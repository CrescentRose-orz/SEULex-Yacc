#include<bits/stdc++.h>
#include"RE.hpp"
#include<action.hpp>
#define eps 256

#define dev
using namespace std;
const int NFA_t =258;


class NFA_Node{
private:
    multimap<int,int> state;
    action act;
    bool hasAction;
public:
    NFA_Node(){
        hasAction = -1;
    }
    NFA_Node(action act):act(act){
        hasAction = 1;
    }
    void setAction(action act){
        this->act = act;
        hasAction = 1;
    }
    void addMultiTrans(int target,int l,int r);
    void addTrans(int target,int c);
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
    int add(NFA_Node node){
        unique_lock<mutex> lock(Wrlock);
        pool.emplace_back(node);
        ++tail;
        return tail-1;
    }
};

class NFA_Cluster{
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
    NFA_Cluster(NFA &buff,char c){
        NFA_Node _head,_tail;
        tail = buff.add(_tail);
        if (c!='.'){
            _head.addTrans(tail,c);
        } else {
            _head.addMultiTrans(tail,0,255);
        }
        head = buff.add(_head);
    }
    NFA_Cluster(NFA &buff,char op ,NFA_Cluster a ,NFA_Cluster b ){
        NFA_Node _head,_tail;
        _head.addTrans(a.head,eps);
        _head.addTrans(b.head,eps);
        head = buff.add(_head);        
        tail = buff.add(_tail);
        {
            lock_guard<mutex> lock(buff.Wrlock);
            buff.pool[a.tail].addTrans(tail,eps);
            buff.pool[b.tail].addTrans(tail,eps);
        }
    }
    NFA_Cluster(NFA &buff,int l,int r,NFA_Cluster a ){
        if (l < 0 || r < l){
            throw invalid_argument("error, {l,r} must have l >= 0 && r >= l");
        }
        NFA_Node _tail;
        vector<NFA_Node> _heads(r+1);
        tail = buff.add(_tail);
        for (int i = l; i <= r; ++i){
            _heads[i].addTrans(tail,);
        }


    }
    //single op : * ? +
    NFA_Cluster(NFA &buff,char op ,NFA_Cluster a ){
        NFA_Node _head,_tail;      
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
                    buff.pool[head].addTrans(tail,eps);
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
        NFA_Node _head,_tail;      
        bool trans;
        for (int i = l ; i < r; ++i){
            if (trans){
                int tailIdx = buff.add(NFA_Node());
                lock_guard<mutex> lock(buff.Wrlock);
                buff.pool[rt.tail].addTrans(tailIdx,RE::trans(quotation[i]));
                rt.tail = tailIdx;
                trans = false;
            }
            if (quotation[i] =='\\'){
                trans = true;
                continue;
            }
            int tailIdx = buff.add(NFA_Node());
            lock_guard<mutex> lock(buff.Wrlock);
            buff.pool[rt.tail].addTrans(tailIdx,quotation[i]);            
            rt.tail = tailIdx;
        }
        this -> head = rt.head;
        this -> tail = rt.tail;
    }
    static NFA_Cluster createEmpty(NFA &buff){
        int && idx = buff.add(NFA_Node());
        return NFA_Cluster(idx,idx);
    }
    NFA_Cluster getBrace(NFA &buff,string &brace ,int l ,int r){
        NFA_Cluster rt;
        return rt;
    }
    static NFA_Cluster createSingle(NFA &buff,NFA_Node node){
        int && idx = buff.add(node);
        return NFA_Cluster(idx,idx);
    }

};

NFA_Cluster RE2NFA_Cluster(string RE,NFA &buff){
stack<NFA_Cluster> content;
stack<char> op; 
int i = 0,j;
    NFA_Cluster &&head = NFA_Cluster::createEmpty(buff);
    NFA_Node nhead;
    nhead.addTrans(head.tail,'\n');
    if (RE[0] != '^'){
        nhead.addTrans(head.tail,eps);
    } else {
        ++i;
    }
    head.head = buff.add(nhead);

    content.push(head);
    

    while (i < RE.size()){



    }
    while (!op.empty()){


    }
    return head;

}

NFA_Cluster RE2NFA(string RE,NFA &buff){
    NFA_Cluster &&head = RE2NFA_Cluster(RE[0]=='^'?RE.substr(1,RE.size()-(RE[RE.size()-1]=='$'?2:1)):RE,buff);
    NFA_Node nhead;
    nhead.addTrans(head.head,'\n');
    if (RE[0] != '^'){
        nhead.addTrans(head.head,eps);
    } 
    head.head = buff.add(nhead);
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