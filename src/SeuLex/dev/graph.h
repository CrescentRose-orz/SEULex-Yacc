#include <bits/stdc++.h>
#define to(x) _edge[x].to
#define next(x) _edge[x].next

using namespace std;


class edge{
public:    
    int next;
    int to;
    edge(int to,int next){
        this-> next = next;
        this-> to = to;
    }
    edge(){
        next = to = 0;
    }
};

/*
    This graph only support node labeled with 1-n.
    The memory complexity is O(n + m)
    n is the largest label number
    m is the number  of edge
    for unconsistent label number / string ,use mappingGraph instead

*/
class graph{
    vector<int> head;
    vector<edge> _edge;
    int maxN ;
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
        _edge.emplace_back(_tmpEdge);
        maxN = max(maxN, max(from, to));
        return _edge.size() - 1;
    } 
    void reSize(int size){
        if (head.size() < size)
            head.resize(size);
    }
    vector<int> topSort(int n, bool &hasCircle){
        hasCircle = false;
        vector<int> rt;
        vector<int> inDeg;
        queue<int> toHandle;
        inDeg.resize(maxN+1);
        int e;
        for (int s = 1; s <= n; ++s){
            for (e = head[s] ; e ; e =next(e) ){
                ++inDeg[to(e)];
            }
        }
        for (int s = 1; s <= n; ++s){
            if (!inDeg[s]){
                toHandle.push(s);
            }
        }
        while(!toHandle.empty()){
            int s = toHandle.front();
            toHandle.pop();
            rt.emplace_back(s);
            for (e = head[s] ; e ; e =next(e) ){
                if(!(--inDeg[to(e)])){
                    toHandle.push(to(e));
                }
            }  
        }
        if (rt.size()!=n){
            hasCircle = true;
        }
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
    vector<T> topSort(bool &hasCircle){
        vector<int> &&tmp = _graph.topSort(tot,hasCircle);
        vector<T> rt;
        rt.resize(tot);
        for (int i = 0 ; i < rt.size(); ++i){
            rt[i] = reverseMp[tmp[i]];
        }
    }
};