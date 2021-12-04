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
					if (symbol == producer.second[j]){
						break;
					}
					// 若非终结符已求过first则退出循环以防止左递归的出现
					if(processedSymbol.find(producer.second[j]) != processedSymbol.end()){
						unordered_set<int> tmp2;
						getFirst(producer.second[j], tmp2, processedSymbol);
						temp_set.insert(tmp2.cbegin(),tmp2.cend());
						if (!tmp2.count(-1)){
							resultSet.insert(temp_set.cbegin(), temp_set.cend());
							break;
						}
					//	break;
					}

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
		First.emplace(symbol,resultSet);
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

void getFirst(const vector<int>& symbols, unordered_set<int>& resultSet){
	
	// 输入为空串
	if (symbols.size() == 0) { 
		// 返回结果集为空串
		resultSet.insert(-1);
		return;
	}

	// 输入不为空串，则依次考察首字符的First
	int i = 0;

	// 不是最后一项，且其求得字符的First集存在空串时继续循环
	while (i < symbols.size()) {
		// 获取当前循环下的字符First集合
		auto & tempFirst = First[symbols[i]];

		// 当前字符First集合中不存在空串，将该First集结果加入所求First集中，退出循环
		if (tempFirst.find(-1) == tempFirst.end()) {
			resultSet.insert(tempFirst.cbegin(), tempFirst.cend());
			break;
		}

		// 假设存在空串且遍历到了字符串的最后一个字符
		if (i == symbols.size()) {
			// 将空串加入所求First集中，退出循环
			resultSet.insert(-1); 
			break;
		}

		// 假设存在空串但不是字符串的最后一个字符
		// 消除空串，将该First集结果加入所求First集中，继续循环
		tempFirst.erase(-1);
		resultSet.insert(tempFirst.cbegin(), tempFirst.cend());
		
		++i;
	}
}

void solveFirst(){
unordered_set<int> vis;
queue<int> changes;
bool flag = true;
	#ifdef DEBUG_FIRST
	cout<<"solving first, NLBound = "<<NLBound<<endl;
	#endif
	for (int i = 0 ; i < NLBound; ++i){
		solveSingleFirst(i,flag);
		//changes.push(i);
		// if (!vis.count(i)){
		// 	solveSingle(i,vis);
		// }
	}
	flag = true;
	while(flag){
		flag = false;
		for (int i = TNBound; i < NLBound; ++i){
			solveSingleFirst(i,flag);
		}
	}
	// while(!changes.empty()){
	// 	solveSingleFirst(changes.front(),changes);
	// 	changes.pop();
	// }


}

void solveSingleFirst(int idx,bool &changesFlag){
	#ifdef DEBUG_FIRST
	if (idx<128)
		cout<<"solving "<<idx<<" : "<<(char)idx<<endl;
	else
		cout<<"solving "<<idx<<" : "<<I2S(idx)<<endl;
	#endif
	if (isTerminal(idx)){
		First[idx].insert(idx);
		return;
	}
	for (auto pros:LHSToPos[idx]){
		#ifdef DEBUG_FIRST
		cout<<"get new producer for "<<idx<<endl;
		cout<<I2S(idx)<<"->";
		for (auto r:getRight(pros)){
			cout<<I2S(r)<<" ";
		}
		cout<<endl;
		#endif
		for (auto now:getRight(pros)){

			#ifdef DEBUG_FIRST
			cout<<"get "<<I2S(now)<<endl;
			#endif
			// if (now == idx){
			// 	if ()
			// 	break;
			// }
			//solveSingle(now,vis);
			bool flag = false;
			for (auto _first:First[now]){
				if (_first != -1 ){
					if ( First[idx].count(_first) == 0){
						First[idx].insert(_first);
						changesFlag = true;
					}
				} else {
					flag = true;
				}
			}
			if (!flag){
				break;
			}
		}
	}
}


// old version
void solveSingle(int idx,unordered_set<int> &vis){
	if (vis.count(idx)){
		return;
	}
	if (isTerminal(idx)){
		First[idx].insert(idx);
		vis.insert(idx);
		return;
	}
	vis.insert(idx);
	for (auto pros:LHSToPos[idx]){
		for (auto now:getRight(pros)){
			if (now == idx){
				break;
			}
			solveSingle(now,vis);
			bool flag = false;
			for (auto _first:First[now]){
				if (_first != -1){
					First[idx].insert(_first);
				} else {
					flag = true;
				}
				if (!flag){
					break;
				}
			}
		}
	}
	
}