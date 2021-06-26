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
    
}

eclosureHash LR_Node::core(){return coreHash;}

int LR_Node::getTrans(int c){return state.count(c)?state[c]:-1;}

void LR_Node::solveEclosure(){


}