#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
#include"header.h"
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


// 存放所有string与int相互映射关系
unordered_map<string, int> StrToInt;
unordered_map<int, string> IntToStr;

// 存放Int形式产生式->产生式在数据结构中的存放序号的映射关系
unordered_map<vector<int>, int> ProToPos;

// Int形式产生式左部->Int形式产生式右部的映射关系
unordered_map<int, vector<int>> LHSToPos;

// 终结符与非终结符边界，非终结符与左结合运算符边界
// 终结符对应的Int范围为[0,TNBound)
// 非终结符对应的Int范围为[TNbound,NLBound)
int TNBound,NLBound;

// 使用Int来表示的产生式集合
ProducerVecInt TranslationRule_Int;




// 定义空串，可以根据不同的yacc.y进行调整
const std::string NullString = "epsilon";

namespace std {

	template<>
	struct hash<LRitem> {
		inline size_t operator()(const LRitem& s) const {
			return  hash<int>()(s.second);
		}
	};
}

#endif