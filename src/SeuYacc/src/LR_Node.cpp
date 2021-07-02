#include"LR_Node.h"


#ifdef VISUAL
LR_Node::LR_Node(){}
LR_Node::LR_Node(const LR_Node &other){
    allNexts = other.allNexts;
    allEnds = other.allEnds;
    nextPros = other.nextPros;
    producers = other.producers;
    coreHash = other.coreHash;
    LR_Node_Hash = other.LR_Node_Hash;
    state = other.state;
    idx = other.idx;
}
LR_Node& LR_Node::operator = (const LR_Node&other){
    allNexts = other.allNexts;
    allEnds = other.allEnds;
    nextPros = other.nextPros;
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

void LR_Node::addProducer(LR_Producer producer){
int producerID = (producer.producer<<maxLen)+producer.nowPlace;
    if (!producers.count(producerID)){
        #ifdef DEBUG_FOR_ECL
        cout<<"OH New Producer!!"<<endl;
        cout<<"get new:";
        producer.print();
        cout<<endl;
        #endif
        producers[producerID] = producer;
        #ifdef DEBUG_Node
        cout<<"add producer "<<I2S(getLeft(producer.producer))<<" -> ";

        for(auto &id:getRight(producer)){
            cout<<I2S(id)<<" ";
        }
        cout<<endl;
        #endif
        if (!producer.isEnd()){
            nextPros.insert({producer.getNext(),producer});
            allNexts.insert(producer.getNext());
            #ifdef DEBUG_Node
            cout<<"gain new next "<<producer.getNext()<<" "<<(producer.getNext())<<endl;
            cout<<nextPros.count(producer.getNext()) <<endl;
            #endif
        } else {
            allEnds.insert(producer);
        }
        coreHash.add(producer);
        for (auto &look:producer.lookAhead){
            LR_Node_Hash.add(producer.getIdentifier(look));
        }
        return;
    }
    #ifdef DEBUG_FOR_ECL
    cout<<"add old producer with new look:";
    producer.print();
    cout<<endl;
    #endif
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
            q.push(LR_Producer(pr.second, pr.second.nowPlace, lookAhead));
            vis.insert(LR_Producer::getIdentifier(pr.second, pr.second.nowPlace, lookAhead));
        }
    }
    while (!q.empty()){//对所有待扩展产生式扩展
        LR_Producer now = q.front();
        q.pop();
        if (isTerminal(now.getNext())){ //若点后为终结符或末尾，不扩展 为结尾-1亦满足条件(-1<TNBound)
            continue;
        }
        //否则为非终结符，遍历所有该非终结符为左符号的产生式
            unordered_set<int> lookAhead;
            vector<int> symbols;
            for (int i = now.nowPlace + 1; i < now.getLength(); ++i){
                symbols.push_back(getRight(now)[i]);
            }
            getFirst(symbols,lookAhead);
            if (lookAhead.empty()){
                cout<<"error lookahead got empty! at"<<endl;
                cout<<"input is"<<endl;
                for (auto id:symbols){
                    cout<<I2S(id)<<" ";
                }
                cout<<endl;
                print();
                cout<<endl;
            }
            if (lookAhead.count(-1)){
                lookAhead.erase(-1);
                for (auto &look:now.lookAhead){
                    lookAhead.insert(look);
                }
            }
            #ifdef DEBUG_Node
            cout<<"processing on producer start with "<<I2S(now.getNext())<<endl;;
            #endif
            for (auto newProducer:LHSToPos[now.getNext()]){//此写法默认LHSToPos指一个非终结符对应的所有产生式id，待明确
            #ifdef DEBUG_FOR_ECL
                cout<<"adding producer begin with "<<I2S(now.getNext())<<endl;
            #endif
                for (auto &look:lookAhead ){   //加入计算得的向前看符后，若闭包内不存在，则加入闭包和扩展队列
                    if (!vis.count(LR_Producer::getIdentifier(newProducer, 0, look))){
                        q.push(LR_Producer(newProducer, 0, look));
                        addProducer(LR_Producer(newProducer, 0, look));
                        vis.insert(LR_Producer::getIdentifier(newProducer, 0, look));
                        #ifdef DEBUG_FOR_ECL
                        cout<<"adding "<<endl;
                        LR_Producer(newProducer, 0, look).print();
                        cout<<endl<<"-------------"<<endl;
                        #endif
                    }
                    #ifdef DEBUG_FOR_ECL 
                    else {
                        cout<<"already has"<<endl;
                        LR_Producer(newProducer, 0, look).print();
                        cout<<endl<<"-------------"<<endl;
                        if (getLeft(newProducer)==S2I("pointer")){
                            cout<<"already has pointer!"<<endl;
                            print();
                            cout<<endl;
                        }
                    }
                    #endif
            }
        }
    }
}

eclosureHash LR_Node::getHash(){
    return LR_Node_Hash;
}