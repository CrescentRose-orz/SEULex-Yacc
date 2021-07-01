#ifndef LR_HEADER
#define LR_HEADER
#include"DataStructure.h"
#include"header.h"
#include"LR_Node.h"
#include"Logger.h"
#include"basicFA.hpp"
#include"visualLR.h"
using namespace std;


class visualLR;
class LR:public basicFA<LR_Node>{

private:
    // queue<NFA_eclosure> q;
    // unordered_map<eclosureHash,int,hashFunction> DFAMap;
    // unordered_map<eclosureHash,int,hashFunction> vis;

public:
    unordered_multimap<eclosureHash,int,hashFunction> concentricItem;
    unordered_set<eclosureHash,hashFunction> allKeys;
    int tail = 0;
    LR();
    LR(string s);
    LR(Logger &log);
    /**
     * @brief 新建一个LR节点 访问方式：
     * LR myLR；
     * int idx = myLR.add();
     * myLR[idx].addProducer(xxxx);
     * @return int 新建LR节点的idx
     */
    int add();
    /**
     * @brief 将一个LR节点插入LR_Node 
     * exp:
     * LR_Node myNode(myLR.vLR);
     * LR_Node.xxxxxx;
     * 
     * myLR.add(myNode);
     * 
     * @param node 插入的节点
     * @return int 插入节点的idx，访问方式与add()同
     */
    int add(LR_Node &node);
    /**
     * @brief 对两节点间建转移边
     * edge：from -> to 转移条件为id为c的符号
     * @exception invalid exception from to下标在该LR图中越界
     * @param from 起始节点id
     * @param to 目标节点id
     * @param c 转移符号id
     * @return int 0为正常退出，无意义
     */
    int addTrans(int from, int to, int c);
    /**
     * @brief 返回LR起始点id
     * yacc中固定为0，即head()始终返回0
     * 
     * @return int LR起始点id
     */
    int head(){
        return 0;
    }
    // bool exist(NFA_eclosure &_e);
    // int idx(NFA_eclosure &_e);
    // int insert(NFA_eclosure &_e);
    // void expandEclosure(NFA_eclosure &nowE);
    // void generateCode(fstream &file);
    /**
     * @brief 开始构造LR，需要已插入首项
     * @exception invalid_argument LR图中不存在首项
     * @return int LR图节点数
     */
    int consturctLR();
    void printVisualLR(fstream &fout);
    /**
     * @brief 分类同心项
     * 
     */
    void checkSame();
    /**
     * @brief 返回对于哈希为concentric的起始迭代器
     * 
     */
    auto getConcentricBegin(eclosureHash concentric);
    /**
     * @brief 返回对于哈希为concentric的结束迭代器
     * 
     */
    auto getConcentricEnd(eclosureHash concentric); 
    /**
     * @brief 构造LALR规约表并生成y.tab.h和y.tab.c
     * @exception LALR规约表中存在冲突
     * @return int 构建状态，0表示成功构建，1表示构建失败
     */
    int constructParsingTable();
};



#endif