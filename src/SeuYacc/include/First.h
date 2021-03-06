#include "header.h"
#include "DataStructure.h"

using namespace std;

// 该重载函数对单个字符求其first，是一个递归算法
// 该重载函数的输入依次为所求字符，返回结果集，以及一个维护的已求过first的非终结符集合
void getFirst(const int& symbol, unordered_set<int>& resultSet, unordered_set<int>& processedSymbol);

// 该重载函数求取所有符号的first并存入数据结构First中供后续调用
void getFirst();

// 该重载函数对字符串求first
// 该重载函数的输入依次为所求字符串，返回结果集
void getFirst(const vector<int>& symbols, unordered_set<int>& resultSet);



void solveFirst();


void solveSingle(int idx,unordered_set<int> &vis);

void solveSingleFirst(int idx,bool &changesFlag);

