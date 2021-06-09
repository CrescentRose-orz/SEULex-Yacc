#pragma once
#include<bits/stdc++.h>

using namespace std;
const int HASH_CNT = 2;
const int HASH_KEY[HASH_CNT] = {19260817,2281701377};
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
    long long operator[](int i) const {
        if (i<0 || i >= HASH_CNT){
            throw invalid_argument("index out of bound in basicHash[]");
        }
        return hash[i];
    }
    bool operator == (basicHash &other){
        if (!valid){
            throw invalid_argument("this hashKey has not been initialized!");
        }
        for (int i = 0 ; i <HASH_CNT; ++i){
            if (hash[i] != other.hash[i]){
                return 0;
            }
        }
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
    bool operator ==(eclosureHash other){
        for (int i = 0 ; i <HASH_CNT; ++i){
            if (hash[i] != other.hash[i]){
                return 0;
            }
        }
        return 1; 
    }
};

struct hashFunction{
	size_t operator()(const eclosureHash & p) const{
		return hash<long long>()(p[0]);
	}
};
