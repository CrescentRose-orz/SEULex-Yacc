#include"DFA.h"

NFA_eclosure::NFA_eclosure(NFA &_NFA):hash(),_NFA(_NFA){}
    //NFA_eclosure(int idx);//:hash(basicHash(idx));
void NFA_eclosure::add(int idx){
    RdLock(_NFA.Wrlock);
    NFAs.insert(idx);
    hash.add(_NFA[idx].hash());
}
bool NFA_eclosure::operator == (const NFA_eclosure &other)const{
    return hash == other.hash;
}
void NFA_eclosure::expandEclosure(){
queue<int> q;
int now;
    for (auto _nfa:NFAs){
        q.push(_nfa);
    }
    while(!q.empty()){
        now = q.front();
        q.pop();
        {
            RdLock(_NFA.Wrlock);
            vector<int> &&rt =_NFA[now].getTrans(eps);
            for (auto &t:rt){
                if (!has(t)){
                    q.push(t);
                    add(t);
                }
            }
        }
    }
}
bool NFA_eclosure::has(int x){
    return NFAs.find(x) != NFAs.end();
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
int DFA::addTrans(int from, int to, int c){
    WrLock(Wrlock);
    pool[from].addTrans(to,c);
}
bool DFA::exist(NFA_eclosure &_e){
    shared_guard<shared_mutex> lock(mapMutex);
    return DFAMap.count(_e.hash);
}
int DFA::idx(NFA_eclosure &_e){
    if (exist(_e)){
        RdLock(mapMutex);
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
#ifdef MULTI
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
#else
queue<NFA_eclosure> q;
NFA_eclosure startPoint(_NFA);
    startPoint.add(_NFA.head());
    startPoint.expandEclosure();    
    insert(startPoint);
    q.push(startPoint);
    while (!q.empty()){
        NFA_eclosure & now = q.front();
        for (int i = 0; i <= charSetMAX; ++i){
            NFA_eclosure newE(_NFA);            
            for (auto &t:now.NFAs){
                RdLock(Wrlock);
                vector<int> &&rt = _NFA[t].getTrans(i);
                for (auto &newt:rt){
                    if (!newE.has(newt)){
                        newE.add(newt);
                    }
                }
            }
            addTrans(idx(now),insert(newE),i);
            q.push(newE);
        }
        q.pop();
    }
#endif
}