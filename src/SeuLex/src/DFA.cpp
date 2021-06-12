#include"DFA.h"

NFA_eclosure::NFA_eclosure(const NFA_eclosure & other):_NFA(other._NFA){
    NFAs= other.NFAs;
    hash = other.hash;                     
}

NFA_eclosure::NFA_eclosure(NFA &_NFA):hash(),_NFA(_NFA){}
    //NFA_eclosure(int idx);//:hash(basicHash(idx));
void NFA_eclosure::add(int idx){
    NFAs.insert(idx);
    hash.add(_NFA[idx].hash());
    RdLock(_NFA.Wrlock);
    if (_NFA[idx].valid()){
        if (_action.getIdx() == 0){
            _action = _NFA[idx].getAction();
        } else if (_action < _NFA[idx].getAction()){
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
        system("pause");
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
        if (_e._action.getIdx() != 0){
            pool[rt].setAction(_e._action);
        }
    }
    return rt;
}

NFA_eclosure NFA_eclosure::move(int &c){
NFA_eclosure newE(_NFA);
    for (auto &t:NFAs){
        RdLock(_NFA.Wrlock);
        vector<int> &&rt = _NFA[t].getTrans(c);
        for (auto &newt:rt){
            if (!newE.has(newt)){
                newE.add(newt);
            }
        }
    }
    return newE;
}

void DFA::expandEclosure(NFA_eclosure &nowE){
    for (int i = 0; i <= charSetMAX; ++i){
        NFA_eclosure &&newE = nowE.move(i);
        int to;
        bool flag;
        {
            #ifdef USE_MULTITHREAD
            RdLock(visMutex);
            #endif
            flag = !vis.count(newE.hash);
        }
        if (flag){
            eclosureHash tmp = newE.hash;
            newE.expandEclosure();
            if (newE.NFAs.empty()){
                continue;
            }
            if (!exist(newE)){
                to = insert(newE);
                #ifdef USE_MULTITHREAD
                ++taskCnt;
                //cout<<"add task!"<<endl;
                #endif
                {
                    #ifdef USE_MULTITHREAD    
                    WrLock(qMutex);
                    #endif
                    q.push(newE);
                    task.signal();
                }
                {
                    #ifdef USE_MULTITHREAD
                    WrLock(visMutex);
                    #endif
                    vis[tmp] = to;
                }
            }
        } else {
            to = vis[newE.hash];
        }
        addTrans(idx(nowE),to,i);            
        #ifdef DEBUG
        stringstream s;
        s<<now<<" to " <<newE<<" with "<<i;
        logger.customMSG(s.str());
        #endif
    }
    #ifdef USE_MULTITHREAD
    --taskCnt;
    #endif
}
#ifdef USE_MULTITHREAD



void DFA::process(){
    while (taskCnt){
        task.wait();
        if (taskCnt == 0){
            break;
        }
        qMutex.lock();
        if (!q.empty()){
            --idleCnt;
            NFA_eclosure now = q.front();
            q.pop();
            qMutex.unlock();
            expandEclosure(now);
            ++idleCnt;
        } else {
            qMutex.unlock();
        }
    }    
    return;
}



// void DFA::IncTask(){
//     RdLock(taskM);
//     ++taskCnt;
// }
// void DFA::DecTask(){
//     WrLock(taskM);
//     --taskCnt;
// }

void DFA::MULTITHREAD_expandEclosure(NFA_eclosure nowE){
    for (int i = 0; i <= charSetMAX; ++i){
        NFA_eclosure &&newE = nowE.move(i);
        int to;
        bool flag;
        {
            RdLock(visMutex);
            flag = !vis.count(newE.hash);
        }
        if (flag){
            eclosureHash tmp = newE.hash;
            newE.expandEclosure();
            if (newE.NFAs.empty()){
                continue;
            }
            if (!exist(newE)){
                to = insert(newE);
                //IncTask();
                {
                    WrLock(visMutex);
                    vis[tmp] = to;                
                }
                {
                    WrLock(tpMutex);
                    tp.commit(bind(&DFA::MULTITHREAD_expandEclosure,this,newE));
                }
            }
        } else {
            RdLock(visMutex);
            to = vis[newE.hash];
        }
        addTrans(idx(nowE),to,i);            
        #ifdef DEBUG
        stringstream s;
        s<<now<<" to " <<newE<<" with "<<i;
        logger.customMSG(s.str());
        #endif
    }
    //DecTask();
}

#endif
void DFA::NFA2DFA(NFA& _NFA){
NFA_eclosure startPoint(_NFA);
    startPoint.add(_NFA.head());
    startPoint.expandEclosure();    
    q.push(startPoint);
    int st = insert(startPoint);
    #ifdef DEBUG
    stringstream s;
    s<<"begin with" << startPoint;
    logger.customMSG(s.str());
    #endif
    vis.insert({startPoint.hash,st});
    #ifdef USE_MULTITHREAD
    // int tmpp = 0;
    // taskCnt = 1;
    // tp.commit(bind(&DFA::MULTITHREAD_expandEclosure,this,startPoint));
    // for (int i = 1 ; i < 10000000;++i);

    // while(1){
    //     #ifdef DEBUG
    //     ++tmpp;
    //     if (tmpp == 1000000){
    //         RdLock(taskM);
    //         cout<<taskCnt<<" "<<tp.idlCount()<<endl;
    //         tmpp = 0;
    //     } 
    //     #endif
    //     if (tmpp == 10000000){
    //         cout<<taskCnt<<" "<<tp.idlCount()<<endl;
    //         tmpp = 0;
    //     }
    //     ++tmpp;
    // }
    // tp.close();
    thread th[THREADCNT];
    taskCnt = 1;
    idleCnt = THREADCNT;
    for (int i = 0; i < THREADCNT; ++i){
        th[i] = thread(bind(process,this));
    }
    while(1){
        for(int i = 0 ; i < 10000000;++i){

        }
        if (idleCnt == THREADCNT&&taskCnt == 0){
            break;
        }
    }
    cout<<"all task is done! "<<taskCnt<<" "<<idleCnt<<" "<<q.size()<<endl;
    task.signalAll();
    for (int i = 0; i < THREADCNT; ++i){//join failed why?
        cout<<"try to join "<<i<<" "<<idleCnt<<endl;
        th[i].detach();
        cout<<"join "<<i<<"ok"<<endl;
    }
    #else
    while (!q.empty()){
        NFA_eclosure & now = q.front();
        expandEclosure(now);
        q.pop();
    }
    #endif
    cout<<"total "<<pool.size()<<"nodes"<<endl;
}
