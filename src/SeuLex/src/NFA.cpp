#include"NFA.h"


NFA::NFA():nowHash(0){
        add();
}

NFA::NFA(Logger &log):basicFA(log),nowHash(0){
        add();
}



int NFA::add(){
        WrLock(Wrlock);
        pool.emplace_back(NFA_Node(vFA));
        pool[tail].idx = tail;
        pool[tail].setHash(nowHash);
        nowHash.inc();
        return tail++;       
}

int NFA::add(NFA_Node node){
        WrLock(Wrlock);
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
        #ifdef DEBUG
        stringstream s;
        s<<RE<<" to"<<" NFA";
        logger.customMSG(s.str());
        #endif
        NFA_Cluster &&rt = NFA_Cluster::RE2NFA(RE,*this,_action);
        {
            WrLock(Wrlock);
            pool[0].addTrans(rt.head,eps);
            //pool[rt.tail].setAction
        }
}