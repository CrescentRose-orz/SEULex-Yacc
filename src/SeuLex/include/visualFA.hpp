#pragma once
#include<bits/stdc++.h>
using namespace std;




template<typename T>
class visualFA{
private:
template<typename D>
class edge{
private:
    string label;
    D from;
    D to;
public :
    void print(fstream &fout)
    {
         fout<<from<<" -> "<<to<<" [label =\" "<<label<<"\" ]"<<endl;
    }
    edge(D from,D to,string label);
    edge();
};
    vector<T> doubleCircle;
    vector<edge<T> > edgePool;
public :
    //visualFA(vector<T> dC,vector<edge> eP);
    visualFA();

    void addNode(T label)
    {
        doubleCircle.push_back(label);
    }

    void addEdge(T from,T to, string label)
    {
        edgePool.push_back(edge<T>(from,to,label));
    }

    void print(fstream &fout)
    {
        fout<<"digraph finite_state_machine {"<<endl<<"rankdir=LR;"<<endl;
      //  fout<<"size = \""<<i+j<<","<<doubleCircle.size()<<"\""<<endl;

        fout<<"node [shape = doublecircle]; ";
        for(int i=0;i<doubleCircle.size();i++)
        {
        fout<<doubleCircle[i]<<" ";
        }
        fout<<";"<<endl;

        fout<<"node [shape = circle];"<<endl;
        for(int j=0;j<edgePool.size();j++)
        {
            edgePool[j].print(fout);
        }
        fout.close();
    }
};



