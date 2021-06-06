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
public:
    basicHash(){memset(hash,0,sizeof(hash));}
    basicHash(int idx){
        for (int i = 0; i < HASH_CNT;++i){
            hash[i] = qPow(HASH_KEY[i],idx);
        }
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
    }
    vector<long long> getVal(){return vector<long long>(hash,hash+HASH_CNT-1);}
    long long operator[](int i) const {
        if (i<0 || i >= HASH_CNT){
            throw invalid_argument("index out of bound in basicHash[]");
        }
        return hash[i];
    }
    bool operator == (basicHash &other){
        for (int i = 0 ; i <HASH_CNT; ++i){
            if (hash[i] != other.hash[i]){
                return 0;
            }
        }
        return 1;
    }
};

class eclosureHash:public basicHash{
public:
    eclosureHash(){}
    eclosureHash(basicHash element){add(element);}
    //void add(NFA_Node);
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

class NFA_eclosure{
public:
    set<int> NFAs;
    eclosureHash hash;
    NFA_eclosure():hash(){}
    NFA_eclosure(int idx):hash(basicHash(idx)){}
    void add(int idx){ hash.add(basicHash(idx));}
    bool operator == (NFA_eclosure &other){
        return this -> hash == other.hash;
    }
};

struct hashFunction{
	size_t operator()(const eclosureHash & p) const{
		return hash<long long>()(p[0]);
	}
};

void getEclosure(){
queue<NFA_eclosure> q;
unordered_map<eclosureHash,hashFunction> hasVisited;
    q.push(NFA_eclosure())

}





class DFA_Node{



};

class DFA{
private:
    int tmp;
public:
    mutex Wrlock;
    vector<NFA_Node> pool;
    int head = 0;
    int tail = 0;
    int add(){
        pool.emplace_back();
    }

    int add(NFA_Node node){
        unique_lock<mutex> lock(Wrlock);
        pool.emplace_back(node);
        ++tail;
        return tail-1;
    }
    #ifdef VISUAL
    visualFA<int> vNFA;

    #endif
};