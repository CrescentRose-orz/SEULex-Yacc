// 此文件是SeuYacc的主体文件，包含main函数

#include "header.h"
#include "DataStructure.h"
#include "YaccFileParsing.h"

using namespace std;

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

int main(int argc, char const* argv[]){
    
    // yacc规约文件的文件名
    string file_name;

    // 参数个数不正确
	if (argc != 2) {
		cout << "ERROR: A parameter is missing!\n";
		return -1;
	}

	else {
		file_name = string(argv[1]);
	}

    // 输出正在解析文件
    cout << "Reading yacc file...";
    int flag = yaccFileParsing(file_name, start, Terminals_Str, TranslationRule_Str, Declarations, CRoutines, Left_Str, Left_Precedence);
    
    // 正确解析文件
    if(flag == 0)
        cout << "\t File parsing COMPLETED." << endl;
    // 解析文件错误，退出文件
    else
        exit(0);
    
    // 输出各个数据结构，检查其正确性
    cout << "start: " << start << endl;
    cout << "\nterminals: " << endl;
    for (auto str : Terminals_Str){
        cout << str << " ";
    }

    cout << "\ntranslationRule: " << endl;
    for (int i = 0; i < TranslationRule_Str.size(); i++){
        cout << TranslationRule_Str[i].first << "->";
        for (int j = 0; j < TranslationRule_Str[i].second.size(); j++){
            cout << TranslationRule_Str[i].second[j] << " ";
        }
        cout << endl;
    }

    cout << "\ndeclarations: " << endl;
    cout << Declarations << " ";

    cout << "\ncroutines: " << endl;
    cout << CRoutines << " ";

    cout << "\nleft: " << endl;
    for (auto str : Left_Str){
        cout << str << " ";
        cout << Left_Precedence[str] << " ";
    }
}