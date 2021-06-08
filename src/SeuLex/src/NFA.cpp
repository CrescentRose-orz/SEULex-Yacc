#include"NFA.h"


NFA::NFA(){
        add();
}

int NFA::add(){
        unique_lock<mutex> lock(Wrlock);
        pool.emplace_back(NFA_Node(vNFA));
        pool[tail].idx = tail;
        return tail++;       
}

NFA_Node& NFA::operator[](int i){
        return pool[i];
    }
int NFA::add(NFA_Node node){
        unique_lock<mutex> lock(Wrlock);
        pool.emplace_back(node);
        pool[tail].idx = tail;
        pool[tail].update();
        return tail++;
}
int NFA::head(){
        return 0;
}

void NFA::addRE(string &RE,action _action){
        NFA_Cluster &&rt = NFA_Cluster::RE2NFA(RE,*this,_action);
        {
            lock_guard<mutex> lock(Wrlock);
            pool[0].addTrans(rt.head,eps);
        }
}