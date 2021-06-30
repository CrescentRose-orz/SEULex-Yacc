#include "header.h"
#include "DataStructure.h"

using namespace std;

// 该函数将终结符初始数据结构中的字符串映射为一个整型数
void transformTerminals(unordered_set<string>& Terminals_Str, unordered_map<string, int>& StrToInt, unordered_map<int, string>& IntToStr, int& TNBound);

// 该函数将非终结符初始数据结构中的字符串映射为一个整型数
// 该函数的输入依次为Str形式的非终结符，所有Str->Int的映射，所有Int->Str的映射，终结符和非终结符的边界，非终结符和左结合操作符的边界
void transformNonterminals(unordered_set<string>& Nonterminals_Str, unordered_map<string, int>& StrToInt, unordered_map<int, string>& IntToStr, int& TNBound, int& NLBound);

// 该函数将左结合操作符初始数据结构中的字符串映射为一个整型数
// 该函数的输入依次为Str形式的左结合操作符，所有Str->Int的映射，所有Int->Str的映射，非终结符和左结合操作符的边界
void transformLeft(unordered_set<string>& Left_Str, unordered_map<string, int>& StrToInt, unordered_map<int, string>& IntToStr, int& NLBound);

// 该函数将产生式集合初始数据结构中的字符串映射为一个整型数
// 由于产生式集合使用vector存储，还需建立一个产生式
// 该函数的输入依次为Str形式的产生式集合，Int形式的产生式集合，所有Str->Int的映射，所有Int->Str的映射
void transformTranslationRule(ProducerVecStr& TranslationRule_Str, ProducerVecInt& TranslationRule_Int, unordered_map<string, int>& StrToInt, unordered_map<int, string>& IntToStr);