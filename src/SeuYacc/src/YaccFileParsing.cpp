#include "header.h"
#include "DataStructure.h"

using namespace std;

// 该函数主要用来解析yacc规约文件，以获得Token和产生式等信息
// 该函数的输入依次为yacc规约文件的文件名，%start内容，终结符(保存所有token)，产生式集合，C声明部分，辅助性C语言例程，二义文法下左结合操作符，左结合操作符-优先级映射
// 该函数的输出代表解析状态，为0则表示解析成功，为1则表示解析失败
int yaccFileParsing(const string& fileName, string& start, unordered_set<string>& Terminals_Str, ProducerVecStr& TranslationRule_Str, string& Declarations, string& CRoutines, unordered_set<string>& Left_Str, unordered_map<string, int>& Left_Precedence){

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

		Terminals_Str.insert(str);
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
			Left_Str.insert(str);
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
				TranslationRule_Str.push_back(p);
				p.second.clear();
				inFile >> str;
			}

			while (str != "|" && str != ";"){
                // 将产生式右部放入一个向量
				p.second.push_back(str);
				inFile >> str;
			}
		} while (str != ";");

		TranslationRule_Str.push_back(p);
		inFile >> str;

	} while (str != "%%");
	
    //读入函数部分
	do {
		getline(inFile, str);
		CRoutines += str + "\n";
	} while (!inFile.eof());

    // 向终结符集中添加#
	Terminals_Str.insert("#");

    // 解析成功并返回
	return 0;
}

// 该函数为了防止开始产生式有多个分支，故增加一条初始产生式
// 该函数的输入依次为产生式符号str，产生式集合
void setStart(string& str, ProducerVecStr& TranslationRule_Str){
	
	pair<string, vector<string>> start;
	// 产生初始产生式
	start.first = "start";
	start.second.push_back(str);
	// 将初始产生式加入产生式集合
	TranslationRule_Str.insert(TranslationRule_Str.begin(), start);
}

// 该函数用于获得所有的非终结符，即产生式集合的所有左部
// 该函数的输入为产生式集合，使用引用传递来传递输出值Nonterminals_Str
void getNonterminals(ProducerVecStr& TranslationRule_Str, unordered_set<string>& Nonterminals_Str){
	
	string str;
	// 将所有产生式的左部加入非终结符集Nonterminals_Str
	for (int i = 0; i < TranslationRule_Str.size(); i++){
		Nonterminals_Str.insert(TranslationRule_Str[i].first);
	}
}

// 该函数用于获得所有的终结符(除了已解析的token)
// 即遍历所有产生式的右部，出现单引号括起来的就认为是非终结符
// 该函数的输入为产生式集合，使用引用传递来传递输出值Terminals_Str
void getTerminals(ProducerVecStr& TranslationRule_Str, unordered_set<string>& Terminals_Str){

	// 寻找非token的终结符，并除去产生式右部中的单引号
	for (int i = 0; i < TranslationRule_Str.size(); i++)
		for (int j = 0; j < TranslationRule_Str[i].second.size(); j++) 
			if (TranslationRule_Str[i].second[j][0] == '\''){
				TranslationRule_Str[i].second[j] = TranslationRule_Str[i].second[j][1];
				Terminals_Str.insert(TranslationRule_Str[i].second[j]);
			}
}