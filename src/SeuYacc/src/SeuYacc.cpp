// 此文件是SeuYacc的主体文件，包含main函数

#include "header.h"
#include "DataStructure.h"
#include "YaccFileParsing.h"

using namespace std;



int main(int argc, char const* argv[]){
    freopen("out.txt","w",stdout);
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
        cout << "\n File parsing COMPLETED." << endl;
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

    // 添加一条起始的产生式
    setStart(start, TranslationRule_Str);

    // 获得所有Str形式的非终结符
    getNonterminals(TranslationRule_Str, Nonterminals_Str);

    // 获得所有Str形式的终结符
    getTerminals(TranslationRule_Str, Terminals_Str);

    // 输出各个数据结构，检查其正确性
    cout << "\nterminals: " << endl;
    for (auto str : Terminals_Str){
        cout << str << " ";
    }

    cout << "\nNonterminals: " << endl;
    for (auto str : NonTerminals_Str){
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

    // ----------------以上为yacc规则文件解析部分----------------
    // ----------------下面开始进行Str到Int的转换----------------


    fclose(stdout);
}