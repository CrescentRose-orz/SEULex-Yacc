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
    int add();
    int add(LR_Node node);
    int addTrans(int from, int to, int c);
    int head();
    // bool exist(NFA_eclosure &_e);
    // int idx(NFA_eclosure &_e);
    // int insert(NFA_eclosure &_e);
    // void expandEclosure(NFA_eclosure &nowE);
    // void generateCode(fstream &file);
};



