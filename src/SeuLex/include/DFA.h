#ifndef DFA_HEADER
#define DFA_HEADER
#include"Hash.hpp"
#include<bits/stdc++.h>
#include"NFA.h"
#include"basicFA.hpp"
#include"CONSTANT.h"
#include"threadpool.hpp"
#include"semaphore.h"
using namespace std;
class DFA;
class NFA_eclosure{
private:
    NFA& _NFA;
public:
    int actIdx = -1;
    action _action;
    set<int> NFAs;
    eclosureHash hash;
    NFA_eclosure(const NFA_eclosure & other);
    NFA_eclosure(NFA &_NFA);//:hash(),_NFA(_NFA)
    //NFA_eclosure(int idx);//:hash(basicHash(idx));
    void add(int idx);  
    bool operator == (const NFA_eclosure &other)const;
    bool has(int idx);
    void expandEclosure();
    friend ostream &operator << (ostream& out,const NFA_eclosure &eNFA);
    NFA_eclosure move(int &c);
};


class DFA_Node{
private:
#define _state_type map<int,int>
    _state_type state;
    bool hasAction = 0;
    #ifdef VISUAL
    visualFA<int> &fa; 
    vector<pair<int,int>> edge;
    bool aflag = 0;
    #endif
public:
    int idx;
    action act;    
    #ifdef VISUAL
    _state_type::const_iterator stateBegin(){
        return state.cbegin();
    }
    _state_type::const_iterator stateEnd(){
        return state.cend();
    }
    DFA_Node(visualFA<int> &_fa):fa(_fa){
        hasAction = 0;
        idx = -1;
    }
    DFA_Node(action act,visualFA<int> &_fa):act(act),fa(_fa){
        hasAction = 1;
        idx = -1;
    }
    void update(){
        for (auto &p:edge){
            fa.addEdge(idx, p.first, p.second);
        }
        if (aflag){
            fa.addNode(idx);
        }
    }
    #else
    DFA_Node(){
        hasAction = -1;
        idx = -1;
    }
    DFA_Node(action act):act(act){
        hasAction = 1;
        idx = -1;
    }
    #endif
    void setAction(action act){
        this->act = act;
        if (hasAction) return;
        hasAction = 1;        
        #ifdef VISUAL
        if (idx!=-1){
            fa.addNode(idx);
        } else {
            aflag = 1;
        }
        #endif

    }
    void addMultiTrans(int target,int l,int r){
        for (int i = l; i <= r; ++i){
            addTrans(target,i);
        }
    }
    void addTrans(int target,int c){
        if (state.count(c)){
            throw invalid_argument("add multi trans in NFA");
        }
        state.insert({c,target});
        #ifdef VISUAL
        if( idx != -1){
            fa.addEdge(idx , target, c);
        } else {
            edge.push_back(make_pair(target,c));
        }
        #endif
    }
    bool valid(){
        return hasAction == -1 ? 0 : 1;
    }

    int getTrans(int c){
        return state.count(c)?state[c]:-1;
    }
};


class DFA:public basicFA<DFA_Node>{

private:
#ifdef USE_MULTITHREAD
    shared_timed_mutex taskM;
    atomic_int taskCnt;
    threadpool tp;
    _mutex_type tpMutex;
    _mutex_type visMutex;
    _mutex_type qMutex;
    my::semaphore task{1};
    atomic_int idleCnt;
    //void IncTask();
    //void DecTask();
    void MULTITHREAD_expandEclosure(NFA_eclosure nowE);
    void process();
#endif
    _mutex_type mapMutex;
    queue<NFA_eclosure> q;
    unordered_map<eclosureHash,int,hashFunction> DFAMap;
    unordered_map<eclosureHash,int,hashFunction> vis;
public:
    int tail = 0;
    DFA();
    DFA(Logger &log);
    int add();
    int add(DFA_Node node);
    int addTrans(int from, int to, int c);
    int head();
    bool exist(NFA_eclosure &_e);
    int idx(NFA_eclosure &_e);
    int insert(NFA_eclosure &_e);
    void NFA2DFA(NFA& _NFA);
    void expandEclosure(NFA_eclosure &nowE);
    void generateCode(fstream &file);

};


#endif