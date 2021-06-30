#include "header.h"
#include "DataStructure.h"

using namespace std;

// 存储所有单个字符的first的映射


// 终结符与非终结符边界
// 终结符对应的Int范围为[0,TNBound)
// 非终结符对应的Int范围为[TNbound,NLBound)
//extern int TNBound, NLBound;

// 使用Int来表示的产生式集合
//extern ProducerVecInt TranslationRule_Int;

// Int形式产生式左部->Int形式产生式右部的映射关系
//extern unordered_map<int, vector<int>> LHSToPos;

// 该函数对单个字符求其first，是一个递归算法
// 该函数的输入依次为所求字符，返回结果集，以及一个维护的已求过first的非终结符集合
void getFirst(const int& symbol, unordered_set<int>& resultSet, unordered_set<int>& processedSymbol) {

	// 该符号之前已求过first
	if (First.find(symbol) != First.end()) {
		resultSet.insert(First[symbol].cbegin(), First[symbol].cend());
		return;
	}

	// 如果是-1返回空串
	if (symbol == -1) {
		resultSet.insert(-1);
		return;
	}

	// 该符号为终结符
	if (symbol < TNBound) {
		resultSet.insert(symbol);
		return;
	}
	
	// 该符号为非终结符
	else {
		// 将该非终结符加入已处理集合中
		processedSymbol.insert(symbol);

		// 寻找该非终结符所有产生式的存储位置
		vector<int> Position = LHSToPos[symbol];
		// 遍历所有相关产生式
		for(int i = 0; i < Position.size(); i++){
			int pos = Position[i];
			// 取出产生式
			auto producer = TranslationRule_Int[pos];

			// 产生式形式为symbol->eclipse
			if (producer.second.size() == 1 && producer.second[0] == -1){
				// 返回空串
				resultSet.insert(-1);
				continue;
			}

			// 否则遍历产生式右部
			else{
				int j = 0;
				unordered_set<int> temp_set;
				while(j < producer.second.size()){
					temp_set.clear();
					// 若非终结符已求过first则退出循环以防止左递归的出现
					if(processedSymbol.find(producer.second[j]) != processedSymbol.end())
						break;

					// 对于未求过first的符号求取其first
					getFirst(producer.second[j], temp_set, processedSymbol);
					
					// 若求得的first中无空串，则first求取完成
					if (temp_set.find(-1) == temp_set.end()) {
						resultSet.insert(temp_set.cbegin(), temp_set.cend());
						break;
					}

					// 最后一项仍存在空串则该非终结符的first集中加入空串
					if (j == producer.second.size() - 1) {
						resultSet.insert(-1);
						break;
					}
					
					// 否则不是最后一项，移除空串
					temp_set.erase(-1);
					// 将该次迭代结果加入结果集
					resultSet.insert(temp_set.cbegin(), temp_set.cend());
					j++;
				}
			}
		}
	}
}


// 该函数求取所有符号的first并存入数据结构First中供后续调用
void getFirst() {
	unordered_set<int> firstSet, stackSet;

	// 求取所有符号的first
	for (int i = 0; i < NLBound; i++) {
		firstSet.clear();
		stackSet.clear();
		getFirst(i, firstSet, stackSet);
		First.emplace(i, firstSet);
	}
}