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
    #ifdef USE_MULTITHREAD
    RdLock(mapMutex);
    #endif
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
        #ifdef USE_MULTITHREAD
        WrLock(mapMutex);
        #endif
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

// struct _item{
//     unordered_set<int> num;
//     eclosureHash hash;
// }Item;

DFA DFA::minimize(){
DFA rt(logger);
queue<unordered_set<int>> q;
unordered_set<int> nonterminal;
unordered_map<int,unordered_set<int>>  Terminal;
unordered_map<int,int> belong;
unordered_map<int,int> finalBelong;
int cnt = 1;
int st = -1;
int stable = 0;
unordered_map<int,int> member;
    belong[-1] = -1;
    for (int i = 0 ; i < pool.size();++i){  
        if (!pool[i].valid()){
            nonterminal.insert(i);
        } else {
            Terminal[pool[i].act.getIdx()].insert(i);
        }
    }
    if (!nonterminal.empty()){
        ++st;
        for (auto &id:nonterminal){
            belong[id] = st;
        }
        q.push(nonterminal);
    }
    for (auto &item:Terminal){
        ++st;
        #ifdef debug_MINI 
        stringstream ss;
        ss<<"group "<<st<<" has:";
        //logger.customMSG(ss.str());
        #endif
        for (auto &id:(item.second)){
            belong[id] = st;
        #ifdef debug_MINI 

        stringstream ss;
        ss<<id<<" has act:"<<pool[id].act.getIdx();
       
        logger.customMSG(ss.str());
        #endif
        }
        q.push(item.second);
    }
    while(!q.empty()){
        int flag = false;
        unordered_set<int> now = q.front();
        q.pop();
        if (now.size() != 1&&stable < q.size()){
            for (int i = 30; i <=charSetMAX; ++i){
                //int flag = pool[*now.cbegin()].getTrans(i);
                unordered_map<int,unordered_set<int>> newSplit;
                for (auto iter =now.cbegin();iter!=now.cend();++iter){
                    newSplit[belong[pool[*iter].getTrans(i)]].insert(*iter);
                }
                if (newSplit.size()>1){
                    flag = true;
                    stable = 0;
                    #ifdef debug_MINI 
                    cout<<"group: ";
                    for (auto p:now){
                        cout<<p<<" ";
                    }
                    cout<<"split into:"<<endl;
                    #endif
                    for (auto iter = newSplit.cbegin();iter != newSplit.cend();++iter){
                        ++st;

                        for (auto &id:iter->second){
                            #ifdef debug_MINI 
                            cout<<id<<" ";
                            #endif
                            belong[id] = st;
                        }
                        #ifdef debug_MINI 
                        cout<<endl;
                        #endif
                        q.push(iter->second);
                    }
                    break;
                } else {

                }
            }
        }
        if (flag ){
            continue;
        }
        if (stable < q.size()){
            #ifdef debug_MINI 
            cout<<"group: ";
            for (auto p:now){
                cout<<p<<" ";
            }
            cout<<"ok"<<endl;
            #endif
            q.push(now);
            ++stable;
            #ifdef debug_MINI 
            cout<<"now stable "<<stable<<" now queue"<<q.size()<<endl;
            #endif
            continue;
        }
        //cout<<" all stable!"<<endl;
        //if (flag){continue;}
        if (!now.count(0)){
            member[cnt]=(*now.cbegin());
            #ifdef debug_MINI 
            map<int,int> dflag;
            for (auto iter = pool[*now.cbegin()].stateBegin(); iter != pool[*now.cbegin()].stateEnd();++iter){
                dflag[iter->first] = belong[iter->second];
            }
            stringstream ss;
            ss<<"final group "<<cnt<<" with delegate "<<*now.cbegin() <<" has ";
            #endif
            for (auto &id:now){
                int pcnt = 0;
                finalBelong[id] = cnt;
                #ifdef debug_MINI 

                ss<<" "<<id;
                for (auto iter = pool[*now.cbegin()].stateBegin(); iter != pool[*now.cbegin()].stateEnd();++iter){
                    if (dflag[iter->first] != belong[iter->second]){
                        cout<<"ERROR to group "<<member[cnt]<<" with "<<id<<endl;
                        cout<<"input "<<(char)iter->first<<" get "<<dflag[iter->first]<<" while "<<id<<" get "<<belong[iter->second]<<endl;
                    }
                    ++pcnt;
                }
                if (pcnt != dflag.size()){
                    cout<<"ERROR to group "<<member[cnt]<<" with "<<id<<" delegate has "<<dflag.size()<<" trans while "<<id<<" has "<<pcnt<<endl;
                }
                #endif
            }
            ++cnt;
            #ifdef debug_MINI 

            logger.customMSG(ss.str());
            #endif
        } else {
            #ifdef debug_MINI
            stringstream ss;
            ss<<"final group "<<0<<" with delegate "<<*now.cbegin() <<" has ";
            #endif
            for (auto &id:now){
                finalBelong[id] = 0;
                #ifdef debug_MINI
                ss<<" "<<id;
                #endif
            }
            member[0]=(*now.cbegin());
            #ifdef debug_MINI 
            logger.customMSG(ss.str());
            #endif
        }
    }
    for (int i = 0; i < cnt; ++i){
        rt.add();
        for (auto iter = pool[member[i]].stateBegin(); iter != pool[member[i]].stateEnd();++iter){
            
            if (finalBelong[iter->second]>=cnt){
                #ifdef debug_MINI 
                stringstream ss;
                ss<<"try to add edge to"<<finalBelong[iter->second]<<" while total node is"<<cnt<<endl;

                throw invalid_argument(ss.str());
                                #endif
            }
            #ifdef debug_MINI
            {
                stringstream ss;
                ss<<"add edge from "<<i<<" to "<<finalBelong[iter->second]<<" with c ="<<iter->first;
                logger.customMSG(ss.str());
            }
            #endif
            rt.addTrans(i,finalBelong[iter->second],iter->first);
        }
        if (pool[member[i]].valid()){
            #ifdef debug_MINI
            {
                stringstream ss;
                ss<<"delegate "<<member[i]<<" has act "<<pool[member[i]].act.getIdx()<<" for group"<<i<<endl;
                logger.customMSG(ss.str());
            }
            #endif
            rt[i].setAction(pool[member[i]].act);
        } 

        for (auto iter = rt.pool[i].stateBegin();iter !=rt.pool[i].stateEnd(); ++iter){ 
            #ifdef DEBUG
            stringstream s;
            s<<"get node "<<i<<"char  "<<iter->first<<" to state"<<iter->second<<endl;
            logger.customMSG(s.str());
            #endif
                    #ifdef debug_MINI
            stringstream ss;
            ss<<"node "<<i<<" has edge"<<iter->first<<" to "<<iter->second<<endl;
            logger.customMSG(ss.str());
            #endif
        }
        #ifdef debug_MINI
            stringstream ss;
            ss<<"node "<<i<<" has act"<<rt.pool[i].act.getIdx();
            logger.customMSG(ss.str());
        #endif
    }
    return rt;
}








void DFA::generateCode(fstream &file,int flag){
    int buff[charSetMAX + 1];
    file<<"int yyLexerNext["<<pool.size()<<"]["<<charSetMAX + 1<<"] = {";
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
    file<<R"(
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
    if (yyEOF >= 1){
        yyEOF = 2;
        return '#';
    }
    buff[_leng++] = c;
    while (c != -1 &&yyLexerNext[nowState][c] != -1){
        nowState = yyLexerNext[nowState][c];
        if (act[nowState] != -1){
            {
                actIdx = act[nowState];
                yypos =ftell(yyin);// c == '\n'?ftell(yyin)-1:ftell(yyin);
                yyleng = _leng;
            }
        } else {
            buff[_leng]='\0';
        }
        c = fgetc(yyin);
        if (c != EOF&& c !='\n'&& c!='\r' && c!=' ')
            buff[_leng++] = c;
    }
    if (c == -1){
        yyEOF = 1;
    }
    if (nowState == -1){
        fprintf(yyout,"ERROR CANNOT MATCH at %s!\n",buff);
    } else {
        buff[_leng] = '\0';
        buff[yyleng] = '\0';
        strcpy(yytext,buff);
        fseek(yyin,yypos,SEEK_SET);
    )";
    if (flag){
    file<<R"(
        return doAction(actIdx);
    }
})"<<'\n';
    }
}
