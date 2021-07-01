#include"LR.h"



LR::LR(){
    tail = 0;
}
LR::LR(string s){
    logger = Logger(s);
    logger.init();
}
LR::LR(Logger &log):basicFA<LR_Node>(log){}

int LR::add(){
    pool.push_back(LR_Node());
    pool[tail].idx = tail;
    return tail++;
    // todo vlr update
}

int LR::add(LR_Node &node){
    node.idx = tail;
    pool.push_back(node);
    return tail++;
    // todo vlr update
}

int LR::addTrans(int from, int to, int c){
    if( from < 0 || to < 0 || from >= tail || to >= tail){
        throw invalid_argument("out of bound error occurs in adding trans edge in LR");
    }
    pool[from].addTrans(to , c);
    return 0;
}


int LR::consturctLR(){
unordered_map <eclosureHash, int, hashFunction> wholeLRNode;
unordered_map <eclosureHash, int, hashFunction> vis;
queue<int> q;
int newId;
bool flag = 1;
    if (pool.empty()){
        return -1;
    }
    q.push(0);
    while (!q.empty()){
        int now = q.front();
        LR_Node nowNode = pool[now];
        q.pop();
        #ifdef DEUBG
        {
            stringstream s;
            s<<"trying node "<<now<<endl;
            logger.customMSG(s.str());
            cout<<s.str()<<endl;
        }
        #endif
        //nowNode.
        for (auto next:nowNode.allNexts){
            #ifdef DEBUG
            {
                stringstream s;
                s<<"trying node "<<now<<" possible trans "<<next<<" "<<I2S(next)<<" which has "<<nowNode.nextPros.count(next)<<" producers"<<endl;
                logger.customMSG(s.str());
                cout<<s.str()<<endl;
            }
            #endif
            LR_Node tmp;
            auto iterPair = nowNode.nextPros.equal_range(next);
            if (iterPair.first == iterPair.second){
                cout<<"indeed none"<<endl;
                continue;
            }
            for (auto iter = iterPair.first; iter != iterPair.second; ++iter){

                {
                    #ifdef DEUBG
                    stringstream s;
                    s<<"trying node "<<now<<" possible producer "<<I2S(getLeft(nowNode.producers[iter->second]))<<"->";
                    for (auto &ss:getRight(nowNode.producers[iter->second])){
                        s<<I2S(ss)<<" ";
                    }
                    s<<endl;
                    logger.customMSG(s.str());
                    cout<<s.str()<<endl;
                    #endif
                }
                if (!nowNode.producers[iter->second].isEnd()){
                    tmp.addProducer(nowNode.producers[iter->second].move());
                }
            }
            if (vis.count(tmp.getHash())){
                #ifdef DEBUG
                cout<<"already exist!"<<endl;
                #endif
                newId = vis[tmp.getHash()];
            } else {
                eclosureHash oldHash = tmp.getHash();
                tmp.solveEclosure();
                if (wholeLRNode.count(tmp.getHash())){
                    #ifdef DEBUG
                    cout<<"whole already exist!"<<endl;
                    #endif
                    newId = wholeLRNode[tmp.getHash()];
                } else {
                    newId = add(tmp);
                    #ifdef DEBUG
                    cout<<"new eclosure "<<newId<<endl;
                    #endif
                    q.push(newId);
                    wholeLRNode[tmp.getHash()] = newId;
                }
                vis[oldHash] = newId;
            }
            addTrans(now,newId,next);
        }
    }
    stringstream s;
    #ifdef DEUBG
    s<<"totoal "<<pool.size()<<"lr1 nodes";
    #endif
    logger.customMSG(s.str());
    logger.save();
    cout<<s.str()<<endl;
    return  pool.size();
}
    void LR::printVisualLR(fstream &fout){
        visualLR vLR;
        vLR.print(fout,*this);
    }