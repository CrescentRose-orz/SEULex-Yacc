#include "Transform.h"

using namespace std;

void transformTerminals(unordered_set<string>& Terminals_Str, unordered_map<string, int>& StrToInt, unordered_map<int, string>& IntToStr, int& TNBound){
    
    int counter = 0;

	for (auto str : Terminals_Str){
		StrToInt.insert(make_pair(str, counter));
		IntToStr.insert(make_pair(counter, str));
		counter++;
	}

	TNBound = counter;
}

void transformNonterminals(unordered_set<string>& Nonterminals_Str, unordered_map<string, int>& StrToInt, unordered_map<int, string>& IntToStr, int& TNBound, int& NLBound){
    
    int counter = TNBound;
    producerBaseIdx.resize(TNBound);
    maxLen = 1;
	for (auto str : Nonterminals_Str){
		StrToInt.insert(make_pair(str, counter));
		IntToStr.insert(make_pair(counter, str));
        for (;(1<<maxLen) <= str.length(); ++maxIdx);
		counter++;
	}

	NLBound = counter;
    for (maxIdx = 1;(1<<maxIdx) <= NLBound; ++maxIdx);
}

void transformLeft(unordered_set<string>& Left_Str, unordered_map<string, int>& StrToInt, unordered_map<int, string>& IntToStr, int& NLBound){
    
    int counter = NLBound;

	for (auto str : Left_Str){
		StrToInt.insert(make_pair(str, counter));
		IntToStr.insert(make_pair(counter, str));
		counter++;
	}
}

void transformTranslationRule(ProducerVecStr& TranslationRule_Str, ProducerVecInt& TranslationRule_Int, unordered_map<string, int>& StrToInt, unordered_map<int, string>& IntToStr){
	
    // 遍历每一个产生式项
    for (auto temp_producer : TranslationRule_Str){
		
        vector<int> temp_vector;
		temp_vector.clear();
		// 遍历每一个产生式右部项
        for (auto temp_RHS : temp_producer.second)
			temp_vector.push_back(StrToInt[temp_RHS]);

		TranslationRule_Int.push_back(make_pair(StrToInt[temp_producer.first], temp_vector));
	}

    // 产生Producer->Position的映射
	for (int i = 0; i < TranslationRule_Int.size(); i++){
        vector<int> temp = TranslationRule_Int[i].second;
		temp.insert(temp.begin(), TranslationRule_Int[i].first);
		//ProToPos.insert(make_pair(temp, i));
	}

    // 产生LHS->Position的映射
    // 利用相同产生式左部的产生式连续存放的规律，只需遍历一遍产生式集合即可
    vector<int> temp_vector;
	temp_vector.clear();
    temp_vector.push_back(0);
    int temp = TranslationRule_Int[0].first;

    // 遍历所有产生式
	for (int i = 1; i < TranslationRule_Int.size(); i++){
        int temp2 = TranslationRule_Int[i].first;
        if(temp2 == temp){
            temp_vector.push_back(i);
            continue;
        }
        else{
            LHSToPos.insert(make_pair(temp, temp_vector));
            temp = temp2;
            temp_vector.clear();
            temp_vector.push_back(i);
            continue;
        }
    }
}