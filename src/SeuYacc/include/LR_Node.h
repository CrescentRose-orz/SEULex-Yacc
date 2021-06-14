#include<CONSTANT.h>
#include<map>
#include"header.h"
#include<vector>
#include"visualLR.hpp"
using namespace std;




class LR_Node{
private:
#define _state_type map<int,int>
     _state_type state;
    #ifdef VISUAL
    visualLR<int> &fa; 
    #endif
public:
    int idx;
    #ifdef VISUAL
    LR_Node(visualLR<int> &vLR);
    #else
    //LR_Node();
    #endif
    //添加转移边c到节点target
    void addTrans(int target,int c);
    // 返回读入符号c的转移后状态，若非法返回-1
    int getTrans(int c){
        return state.count(c)?state[c]:-1;
    }
};


