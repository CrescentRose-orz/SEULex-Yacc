#include "First.h"

using namespace std;

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