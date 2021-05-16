#include <bits/stdc++.h>

using namespace std;


class edge{
    int next;
    int to;
public:
    edge(int to,int next){
        this-> next = next;
        this-> to = to;
    }
    edge(){
        next = to = 0;
    }
};


class graph{
    vector<int> head;
    vector<edge> _edge;
public :
    graph(){
        //head.push_back(0);
        _edge.push_back(edge());
        head.resize(10000);    //resize will set all empty head to 0
    }
    int add_edge(int from,int to){
        if (from >= head.size()){
            head.resize(head.size()<<1);
        }
        edge _tmpEdge(to,head[from]);
        head[from] = _edge.size();
        _edge.push_back(_tmpEdge);
        return _edge.size() - 1;
    } 
    void reSize(int size){
        if (head.size() < size)
            head.resize(size);
    }
    vector<int> topSort(int n){
        vector<int> rt;



        return rt;
    }
};

template<typename T>
class mappingGraph{
private:
    int tot = 0;
    graph _graph;
    map<T,int> mp;
    vector<T> reverseMp;
    int checkExist(T newNode){
        if (mp.count(newNode)){
            return mp[newNode];
        }
        map[newNode] = ++tot;
        reverseMp.emplace_back(T);
        return tot;
    }
public :
    void add_edge(T from,T to){
        _graph.add_edge(checkExist(from), checkExist(to))
    }
    vector<T> topSort(){
        vector<int> &&tmp = _graph.topSort(tot);
        vector<T> rt;
        rt.resize(tot);
        for (int i = 0 ; i < rt.size(); ++i){
            rt[i] = reverseMp[tmp[i]];
        }
    }
};