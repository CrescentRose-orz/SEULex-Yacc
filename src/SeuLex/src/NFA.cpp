#include"NFA.h"


NFA::NFA():nowHash(0){
        add();
}

NFA::NFA(Logger &log):basicFA(log),nowHash(0){
        add();
}



int NFA::add(){
        unique_lock<shared_mutex> lock(Wrlock);
        pool.emplace_back(NFA_Node(vFA));
        pool[tail].idx = tail;
        nowHash.inc();
        return tail++;       
}

int NFA::add(NFA_Node node){
        unique_lock<shared_mutex> lock(Wrlock);
        pool.emplace_back(node);
        pool[tail].idx = tail;
        pool[tail].update(nowHash);
        nowHash.inc();
        return tail++;
}
int NFA::head(){
        return 0;
}

void NFA::addRE(string &RE,action _action){
        NFA_Cluster &&rt = NFA_Cluster::RE2NFA(RE,*this,_action);
        {
            lock_guard<shared_mutex> lock(Wrlock);
            pool[0].addTrans(rt.head,eps);
            //pool[rt.tail].setAction
        }
}