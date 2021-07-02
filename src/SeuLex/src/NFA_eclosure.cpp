#include"NFA_eclosure.h"


NFA_eclosure::NFA_eclosure(const NFA_eclosure & other):_NFA(other._NFA){
    NFAs= other.NFAs;
    hash = other.hash;                     
}

NFA_eclosure::NFA_eclosure(NFA &_NFA):hash(),_NFA(_NFA){}
    //NFA_eclosure(int idx);//:hash(basicHash(idx));
void NFA_eclosure::add(int idx){
    NFAs.insert(idx);
    hash.add(_NFA[idx].hash());
    #ifdef USE_MULTITHREAD
    RdLock(_NFA.Wrlock);
    #endif
    if (_NFA[idx].valid()){
        action &&other = _NFA[idx].getAction();
        if (_action.getIdx() == -1){
            _action = other;
        } else if (_action > other){
            _action = other;
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
            #ifdef USE_MULTITHREAD
            RdLock(_NFA.Wrlock);
            #endif
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


NFA_eclosure NFA_eclosure::move(int &c){
NFA_eclosure newE(_NFA);
    for (auto &t:NFAs){
        #ifdef USE_MULTITHREAD
        RdLock(_NFA.Wrlock);
        #endif
        vector<int> &&rt = _NFA[t].getTrans(c);
        for (auto &newt:rt){
            if (!newE.has(newt)){
                newE.add(newt);
            }
        }
    }
    return newE;
}
