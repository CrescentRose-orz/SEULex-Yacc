#include"LR.h"



LR::LR(){
    tail = 0;
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
        q.pop();
        //pool[now].
        for (auto &next:pool[now].allNexts){
            LR_Node tmp;
            auto iterPair = pool[now].nextPros.equal_range(next);
            for (auto iter = iterPair.first; iter != iterPair.second; ++iter){
                if (!pool[now].producers[iter->second].isEnd()){
                    tmp.addProducer(pool[now].producers[iter->second].move());
                }
            }
            if (vis.count(tmp.getHash())){
                newId = vis[tmp.getHash()];
            } else {
                eclosureHash oldHash = tmp.getHash();
                tmp.solveEclosure();
                if (wholeLRNode.count(tmp.getHash())){
                    newId = wholeLRNode[tmp.getHash()];
                } else {
                    newId = add(tmp);
                    q.push(newId);
                    wholeLRNode[tmp.getHash()] = newId;
                }
                vis[oldHash] = newId;
            }
            addTrans(now,newId,next);
        }
    }
    return  pool.size();
}