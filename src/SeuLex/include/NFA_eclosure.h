#ifndef NFA_CLUSTER
#define NFA_CLUSTER
#include"NFA.h"
#include"bits/stdc++.h"
#include"Hash.hpp"
using namespace std;





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
    #ifdef DEBUG2
    int check();
    #endif
};
#endif