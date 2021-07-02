#include"DFA.h"


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
    #ifdef USE_MULTITHREAD
    WrLock(Wrlock);
    #endif
    if (to<0||to>1000){
        cout<<"ERROR! try to add"<<from<<" to "<<to<<" with "<<c<<"!"<<endl;
    }
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
        #ifdef USE_MULTITHREAD
        RdLock(mapMutex);
        #endif
        if (DFAMap[_e.hash]<0||DFAMap[_e.hash]>1000){
            cout<<"ERROR! try to return idx with"<<DFAMap[_e.hash]<<endl;
    }
        return DFAMap[_e.hash];
    } else {
        stringstream s;
        s<<_e<<"is not in the DFAMap!";
        system("pause");
        throw invalid_argument(s.str());
    }
}

    #ifdef DEBUG2
    int NFA_eclosure::check(){
        int idx = 0x7fff;
        for (auto &t:NFAs){
            if (_NFA[t].getAction().getIdx()!= -1){
                idx =min(idx, _NFA[t].getAction().getIdx());
            }
        }
        if (idx == 0x7fff){
            idx = -1;
        }
        return idx;
    }
    #endif

int DFA::insert(NFA_eclosure &_e){
    if (exist(_e)){
        stringstream s;
        s<<_e<<"already in the DFAMap!";
        throw invalid_argument(s.str());       
    }
    int rt;
    #ifdef DEBUG2
    if (!(_e.check()==_e._action.getIdx() )){
        cout<<"ERROR! not equal!"<<endl;
        cout<<_e.check()<<"expected but "<<_e._action.getIdx()<<"found! "<<endl;
    }
    #endif
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
                    #ifdef USE_MULTITHREAD
                    task.signal();
                    #endif
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
    q = queue<NFA_eclosure>();
    q.push(startPoint);
    int st = insert(startPoint);
    #ifdef DEBUG
    stringstream s;
    s<<"begin with" << startPoint;
    logger.customMSG(s.str());
    #endif
    vis.insert({startPoint.hash,st});
    #ifdef USE_MULTITHREAD
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
        NFA_eclosure  now = q.front();
        expandEclosure(now);
        q.pop();
    }
    #endif
    cout<<"total "<<pool.size()<<"nodes"<<endl;
}

    void DFA::generateCode(fstream &file){
        int buff[charSetMAX + 1];
        file<<"int next["<<pool.size()<<"]["<<charSetMAX + 1<<"] = {";
        for (int i = 0; i < pool.size(); ++i){
            memset(buff,-1,sizeof(buff));
            for (auto iter = pool[i].stateBegin();iter != pool[i].stateEnd(); ++iter){ 
                #ifdef DEBUG
                stringstream s;
                s<<"get node "<<i<<"char  "<<iter->first<<" to state"<<iter->second<<endl;
                logger.customMSG(s.str());
                #endif
                buff[iter->first] = iter->second;
            }
            file<<buff[0];
            for (int j = 1; j <= charSetMAX; ++j){
                file<<","<<buff[j];
            }
            if (i != pool.size()-1){
                file<<",\n";
            } else {
                file<<"};\n";
            }
        }
        file<<"int act["<<pool.size()<<"]= {";
        file<<pool[0].act.getIdx();
        for (int i = 1; i < pool.size(); ++i){
         file<<","<<pool[i].act.getIdx();   
        }
        file<<"};\n";
        file<<R"(#define INF 0x7fff;
        int yyleng;
        char yytext[1024];
        FILE* yyin;
        FILE* yyout;
        long yypos;
        int yyEOF;

        int input(){
            int c = fgetc(yyin);
            ++yypos;
            if (c == -1)
                return 0;
            return c;
        }


        int yyLex(){
            int nowState = 0 , c;
            long _lastMatch = -1;
            int lastAcState = -1;
            int actIdx = INF;
            int _leng = 0;
            char buff[1024];
            memset(yytext,0,sizeof(yytext));
            yyleng = 0;
            c = fgetc(yyin);
            buff[_leng++] = c;
            while (c != -1 &&next[nowState][c] != -1){
                nowState = next[nowState][c];
                if (act[nowState] != -1){
                    {
                        actIdx = act[nowState];
                        yypos = c == '\n'?ftell(yyin)-1:ftell(yyin);
                        yyleng = _leng;
                    }
                } else {
                	buff[_leng]='\0';
                	printf("unfortunate that %s didn't have action\n",buff);
				}
                c = fgetc(yyin);
                if (c != EOF)
                    buff[_leng++] = c;
            }
            if (c == -1){
                yyEOF = 1;
            }
            if (nowState == -1){
                fprintf(yyout,"ERROR CANNOT MATCH at %s!\n",buff);
            } else {
            	buff[_leng] = '\0';
            	printf("stop at %ld %s\n",yypos,buff); 
                buff[yyleng] = '\0';
                strcpy(yytext,buff);
                fseek(yyin,yypos,SEEK_SET);
                return doAction(actIdx);
            }
        })"<<'\n';
        
    }
