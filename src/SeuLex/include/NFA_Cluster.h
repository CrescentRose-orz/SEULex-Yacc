#ifndef NFA_CLUSTER
#define NFA_CLUSTER
#include<bits/stdc++.h>
#include"NFA.h"
#include"action.hpp"
#include"Logger.h"
#include"CONSTANT.h"




class NFA;
class NFA_Node;
class RE_operator;
class NFA_Cluster{
public:    
    int head;
    int tail;
    static bool checkBracket(string &bracket,int l,int r);
    //NFA_Cluster copy(NFA &buff,NFA_Cluster &origin);
    NFA_Cluster(int head);
    NFA_Cluster(int head,int tail);
    //single char,.
    NFA_Cluster(NFA &buff,char c);
    // '|' '^'
    NFA_Cluster(NFA &buff,char op ,NFA_Cluster a ,NFA_Cluster b );
    // multi {}
    NFA_Cluster(NFA &buff,int l,int r,NFA_Cluster a );
    //single op : * ? +
    NFA_Cluster(NFA &buff,char op ,NFA_Cluster a );
   //quotation 
    NFA_Cluster(NFA &buff ,string &quotation , int l ,int r );
    //bracket todo 【】
    static NFA_Cluster getBracket(NFA &buff,string &bracket,int l,int r);
    //todo{}
    static NFA_Cluster getBrace(NFA &buff,NFA_Cluster a,int l ,int r);
    static NFA_Cluster createEmpty(NFA &buff);
    static NFA_Cluster createSingle(NFA &buff,NFA_Node node);
    static NFA_Cluster RE2NFA_Cluster(string RE,NFA &buff);
    static NFA_Cluster RE2NFA(string RE,NFA &buff,action _action);
    static void cal(NFA &buff,stack<NFA_Cluster> &operandStack,RE_operator op);
    static NFA_Cluster getTrans(NFA&buff,char c);
    static NFA_Cluster copy(NFA &buff,NFA_Cluster &origin);
};
/*
. 匹配任何单个字符，除\n.                       

- 表示匹配范围，如：a-z，表示匹配a-z之间的任何字符

* 匹配前面表达式的零个或多个拷贝。

[] 匹配括号内的任意字符的字符类，第一个符号是"^"，表示匹配除括号中的字符以外的任意字符。

^ 作为正则表达式的第一个字符，匹配行的开头。   例：^ab.,表示以ab开头的字符串.

$ 作为正则表达式的最后一字符，匹配行的结尾。


\ 用于转义字符                                                                      OK

+ 匹配前面表达式一次或多次出现。                                                      OK

? 匹配前面表达式零次或1次出现。                                                       OK

| 匹配前面表达式或随后表达式                                                          OK

"" 引号中的每个字符解释为字面意思


{} 指示一个模式可能出现的次数


*/

#endif