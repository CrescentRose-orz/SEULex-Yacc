#include<bits/stdc++.h>
using namespace std;


template<typename T>
class edge{
private:
    string label;
    T from;
    T to;
public :
    void print()
    {
         cout<<from<<" -> "<<to<<" [label =\" "<<label<<"\" ]"<<endl;
    }
    edge(T from,T to,string label);
    edge();
};

template<typename T>
class visualFA{
private:
    vector<T> doubleCircle;
    vector<edge> edgePool;
public :
    //visualFA(vector<T> dC,vector<edge> eP);
    visualFA();

    void addNode(T label)
    {
        doubleCircle.push_back(label);
    }

    void addEdge(T from,T to, string label)
    {
        edgePool.push_back(label,from,to);
    }

    void print()
    {
        cout<<"digraph finite_state_machine {"<<endl<<"rankdir=LR;"<<endl;
        cout<<"size = \""<<i+j<<","<<doubleCircle.size()<<"\""<<endl;

        cout<<"node [shape = doublecircle]; ";
        for(int i=0;i<doubleCircle.size();i++)
        {
        cout<<doubleCircle[i]<<" ";
        }
        cout<<";"<<endl;

        cout<<"node [shape = circle];"<<endl;
        for(int j=0;j<edgePool.size();j++)
        {
            edgePool[j].print();
        }
    }
};



