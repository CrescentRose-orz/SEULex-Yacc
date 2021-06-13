#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

// 为后续方便使用相关数据结构，此文件定义了使用到的数据结构
#include "header.h"

// 初始产生式的序列（每一项中包含产生式的左部和产生式的右部）
typedef std::vector<std::pair<std::string, std::vector<std::string>>> ProducerVecStr;
typedef std::vector<std::pair<int, std::vector<int>>> ProducerVec;

// 产生式以及·的位置，定义为一个项目，vector的第一个元素为产生式左边符号
typedef std::pair<std::vector<int>, int> Item;

// LR(1)/LALR项目
typedef std::pair<Item, int> LRitem;
typedef std::unordered_set<LRitem> DFAState;
typedef std::vector<DFAState> DFA;
typedef std::set<Item> StateCore;
typedef std::map<std::pair<int, int>, int> DFATransitionTable;

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