#include<bits/stdc++.h>
#include<action.hpp>
using namespace std;
const int NFA_t =258;


class NFA_Node{
private:
    multimap<int,int> state;
    action act;
    bool hasAction;
public:
    NFA_Node(){
        hasAction = -1;
    }
    NFA_Node(action act):act(act){
        hasAction = 1;
    }
    void setAction(action act){
        this->act = act;
        hasAction = 1;
    }
    void addMultiTrans(int target,int l,int r);
    void addTrans(int target,int l,int r);
    int transfer(int input);
    bool valid(){
        return hasAction == -1 ? 0 : 1;
    }
};



class NFA_Cluster{
public:    
    int head;
    int tail;
    NFA_Cluster(int head,int tail){
        this->head = head;
        this->tail = tail;
    }
    NFA_Cluster(vector<NFA_Node> &buff,char op ,NFA_Cluster a ,NFA_Cluster b ){
        switch(op){
            case '|':

                break;
            case ' ':

                break;
            default:


        }

    }
    NFA_Cluster(vector<NFA_Node> &buff,char op ,NFA_Cluster a ){
        switch(op){
            case '*':
                
                break;
            case '?':

                break;
            case '+':

                break;
            default:
                // todo: throw exception 
                break;
        }
    }
    NFA_Cluster(vector<NFA_Node> &buff ,string quotation , int l = 0,int r = -1){


    }
};
