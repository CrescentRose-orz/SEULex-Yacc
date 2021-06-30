#include"DataStructure.h"

std::vector<int> producerBaseIdx;

// 存放所有string与int相互映射关系
unordered_map<string, int> StrToInt;
unordered_map<int, string> IntToStr;

// 存放Int形式产生式->产生式在数据结构中的存放序号的映射关系
//unordered_map<vector<int>, int> ProToPos;

// Int形式产生式左部->Int形式产生式右部下标的映射关系
// 存的是translationRule—>下标的集合
unordered_map<int, vector<int>> LHSToPos;


// 终结符与非终结符边界，非终结符与左结合运算符边界
// 终结符对应的Int范围为[0,TNBound)
// 非终结符对应的Int范围为[TNbound,NLBound)
int TNBound,NLBound;
int maxLen,maxIdx;

// 全局数据变量

// 存放Str形式的终结符
unordered_set<string> Terminals_Str;
// 存放Str形式的非终结符
unordered_set<string> Nonterminals_Str;
// 存放Str形式的产生式
ProducerVecStr TranslationRule_Str;
// 存放Str形式的左结合运算符
unordered_set<string> Left_Str;
// 存放Str形式的左结合运算符->优先级的映射
unordered_map<string, int> Left_Precedence;
// 存储所有单个字符(Int形式)->First集合(Int形式)的映射
map<int, unordered_set<int>> First;
// 存储Str形式的开始符
string start;
// 存储Str形式的C声明部分
string Declarations;
// 存储Str形式的辅助性C语言例程
string CRoutines;

// 存放Int形式的产生式
ProducerVecInt TranslationRule_Int;