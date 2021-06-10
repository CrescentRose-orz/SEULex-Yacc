#ifndef DFA_HEADER
#define DFA_HEADER
#include"Hash.hpp"
#include<bits/stdc++.h>
#include"NFA.h"
#include"basicFA.hpp"
#include"CONSTANT.h"
#include"threadpool.hpp"
using namespace std;

class NFA_eclosure{
private:
    NFA& _NFA;
public:
    set<int> NFAs;
    eclosureHash hash;
    NFA_eclosure(NFA &_NFA);//:hash(),_NFA(_NFA)
    //NFA_eclosure(int idx);//:hash(basicHash(idx));
    void add(int idx);  
    bool operator == (const NFA_eclosure &other)const;
    bool has(int idx);
    void expandEclosure();
};

class DFA;
class DFA_Node{
private:
    map<int,int> state;
    action act;
    bool hasAction = 0;
    #ifdef VISUAL
    visualFA<int> &fa; 
    vector<pair<int,int>> edge;
    bool aflag = 0;
    #endif
public:
    int idx;
    #ifdef VISUAL
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
    shared_mutex mapMutex;
    unordered_map<eclosureHash,int,hashFunction> DFAMap;
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

};


#endif