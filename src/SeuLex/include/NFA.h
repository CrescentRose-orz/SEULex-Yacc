#ifndef NFA_HEADER
#define NFA_HEADER
#include<bits/stdc++.h>
#include"RE.h"
#include"action.hpp"
#include"visualFA.hpp"
#include"NFA_Cluster.h"
#include"Logger.h"
#include"CONSTANT.h"
#include"basicFA.hpp"
#include"Hash.hpp"
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
    bool hasAction = 0;
    basicHash _hash;
    #ifdef VISUAL
    visualFA<int> &fa; 
    vector<pair<int,int>> edge;
    bool aflag = 0;
    #endif
public:
    int idx;
    #ifdef VISUAL
    NFA_Node(visualFA<int> &_fa):fa(_fa){
        hasAction = 0;
        idx = -1;
    }
    NFA_Node(action act,visualFA<int> &_fa):act(act),fa(_fa){
        hasAction = 1;
        idx = -1;
    }
    void update(basicHash _hash){
        this -> _hash = _hash;        
        update();
    }
    void update(){
        for (auto &p:edge){
            fa.addEdge(idx, p.first, p.second);
        }
        if (aflag){
            fa.addNode(idx);
        }
        if (!_hash.isValid()){
            _hash = basicHash(idx);
        }
    }
    basicHash hash(){
        return _hash;
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
    void setHash(basicHash _hash){
        this->_hash = _hash;
    }
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
    action getAction(){
        return act;
    }
    void addMultiTrans(int target,int l,int r){
        for (int i = l; i <= r; ++i){
            addTrans(target,i);
        }
    }
    void addTrans(int target,int c){
        try{
        if (state.find(c)!=state.end()){
            auto pr = state.equal_range(c);
            for (auto iter = pr.first; pr.first!=state.end()&&iter!=pr.second;++iter){
                if (iter->second == target){
                    return;
                }
            }
        }}
        catch(exception e){
            cerr<<"ERROR!"<<endl;
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
   // int transfer(int input);
    bool valid(){
        return hasAction == -1 ? 0 : 1;
    }
    vector<int> getTrans(int c){
        vector<int> rt;
        auto pr = state.equal_range(c);
        for (auto iter = pr.first; iter != pr.second;++iter){
            rt.emplace_back(iter->second);
        }   
        return rt;
    }
};

class NFA:public basicFA<NFA_Node>{
public:
    int tail = 0;
    NFA();
    NFA(Logger &log);
    basicHash nowHash;
    int add();
    int add(NFA_Node node);
    int head();
    void addRE(string &RE,action _action);
};

class NFA_Iterator{
private:
    NFA& data;
    int place;
public:
    NFA_Iterator(NFA& d):data(d){

    }
};
#endif