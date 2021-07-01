// 此文件是SeuYacc的主体文件，包含main函数

#include "header.h"
#include <iostream>
#include "DataStructure.h"
#include "YaccFileParsing.h"
#include "Transform.h"
#include "First.h"
#include "LR.h"
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

    // ----------------以上为求取所有单字符First部分----------------
    // ----------------下面开始进行LR1项目集簇的构造----------------
    cout<<S2I("start")<<endl;
    LR myLR("yacc.log");
    LR_Node startNode;
    fstream vLROut;
    try{
    for (auto &id:LHSToPos[S2I("start")]){
        startNode.addProducer(LR_Producer(id,0,S2I("#")));
    }
    myLR.add(startNode);
    cout<<"begin startPoint"<<endl;
    myLR[0].solveEclosure();
    cout<<"adding startPoint done!"<<endl;
        myLR.logger.start("yacc");
        vLROut.open("LR1.dot",ios::out);
        myLR.logger.start("construct lr1");
        cout<<"begin construct lr"<<endl;
        cout<<myLR[0].nextPros.count(S2I("declaration_specifiers"))<<" counts of next"<<endl;
        cout<<"All next:"<<endl;
        for (auto &t:myLR[0].allNexts){
            cout<<" "<<t<<"("<<I2S(t)<<") ";
        }
        cout<<endl;
        cout<<"get node: "<< myLR.consturctLR()<<endl;
        myLR.logger.end("construct lr1");
        myLR.printVisualLR(vLROut);
        myLR.logger.end("yacc");
    fclose(stdout);
    } catch(exception e){
        cout<<"error occurs"<<endl;
        cout<<e.what()<<endl;
        myLR.logger.error("yacc","error occurs \n",0);
        myLR.logger.close();
        myLR.printVisualLR(vLROut);
        system("pause");
    }
    cout<<"main process ended!"<<endl;
    system("pause");
}