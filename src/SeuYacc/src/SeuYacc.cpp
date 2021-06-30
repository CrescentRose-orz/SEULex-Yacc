// 此文件是SeuYacc的主体文件，包含main函数

#include "header.h"
#include <iostream>
#include "DataStructure.h"
#include "YaccFileParsing.h"
#include "Transform.h"
#include "First.h"

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

    // 添加一条起始的产生式
    setStart(start, TranslationRule_Str);

    // 获得所有Str形式的非终结符
    getNonterminals(TranslationRule_Str, Nonterminals_Str);

    // 获得所有Str形式的终结符
    getTerminals(TranslationRule_Str, Terminals_Str);


    // ----------------以上为yacc规则文件解析部分----------------
    // ----------------下面开始进行Str到Int的转换----------------

    // 构造终结符str->int的映射
    transformTerminals(Terminals_Str, StrToInt, IntToStr, TNBound);

    // 构造非终结符str->int的映射
    transformNonterminals(Nonterminals_Str, StrToInt, IntToStr, TNBound, NLBound);

    // 构造左结合运算符str->int的映射
    transformLeft(Left_Str, StrToInt, IntToStr, NLBound);

    // 根据非终结符及终结符转换结果
    // 构造int形式表示产生式集合
    // 并构造int形式左部->positions(vector<int>，下标集合)的映射
    transformTranslationRule(TranslationRule_Str, TranslationRule_Int, StrToInt, IntToStr);

    // ----------------以上为Str到Int的转换部分----------------
    // ----------------下面开始进行求取First部分----------------

    getFirst();
    cout << IntToStr[-1] <<endl;

    for(int i = 0; i < NLBound; i++){
        cout << IntToStr[i] << "的First为：";
        for(auto temp : First[i])
            cout << IntToStr[temp] << ' ';
        cout << endl;
    }

    fclose(stdout);
}