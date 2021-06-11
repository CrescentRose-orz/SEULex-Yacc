#include"DFA.h"

NFA_eclosure::NFA_eclosure(NFA &_NFA):hash(),_NFA(_NFA){}
    //NFA_eclosure(int idx);//:hash(basicHash(idx));
void NFA_eclosure::add(int idx){
    NFAs.insert(idx);
    hash.add(_NFA[idx].hash());
    RdLock(_NFA.Wrlock);
    if (_NFA[idx].valid()){
        if (actIdx == -1){
            actIdx = idx;
            _action = _NFA[idx].getAction();
        } else if (idx < actIdx){
            actIdx = idx;
            _action = _NFA[idx].getAction();
        }
    }
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
ostream& operator << (ostream& out,NFA_eclosure& eNFA){
    out<<"{";
    for(auto iter = eNFA.NFAs.begin();iter!=eNFA.NFAs.end();++iter){
        out<<*iter<<" ";
    }
    out<<"}";
    return out;
}


DFA::DFA(){}
DFA::DFA(Logger &log):basicFA(log){}

int DFA::add(){
    WrLock(Wrlock);
    pool.emplace_back(DFA_Node(vFA));
    pool[tail].idx = tail;
    return tail++;       
}

int DFA::add(DFA_Node node){
    WrLock(Wrlock);
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
    return 0;
}
bool DFA::exist(NFA_eclosure &_e){
    RdLock(mapMutex);
   // shared_guard<shared_mutex> lock(mapMutex);
    return DFAMap.count(_e.hash);
}
int DFA::idx(NFA_eclosure &_e){
    if (exist(_e)){
        RdLock(mapMutex);
        return DFAMap[_e.hash];
    } else {
        stringstream s;
        s<<_e<<"is not in the DFAMap!";
        throw invalid_argument(s.str());
    }
}
int DFA::insert(NFA_eclosure &_e){
    if (exist(_e)){
        stringstream s;
        s<<_e<<"already in the DFAMap!";
        throw invalid_argument(s.str());       
    }
    int rt;
    {
        rt = add();
        WrLock(mapMutex);
        DFAMap[_e.hash] = rt;
        if (_e.actIdx != -1){
            pool[rt].setAction(_e._action);
        }
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
unordered_set<eclosureHash,hashFunction> vis;
    startPoint.add(_NFA.head());
    startPoint.expandEclosure();    
    insert(startPoint);
    q.push(startPoint);
    stringstream s;
    s<<"begin with" << startPoint;
    logger.customMSG(s.str());
    vis.insert(startPoint.hash);
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
            if (vis.count(newE.hash)){
                continue;
            }
            vis.insert(newE.hash);
            stringstream s;
            newE.expandEclosure();
            s<<now<<" to " <<newE<<" with "<<i;
            logger.customMSG(s.str());
            if (exist(newE)){
                addTrans(idx(now),idx(newE),i);
            } else {
                int &&to = insert(newE);
                addTrans(idx(now),to,i);
                q.push(newE);
            }
        }
        q.pop();
    }
    cout<<"total "<<pool.size()<<"nodes"<<endl;
#endif
}