#include<bits/stdc++.h>

using namespace std;


class RE{
public:
    static char transChar[256];   
    static int Newpri[256];
    static int Oldpri[256];
    //static void buildGraph(vector<string> &,vector<string,string> &);
    //static void checkCircle();
    static char trans(char a){
        return transChar[a];
    }

    // static string prepareRE(string RE){
    //     string rt;
    //     int j = 0,i = 0;
    //      rt.resize(RE.length()<<1);
    //     if (RE[i] == '^'){
    //         rt += RE[i++];
    //     }
    //     // if (RE[i] == '\\'){
    //     //     rt+=RE[i++];
    //     // }
    //     // rt+=RE[i++];
    //     while(i < RE.length()){
    //         switch (RE[i]){
    //             case '\\':
    //                 rt += '^';
    //                 rt += RE[i++];
    //                 rt += RE[i++];        // todo: exception 
    //                 break;
    //             case '{':

    //                 break;

    //             case '[':
    //                 rt += '^';
    //                 rt += RE[i];
    //                 while (RE[i]!=']'){
    //                     rt += RE[++i];
    //                 }
    //                 break;
    //             case '?':
    //                 ++i;
    //                 break;
    //             case '+':
    //                 ++i;
    //                 break;
    //             case '*':
    //                 ++i;
    //                 break;
    //             case '|':
    //                 ++i;
    //                 break;
    //             default:
    //                 rt += '^';
    //                 rt += RE[i++];
    //                 break;
    //         }
    //     }
    //     return rt;
    // }
    static void init(){
        for (int i = 0; i < 256; ++i){
            transChar[i] = i;
        }
        transChar['n'] = '\n';
        transChar['r'] = '\r';
      //  transChar['\\'] = '\\';
      //  memset(Newpri,-1,sizeof(Newpri));
      //  memset(Oldpri,-1,sizeof(Oldpri));
        
    }

    // static string repeat(string re,int x,int y){
    //     string tmp;
    //     --x;
    //     while(x--){
    //         tmp.append(re);
    //     }
    //     if (x!=-1){

    //     }
    // }

    // static string getAll(){
    //     string tmp;
    //     char c = 0;
    //     tmp.resize(256);
    //     tmp += c;
    //     for (c = 1; c < 255; ++c){
    //         if (c == '\n'){
    //             continue;
    //         }
    //         tmp += '|';
    //         tmp += c;
    //     }
    //     tmp += '|';
    //     tmp += c;
    //     return tmp;
    // }



};


char RE::transChar[256];   
int RE::Newpri[256];
int RE::Oldpri[256];

