#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include<bits/stdc++.h>
using namespace std;

extern std::map<string,string> SvalType;
extern std::unordered_map<int,string>IvalType;
extern bool hasUnion;
extern string unionCode;


// 为后续方便使用相关数据结构，此文件定义了使用到的数据结构

// 初始产生式的序列（每一项中包含产生式的左部和产生式的右部）
typedef std::vector<std::pair<std::string, std::vector<std::string>>> ProducerVecStr;
typedef std::vector<std::pair<int, std::vector<int>>> ProducerVecInt;

// 产生式以及·的位置，定义为一个项目，vector的第一个元素为产生式左边符号
typedef std::pair<std::vector<int>, int> Item;

// LR(1)/LALR项目
typedef std::pair<Item, int> LRitem;
typedef std::unordered_set<LRitem> DFAState;
typedef std::vector<DFAState> DFA;
typedef std::set<Item> StateCore;
typedef std::map<std::pair<int, int>, int> DFATransitionTable;


extern std::vector<int> producerBaseIdx;

// 存放所有string与int相互映射关系
extern unordered_map<string, int> StrToInt;
extern unordered_map<int, string> IntToStr;

// 存放Int形式产生式->产生式在数据结构中的存放序号的映射关系
//unordered_map<vector<int>, int> ProToPos;

// Int形式产生式左部->Int形式产生式右部的映射关系
// 存的是translationRule—_Int的下标。
extern unordered_map<int, vector<int>> LHSToPos;

// 终结符与非终结符边界，非终结符与左结合运算符边界
// 终结符对应的Int范围为[0,TNBound)
// 非终结符对应的Int范围为[TNbound,NLBound)
extern int TNBound,NLBound;
extern int maxLen,maxIdx;

// 定义空串，可以根据不同的yacc.y进行调整
const std::string NullString = "epsilon";

// 全局数据变量

// 存放Str形式的终结符
extern unordered_set<string> Terminals_Str;
// 存放Str形式的非终结符
extern unordered_set<string> Nonterminals_Str;
// 存放Str形式的产生式
extern ProducerVecStr TranslationRule_Str;
// 存放Str形式的左结合运算符
extern unordered_set<string> Left_Str;
// 存放Str形式的左结合运算符->优先级的映射
extern unordered_map<string, int> Left_Precedence;
// 存储所有单个字符(Int形式)->First集合(Int形式)的映射
extern map<int, unordered_set<int>> First;
// 存储Str形式的开始符
extern string start;
// 存储Str形式的C声明部分
extern string Declarations;
// 存储Str形式的辅助性C语言例程
extern string CRoutines;

extern vector<string> reduceActionCode; 
//初始产生式编号Int->string的语义动作

// 存放Int形式的产生式
extern ProducerVecInt TranslationRule_Int;

extern string YYHEADER;
extern string YY_REDUCER_BODY2;
extern string YY_REDUCER_BODY1;
extern string YY_PARSER_FUNCTION;
extern string C_SOURCE_CODE;
namespace std {

	template<>
	struct hash<LRitem> {
		inline size_t operator()(const LRitem& s) const {
			return  hash<int>()(s.second);
		}
	};
}

#endif