#pragma once
#include<bits/stdc++.h>
#include"RE.hpp"
#include"action.hpp"
#include"visualFA.hpp"
#include"NFA_Cluster.hpp"
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


class NFA{
private:
    int tmp;
public:
    mutex Wrlock;
    int tail = 0;
    vector<NFA_Node> pool;
    NFA();
    NFA_Node& operator [](int i);
    int add();
    int add(NFA_Node node);
    int head();
    void addRE(string &RE,action _action);
    #ifdef VISUAL       
    visualFA<int> vNFA;
    #endif
};

NFA::NFA(){
        add();
}

int NFA::add(){
        unique_lock<mutex> lock(Wrlock);
        pool.emplace_back(NFA_Node(vNFA));
        pool[tail].idx = tail;
        return tail++;       
}

NFA_Node& NFA::operator[](int i){
        return pool[i];
    }
int NFA::add(NFA_Node node){
        unique_lock<mutex> lock(Wrlock);
        pool.emplace_back(node);
        pool[tail].idx = tail;
        pool[tail].update();
        return tail++;
}
int NFA::head(){
        return 0;
}

void NFA::addRE(string &RE,action _action){
        NFA_Cluster &&rt = NFA_Cluster::RE2NFA(RE,*this,_action);
        {
            lock_guard<mutex> lock(Wrlock);
            pool[0].addTrans(rt.head,eps);
        }
}