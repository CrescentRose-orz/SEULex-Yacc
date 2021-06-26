#include"LR_Node.h"


#ifdef VISUAL
LR_Node::LR_Node(visualLR<int> &vLR):fa(vLR){}
#endif

void LR_Node::addTrans(int target,int c){
    if (state.count(c)){
        throw invalid_argument("add multi trans in LR");
    }
    state.insert({c,target});
    #ifdef VISUAL

    #endif
}
int LR_Node::getTrans(int c){
    return state.count(c)?state[c]:-1;
}

void LR_Node::addProducer(LR_Producer producer){
    if (!producers.count(producer.producer)){
        producers[producer] = producer;
        coreHash.add(producer);
        for (auto &look:producer.lookAhead){
            LR_Node_Hash.add(producer.getIdentifier(look));
        }
        return;
    }
    for (auto &lookAhead:producer.lookAhead){
        producers[producer].addLookAhead(lookAhead,LR_Node_Hash);
    }

}

eclosureHash LR_Node::core(){return coreHash;}

int LR_Node::getTrans(int c){return state.count(c)?state[c]:-1;}

void LR_Node::solveEclosure(){
unordered_set<long long> vis;//某LR产生式+某终结符是否已经展开过
queue<LR_Producer> q;//待展开的LR产生式
    for (auto &pr : producers){
        for (auto &lookAhead :pr.second.lookAhead){
            q.push(LR_Producer(pr.second,pr.second.nowPlace,lookAhead));
            vis.insert(LR_Producer::getIdentifier(pr.second,pr.second.nowPlace,lookAhead));
        }
    }
    while (!q.empty()){
        LR_Producer &now = q.front();
        if (isTerminal(now.getNext())){ // 为结尾-1亦满足条件(-1<TNBound)
            continue;
        }
        for (auto &newProducer:LHSToPos[now.getNext()]){//此写法默认LHSToPos指一个非终结符对应的所有产生式id，待明确
            for (auto &look:now.lookAhead ){   
                if (!vis.count(LR_Producer::getIdentifier(newProducer,0,look))){
                    q.push(LR_Producer(newProducer,0,look));
                    addProducer(LR_Producer(newProducer,0,look));
                    vis.insert(LR_Producer::getIdentifier(newProducer,0,look));

                }
            }
        }
        q.pop();
    }
}