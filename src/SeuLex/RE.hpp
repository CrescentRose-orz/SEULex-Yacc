#include<bits/stdc++.h>

using namespace std;


class RE{
public:
    static string repeat(string re,int x,int y){
    
    }
    static string getAll(){
        string tmp;
        char c = 0;
        tmp.resize(256);
        tmp += c;
        for (c = 1; c <256; ++c){
            if (c == '\n'){
                continue;
            }
            tmp += c;
        }
        return tmp;
    }


};