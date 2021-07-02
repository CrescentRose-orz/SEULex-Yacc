#include "DFA.h"

class DFA;

class DFA_Node{
private:
#define _state_type map<int,int>
    /**
     * @brief <int c,int target> 输入c转移到target
     * 
     */
     _state_type state;
    bool hasAction = 0;
    #ifdef VISUAL

    visualFA<int> &fa; 
    vector<pair<int,int>> edge;
    bool aflag = 0;
    #endif
public:
    int idx;
    action act;  

    #ifdef VISUAL
    _state_type::const_iterator stateBegin(){
        return state.cbegin();
    }
    _state_type::const_iterator stateEnd(){
        return state.cend();
    }
    DFA_Node(visualFA<int> &_fa):fa(_fa){
        hasAction = 0;
        idx = -1;
        state.clear();
    }
    DFA_Node(action act,visualFA<int> &_fa):act(act),fa(_fa){
        hasAction = 1;
        idx = -1;
        state.clear();
    }
    void update(){
        for (auto &p:edge){
            fa.addEdge(idx, p.first, p.second);
        }
        if (aflag){
            fa.addNode(idx);
        }
    }
    #else
    DFA_Node(){
        hasAction = -1;
        idx = -1;
    }
    DFA_Node(action act):act(act){
        hasAction = 1;
        idx = -1;
    }
    #endif
    void setAction(action act){
        this->act = act;
        if (hasAction) return;
        hasAction = 1;        
        #ifdef VISUAL
        if (idx!=-1){
            fa.addNode(idx);
        } else {
            aflag = 1;
        }
        #endif

    }
    void addMultiTrans(int target,int l,int r){
        for (int i = l; i <= r; ++i){
            addTrans(target,i);
        }
    }
    void addTrans(int target,int c){
        if (state.count(c)){
            if (state[c] == target){
                return;
            }
            stringstream ss;
            ss<<"add multi trans in DFA No."<<idx<<" with c = "<<c<<" origin target: "<<state[c]<<", new target: "<<target<<endl;
            throw invalid_argument(ss.str());
        }
        #ifdef DEBUG2
        if(target<0||target>1000){
            cout<<"ERROR try to add trans to "<<target<<endl;
        }
        #endif
        state.insert({c,target});
        #ifdef VISUAL
        if( idx != -1){
            fa.addEdge(idx , target, c);
        } else {
            edge.push_back(make_pair(target,c));
        }
        #endif
    }
    bool valid(){
        return hasAction&&act.getIdx()!=-1;
    }

    int getTrans(int c){
        return state.count(c)?state[c]:-1;
    }
};
