#ifndef RE_H
#define RE_H

#include<bits/stdc++.h>

using namespace std;

/*
。 * 【】 ^ $ {} \ + ? | "" / {}
.[] {name} \ "" 
* $ ^ {}
*/




class RE_operator{
public:
    char op;
    int l,r;
    RE_operator(char c);
    RE_operator(char c,int l,int r);
    // operator int(){
    //     return (int)op;
    // }
    operator char();
};



class RE{
public:
    static char transChar[256];   
    static int newPri[256];
    static int oldPri[256];
    //static void buildGraph(vector<string> &,vector<string,string> &);
    //static void checkCircle();
    static char trans(int &idx,string s);
    static char trans(char c);
    static int skipFirst(string & newRE,const string &raw,int &idx,map<string,string> &preDefine);
    static string unfoldRE(const string &raw,map<string,string> &preDefine);
    static void init();
};


#endif