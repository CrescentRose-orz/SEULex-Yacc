#include"DataStructure.h"
#include"header.h"
#include"LR_Node.h"
#include"Logger.h"
#include"basicFA.hpp"
using namespace std;




class LR:public basicFA<LR_Node>{

private:
    _mutex_type mapMutex;
    // queue<NFA_eclosure> q;
    // unordered_map<eclosureHash,int,hashFunction> DFAMap;
    // unordered_map<eclosureHash,int,hashFunction> vis;
public:
    int tail = 0;
    LR();
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
     * @brief 将一个LR节点插入LR_Node 注意构造该节点需要传入相同的可视化对象
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
    int head();
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
};



