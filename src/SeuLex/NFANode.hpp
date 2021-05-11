#include<bits/stdc++.h>
using namespace std;
const int NFA_t =258;

class action{

};


class NFA_Node{
private:
    multimap<int,int> state;
    action act;
    bool hasAction;
public:
    NFA_Node(){
        hasAction = 0;
    }
    NFA_Node(action act):act(act){
        hasAction = 1;
    }
    void addMultiTrans(int target,int l,int r);
    void addTrans(int target,int l,int r);
    int transfer(int input);
};

