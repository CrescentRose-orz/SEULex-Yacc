#include"LR_Node.h"


#ifdef VISUAL
LR_Node::LR_Node(visualLR<int> &vLR):vfa(vLR){}
LR_Node::LR_Node(const LR_Node &other):vfa(other.vfa){
    producers = other.producers;
    coreHash = other.coreHash;
    LR_Node_Hash = other.LR_Node_Hash;
    state = other.state;
    idx = other.idx;
}
LR_Node& LR_Node::operator = (const LR_Node&other){//赋值构造函数不会传递vfa别名，故只能用作临时变量，不可参与可视化！
    producers = other.producers;
    coreHash = other.coreHash;
    LR_Node_Hash = other.LR_Node_Hash;
    state = other.state;
    idx = other.idx;
    return *this;
}
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
    if (!producers.count(getLeft(producer))){
        producers[getLeft(producer)] = producer;
        coreHash.add(producer);
        for (auto &look:producer.lookAhead){
            LR_Node_Hash.add(producer.getIdentifier(look));
        }
        return;
    }
    for (auto &lookAhead:producer.lookAhead){
        producers[getLeft(producer)].addLookAhead(lookAhead,LR_Node_Hash);
    }

}

eclosureHash LR_Node::core(){return coreHash;}

int LR_Node::getTrans(int c){return state.count(c)?state[c]:-1;}

void LR_Node::solveEclosure(){
unordered_set<long long> vis;//某LR产生式+某终结符是否已经展开过
queue<LR_Producer> q;//待展开的LR产生式
    for (auto &pr : producers){
        for (auto &lookAhead :pr.second.lookAhead){
            q.push(LR_Producer(pr.second, pr.second.nowPlace, lookAhead));
            vis.insert(LR_Producer::getIdentifier(pr.second, pr.second.nowPlace, lookAhead));
        }
    }
    while (!q.empty()){//对所有待扩展产生式扩展
        LR_Producer &now = q.front();
        if (isTerminal(now.getNext())){ //若点后为终结符或末尾，不扩展 为结尾-1亦满足条件(-1<TNBound)
            continue;
        }
        //否则为非终结符，遍历所有该非终结符为左符号的产生式
        for (auto &newProducer:LHSToPos[now.getNext()]){//此写法默认LHSToPos指一个非终结符对应的所有产生式id，待明确
            for (auto &look:now.lookAhead ){   //加入计算得的向前看符后，若闭包内不存在，则加入闭包和扩展队列
                if (!vis.count(LR_Producer::getIdentifier(newProducer, 0, look))){
                    q.push(LR_Producer(newProducer, 0, look));
                    addProducer(LR_Producer(newProducer, 0, look));
                    vis.insert(LR_Producer::getIdentifier(newProducer, 0, look));

                }
            }
        }
        q.pop();
    }
}