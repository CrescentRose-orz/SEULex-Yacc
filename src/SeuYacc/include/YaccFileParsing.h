#include "header.h"
#include "DataStructure.h"

using namespace std;

// 该函数主要用来解析yacc规约文件，以获得Token和产生式等信息
// 该函数的输入依次为yacc规约文件的文件名，%start内容，终结符(保存所有token)，产生式集合，C声明部分，辅助性C语言例程，二义文法下左结合操作符，左结合操作符-优先级映射
// 该函数的输出代表解析状态，为0则表示解析成功，为1则表示解析失败
int yaccFileParsing(const string& fileName, string& start, unordered_set<string>& Terminals_Str, ProducerVecStr& TranslationRule_Str, string& Declarations, string& CRoutines, unordered_set<string>& Left_Str, unordered_map<string, int>& Left_Precedence);

// 该函数为了防止开始产生式有多个分支，故增加一条初始产生式
// 该函数的输入依次为产生式符号str，产生式集合
void setStart(string& str, ProducerVecStr& TranslationRule_Str);

// 该函数用于获得所有的非终结符，即产生式集合的所有左部
// 该函数的输入为产生式集合，使用引用传递来传递输出值Nonterminals_Str
void getNonterminals(ProducerVecStr& TranslationRule_Str, unordered_set<string>& Nonterminals_Str);

// 该函数用于获得所有的终结符(除了已解析的token)
// 即遍历所有产生式的右部，出现单引号括起来的就认为是非终结符
// 该函数的输入为产生式集合，使用引用传递来传递输出值Terminals_Str
void getTerminals(ProducerVecStr& TranslationRule_Str, unordered_set<string>& Terminals_Str);