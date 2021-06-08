#include"Hash.hpp"
#include<bits/stdc++.h>
#include"NFA.hpp"
using namespace std;

class NFA_eclosure{
public:
    set<int> NFAs;
    eclosureHash hash;
    NFA_eclosure():hash(){}
    NFA_eclosure(int idx):hash(basicHash(idx)){}
    void add(int idx){ hash.add(basicHash(idx));}
    bool operator == (NFA_eclosure &other){
        return this -> hash == other.hash;
    }
};


void getEclosure(){
queue<NFA_eclosure> q;
unordered_map<eclosureHash,hashFunction> hasVisited;
    q.push(NFA_eclosure())

}；





class DFA_Node{



};

class DFA{
private:
    int tmp;
public:
    mutex Wrlock;
    vector<NFA_Node> pool;
    int head = 0;
    int tail = 0;
    int add(){
        pool.emplace_back();
    }

    int add(NFA_Node node){
        unique_lock<mutex> lock(Wrlock);
        pool.emplace_back(node);
        ++tail;
        return tail-1;
    }
    #ifdef VISUAL
    visualFA<int> vNFA;

    #endif
};