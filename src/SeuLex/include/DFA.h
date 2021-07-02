#ifndef DFA_HEADER
#define DFA_HEADER
#include"Hash.hpp"
#include"NFA_eclosure.h"
#include<bits/stdc++.h>
#include"NFA.h"
#include"basicFA.hpp"
#include"CONSTANT.h"
#include"threadpool.hpp"
#include"semaphore.h"
#include "DFA_Node.hpp"
using namespace std;
class NFA_eclosure;
class DFA_Node;
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
    _mutex_type mapMutex;
    //void IncTask();
    //void DecTask();
    void MULTITHREAD_expandEclosure(NFA_eclosure nowE);
    void process();
#endif
    queue<NFA_eclosure> q;
    unordered_map<eclosureHash,int,hashFunction> DFAMap;
    unordered_map<eclosureHash,int,hashFunction> vis;
public:
    int tail = 0;
    DFA();
    DFA(Logger &log);
    DFA(const DFA &_DFA);
    int add();
    int add(DFA_Node node);
    int addTrans(int from, int to, int c);
    int head();
    bool exist(NFA_eclosure &_e);
    int idx(NFA_eclosure &_e);
    int insert(NFA_eclosure &_e);
    void NFA2DFA(NFA& _NFA);
    void expandEclosure(NFA_eclosure &nowE);
    void generateCode(fstream &file,int flag = 0);
    DFA minimize();
};


#endif