#pragma once
#include<bits/stdc++.h>

using namespace std;
const int HASH_CNT = 4;
const int MAX_HASH_CNT = 4;
const long long HASH_KEY[MAX_HASH_CNT] = {19260817,2281701377,5039,10000007};
inline long long qPow(long long x,long long p){
long long ans = 1;
    while(p){
        if (p&1){
            ans*=x;
        }
        p>>=1;
        x*=x;
    }
    return ans;
}

class basicHash{
protected:
    long long hash[HASH_CNT];
    bool valid = 0;
public:
    bool isValid(){
        return valid;
    }
    basicHash(){memset(hash,0,sizeof(hash));valid = 0;}
    basicHash(int idx){
        for (int i = 0; i < HASH_CNT;++i){
            hash[i] = qPow(HASH_KEY[i],idx);
        }
        valid = 1;
    }
    basicHash(vector<long long> lastVal,int diff){
        if (diff < 0){
            throw invalid_argument("diff shoulb be larger than 0");
        }
        if (lastVal.size() != HASH_CNT){
            throw invalid_argument("argument cnt unexpected ");
        }
        for (int i = 0; i < HASH_CNT;++i){
            hash[i] = lastVal[i] * qPow(HASH_KEY[i],diff);
        }
        valid = 1;
    }
    vector<long long> getVal(){return vector<long long>(hash,hash+HASH_CNT-1);}
    long long operator[](const int &i) const {
        if (i<0 || i >= HASH_CNT){
            throw invalid_argument("index out of bound in basicHash[]");
        }
        return hash[i];
    }
    bool operator == (const basicHash &other)const{
        if (!valid){
            throw invalid_argument("this hashKey has not been initialized!");
        }
        bool flag = 1;
        int cnt = 0;
        for (int i = 0 ; i <HASH_CNT; ++i){
            if (hash[i] != other.hash[i]){
                flag = 0;
            } else {
                ++cnt;
            }
        }
        if (!flag&&cnt){
            cout<<"WARNING HASH INFLECT DETECTED! CNT:"<<cnt<<endl;
        }
        return flag;
        return 1;
    }
    
    void inc(){
        for (int i = 0; i < HASH_CNT;++i){
            hash[i] *= HASH_KEY[i];
        }
    }
};

class eclosureHash:public basicHash{
public:
    eclosureHash(){}
    eclosureHash(basicHash element){add(element);}
    void add(basicHash element){
        for (int i = 0 ; i < HASH_CNT; ++i){
            hash[i] += element[i];
        }
    }
    bool operator ==(const eclosureHash &other)const{
        bool flag = 1;
        int cnt = 0;
        for (int i = 0 ; i <HASH_CNT; ++i){
            if (hash[i] != other.hash[i]){
               flag = 0;
               // return 0;
            } else {
                ++cnt;
            }
        }
        if (!flag&&cnt){
            cout<<"WARNING HASH INFLECT DETECTED! CNT:"<<cnt<<endl;
            system("pause");
        }
        return flag;
        return 1; 
    }
};

struct hashFunction{
	size_t operator()(const eclosureHash & p) const{
		return hash<long long>()(p[0]);
	}
};






/*

{
unorder_map<string,int> name1;
{
    int cnt = 255;
    name1[input] = ++cnt;
    name2[cnt-1] = input;


unorder_map<int,int> name5;
name5.insert()





vector<string> name2;
vector<string> produce..
}

//项目集里的语句
class produceInCluster{
public:
    int place ;
    int produceId;
    vector<int> charSet;

    构造函数

    hash
    int getNext(); //
    produceInCluster expandNext(){


    }
};

class LReClosure_Node{
    unordered_set<long long> has;
    vector<produceInCluster>
    set<int>
    hash
    #define _state_type map<int,int>
     _state_type state;

    #ifdef VISUAL
    visualFA<int> &fa; 
    vector<pair<int,int>> edge;
    bool aflag = 0;
    #endif

    void addTrans(int target,int c);
    void getTrans(int c);

    void expand(){
    }


    
    if (!xxx.count(c)){
        xxx.insert(c);
    } s

    
    
}


class LR public:basicFA<LReClosure_Node>{
    int add();
    int add(T);
    int head();

}







*/