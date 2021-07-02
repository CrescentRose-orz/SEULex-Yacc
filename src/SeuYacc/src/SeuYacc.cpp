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
    
    FILE *fp = freopen("out.txt","w",stdout);
    // yacc规约文件的文件名
    string file_name;
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
    // for (auto pp:TranslationRule_Int){
    //     cout<<I2S(pp.first)<<" -> ";
    //     for (auto pro:pp.second){
    //         cout<<I2S(pro)<<" ";
    //     }
    //     cout<<endl;
    // }
    // cout<<NLBound<<" "<<S2I("declaration_list")<<endl;
    // for (auto pro:LHSToPos[S2I("declaration_list")]){
    //     cout<<"declaration_list ->";
    //     for (auto id:getRight(pro)){
    //         cout<<I2S(id)<<" ";
    //     }
    //     cout<<endl;
    // }
    // for (auto pro:LHSToPos[S2I("declaration")]){
    //     cout<<"declaration ->";
    //     for (auto id:getRight(pro)){
    //         cout<<I2S(id)<<" ";
    //     }
    //     cout<<endl;
    // }
//    solveFirst();
    getFirst();
    // cout<<"first:"<<endl;
    // for (int i = 0; i < NLBound;++i){
    //     if (i<128){
    //         cout<<"'"<<(char)i<<"':"<<endl;
    //     } else {
    //         cout<<I2S(i)<<":"<<endl;            
    //     }
    //     for (auto &id:First[i]){
    //         cout<<"'"<<I2S(id)<<"' ";
    //     }
    //     cout<<endl;
    // }

    //
    // ----------------以上为求取所有单字符First部分----------------
    // ----------------下面开始进行LR1项目集簇的构造----------------
    // cout<<"test begin"<<endl;
    // LR_Node test;
    // test.addProducer(LR_Producer(LHSToPos[S2I("pointer")][0],1,100));
    // test.addProducer(LR_Producer(LHSToPos[S2I("pointer")][1],1,100));
    // test.addProducer(LR_Producer(LHSToPos[S2I("pointer")][2],1,100));
    // test.solveEclosure();
    // test.print();
    // cout<<"test ended!"<<endl<<endl;
    // return 0;
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
        for (auto &t:myLR[0].allNexts){
            cout<<" "<<t<<"("<<I2S(t)<<") ";
        }
        cout<<endl;
        cout<<"get node: "<< myLR.consturctLR()<<endl;
        myLR.logger.end("construct lr1");
        myLR.logger.save();
        //myLR.printVisualLR(vLROut);
        myLR.logger.end("yacc");
        myLR.logger.save();
        LR &&myLALR = myLR.consturctLALR();
        myLALR.constructParsingTable();
        cout<<myLR.allKeys.size()<<endl;
        cout<<"total node:"<<myLALR.pool.size()<<endl;
        //myLR.constructParsingTable();
        fflush(fp);
        fclose(fp);
        freopen("CON","w",stdout);
        //fclose(stdout);
        while (1){
            int id;
            cin>>id;
            if (id < 0){
                break;
            }
            myLALR.printNode(id);
        }
        
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