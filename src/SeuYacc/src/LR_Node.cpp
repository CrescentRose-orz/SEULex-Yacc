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
            nextPros.insert({producer.getNext(),producerID});
            allNexts.insert(producer.getNext());
            #ifdef DEBUG_Node
            cout<<"gain new next "<<producer.getNext()<<" "<<(producer.getNext())<<endl;
            cout<<nextPros.count(producer.getNext()) <<endl;
            #endif
        } else {
            allEnds.insert(producerID);
        }
        coreHash.add(producerID);
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
        producers[producerID].addLookAhead(lookAhead,LR_Node_Hash);
    }

}

eclosureHash LR_Node::core(){return coreHash;}

int LR_Node::getTrans(int c){return state.count(c)?state[c]:-1;}

void LR_Node::solveEclosure(){
unordered_set<long long> vis;//???LR?????????+?????????????????????????????????
queue<LR_Producer> q;//????????????LR?????????
    for (auto &pr : producers){
        for (auto &lookAhead :pr.second.lookAhead){
            q.push(LR_Producer(pr.second, pr.second.nowPlace, lookAhead));
            vis.insert(LR_Producer::getIdentifier(pr.second, pr.second.nowPlace, lookAhead));
        }
    }
    while (!q.empty()){//?????????????????????????????????
        LR_Producer now = q.front();
        q.pop();
        if (isTerminal(now.getNext())){ //?????????????????????????????????????????? ?????????-1???????????????(-1<TNBound)
            continue;
        }
        //???????????????????????????????????????????????????????????????????????????
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
            for (auto newProducer:LHSToPos[now.getNext()]){//???????????????LHSToPos?????????????????????????????????????????????id????????????
            #ifdef DEBUG_FOR_ECL
                cout<<"adding producer begin with "<<I2S(now.getNext())<<endl;
            #endif
                for (auto &look:lookAhead ){   //??????????????????????????????????????????????????????????????????????????????????????????
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