#pragma once
#include<bits/stdc++.h>

using namespace std;
const int HASH_CNT = 2;
const int MAX_HASH_CNT = 2;
const long long HASH_KEY[MAX_HASH_CNT] = {19260817,2281701377};
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
    /**
     * @brief Construct a new basic Hash object
     * 
     * @param idx 非负整数，该元素的非重复编号 
     */
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
    /**
     * @brief Construct a new eclosure Hash object
     *        由单元素的basicHash构造一个有初始集合元素的hash
     * @param element basicHash
     */
    eclosureHash(basicHash element){add(element);}
    /**
     * @brief 加入一个集合元素。
     *  
     * @param element 该元素的basicHash
     */
    void add(basicHash element){
        for (int i = 0 ; i < HASH_CNT; ++i){
            hash[i] += element[i];
        }
    }
    /**
     * @brief 加入一个集合元素。等价于add(basicHash(element));
     * 
     * @param element 非负整数 element为改元素的非负不重复编号
     */
    void add(int element){
        add(basicHash(element));
    }
    eclosureHash(int element){
        eclosureHash(basicHash(element));
    }
    bool operator ==(const eclosureHash &other)const{
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
