#include"DFA.h"
/*
class NFA_eclosure{
private:
    NFA& _NFA;
public:
    set<int> NFAs;
    eclosureHash hash;
    NFA_eclosure(NFA &_NFA);//:hash(),_NFA(_NFA)
    //NFA_eclosure(int idx);//:hash(basicHash(idx));
    void add(int idx){ hash.add(basicHash(idx));}
    bool operator == (NFA_eclosure &other){
        return this -> hash == other.hash;
    }
};
*/
    NFA_eclosure::NFA_eclosure(NFA &_NFA):hash(),_NFA(_NFA){}
    void NFA_eclosure::add(int idx){
        //hash.add(basicHash(idx));
        RdLock(mutex);
        hash.add(_NFA[idx].hash());
    }
    bool NFA_eclosure::operator==(const NFA_eclosure &other)const{
        return this -> hash == other.hash;
    }

DFA::DFA(){}
DFA::DFA(Logger &log):basicFA(log){}

int DFA::add(){
    unique_lock<shared_mutex> lock(Wrlock);
    pool.emplace_back(DFA_Node(vFA));
    pool[tail].idx = tail;
    return tail++;       
}

int DFA::add(DFA_Node node){
    unique_lock<shared_mutex> lock(Wrlock);
    pool.emplace_back(node);
    pool[tail].idx = tail;
    pool[tail].update();
    return tail++;
}
int DFA::head(){
    return 0;
}

bool DFA::exist(NFA_eclosure &_e){
    shared_lock<shared_mutex> lock(mapMutex);
    return DFAMap.count(_e.hash);
}
int DFA::idx(NFA_eclosure &_e){
    if (exist(_e)){
        shared_lock<shared_mutex> lock(mapMutex);
        return DFAMap[_e.hash];
    } else {
        throw invalid_argument("not in the DFAMap!");
    }
}

int DFA::insert(NFA_eclosure &_e){
    if (exist(_e)){
        throw invalid_argument("already in the DFAMap!");          
    }
    int rt;
    {
        lock_guard<shared_mutex> lock(mapMutex);
        rt = add();
    }
    return rt;
}


void DFA::NFA2DFA(NFA& _NFA){
queue<int> q;
threadpool tp(THREADCNT);
    while (!q.empty()){
        if (q.empty()){
            if (tp.idlCount() == THREADCNT){
                break;
            }
            continue;
        }

    }
}