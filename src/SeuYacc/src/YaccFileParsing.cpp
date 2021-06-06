#include "pch.h"
#include "GlobalDataStructure.h"

using namespace std;

// 该函数主要用来解析yacc规约文件，以获得Token和产生式等信息
// 该函数的输入依次为yacc规约文件的文件名，%start内容，终结符(保存所有token)，产生式集合，C声明部分，辅助性C语言例程，二义文法下左结合操作符，左结合操作符-优先级映射
// 该函数的输出代表解析状态，为0则表示解析成功，为1则表示解析失败
int yaccFileParsing(const string& fileName, string& start, unordered_set<string>& Terminals, ProducerVecStr& TranslationRule, string& Declarations, string& CRoutines, unordered_set<string>& Left, unordered_map<string, int>& Left_Precedence){

    // 读取yacc规约文件
	ifstream inFile;
	inFile.open(fileName, ios::in);

    // 文件读取失败
	if (!inFile) {
		cout << "ERROR: Can't open Yacc file!" << endl;
		return 1;
	}

    // str用来临时存储读入的内容
	string str;
	getline(inFile, str);

    // 解析C声明部分
	if (str == "%{") {
		getline(inFile, str);

		while (str != "%}" && !inFile.eof()) {
            // 将结果存入Declarations中
			Declarations += str + "\n";
			getline(inFile, str);
		}
		
        // 错误：声明部分未完成文件却已结束
        if (inFile.eof()) {
			cout << "ERROR: Incomplete Declarations Part!" << endl;
			return 1;
		}
	}

    // 接下来对于token进行解析
    while (str != "%token") 
        inFile >> str;

	// 解析所有token并存入集合中
	inFile >> str;
	do {
		if (str == "%token") 
			inFile >> str;

		Terminals.insert(str);
		inFile >> str;
	} while (str != "%left" && str != "%%" && str != "%start");
    
    // 操作符优先级，0为最低优先级
	int opPrecedence = 0; 
    // 左结合操作符及其优先级声明部分
	if (str == "%left"){
        inFile >> str;
		do {
            // 读到了新的%left
			if (str == "%left"){
				inFile >> str;
				opPrecedence++;
                continue;
			}

            // 读到了注释
			if (str == "/*"){
				inFile >> str;
				while (str != "*/")
					inFile >> str;
                inFile >> str;
                continue;
			}

            // 某左结合操作符
			Left.insert(str);
			Left_Precedence.insert(make_pair(str, opPrecedence));
            inFile >> str;
		} while (str != "%start" && str != "%%");
	}

    // %start部分
	if (str == "%start"){
        inFile >> str;
		do {
            // 读到注释
			if (str == "/*"){
				inFile >> str;
				while (str != "*/")
					inFile >> str;
                inFile >> str;
                continue;
			}

			start = str;
            inFile >> str;
            break;
		} while(1);
	}

    // 若为%%则进入下一部分
    if (str == "%%")
        inFile >> str;

	// 读入产生式
	int counter = 0;
	do {
		pair<string, vector<string>> p;

		p.first = str;   // 产生式左部
		inFile >> str;   // str此时应该是":"

        // 产生式格式错误，缺少":"
		if (str != ":"){
			cout << "ERROR: Expect \":\" before "<< str << "!" << endl;
			return 1;
		}

		inFile >> str;  // 继续读取产生式右部

		do {
			if (str == "|"){
				TranslationRule.push_back(p);
				p.second.clear();
				inFile >> str;
			}

			while (str != "|" && str != ";"){
                // 将产生式右部放入一个向量
				p.second.push_back(str);
				inFile >> str;
			}
		} while (str != ";");

		TranslationRule.push_back(p);
		inFile >> str;

	} while (str != "%%");
	
    //读入函数部分
	do {
		getline(inFile, str);
		CRoutines += str + "\n";
	} while (!inFile.eof());

    // 向终结符集中添加#
	Terminals.insert("#");

    // 除去产生式右部中的单引号
	for (size_t i = 0; i < TranslationRule.size(); i++)
		for (size_t j = 0; j < TranslationRule[i].second.size(); j++) 
			if (TranslationRule[i].second[j][0] == '\'') 
				TranslationRule[i].second[j] = TranslationRule[i].second[j][1];

    // 解析成功并返回
	return 0;
}