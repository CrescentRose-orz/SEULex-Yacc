#include<CONSTANT.h>
#include<map>
#include"header.h"
#include<vector>
#include"visualLR.hpp"
#include"Hash.hpp"
#include"LR_Producer.hpp"
#include "CONSANT.h"
#include"DataStructure.h"
using namespace std;




class LR_Node{
private:
#define _state_type map<int,int>
     _state_type state;
    #ifdef VISUAL
    visualLR<int> &vfa; 
    #endif
    /**
     * @brief int 产生式左边id - >产生式
     * 
     */
    unordered_map<int,LR_Producer> producers;
    /**
     * @brief int 转移符id - > 转移后，后移一项的产生式左边id
     * 
     */
    unordered_map<int,int> transResult; 
    /**
     * @brief 中心项哈希，相等说明为同心项
     * 
     */
    eclosureHash coreHash;
    eclosureHash LR_Node_Hash;
public:
    int idx;
    #ifdef VISUAL
    LR_Node(visualLR<int> &vLR);
    /**
     * @brief 拷贝构造函数
     * 
     * @param other 
     */
    LR_Node(const LR_Node &other);
    /**
     * @brief 赋值构造函数
     * 
     * @param other 
     * @return LR_Node 
     */
    LR_Node& operator = (const LR_Node&other);
    #else
    //LR_Node();
    #endif
    /**
     * @brief 对项集族做扩展
     * 
     */
    void solveEclosure();
    /**
     * @brief 添加一个产生式（若已存在，给已存在的产生式添加向前看符）
     * 内部提供判重检查
     * 
     * @param producer 
     */
    void addProducer(LR_Producer producer);
    /**
     * @brief 添加转移边c到节点target
     * 
     * @param target 转移到的LR节点id
     * @param c 符号id
     */
    void addTrans(int target,int c);
    /**
     * @brief 返回读入符号c的转移后状态，若非法返回-1
     * 
     * @param c 符号id
     * @return int 转移后LR节点id
     */
    int getTrans(int c);
    /**
     * @brief 返回LR项集的中心项哈希\n
     *   用法：LR_Node n1,n2;
     *   LALR判同心项集：n1.core()==n2.core()
     * @return eclosureHash 
     */
    eclosureHash core();
};


