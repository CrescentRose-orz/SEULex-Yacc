#ifndef VISUALFA
#define VISUALFA
#include<bits/stdc++.h>
#include"CONSTANT.h"
using namespace std;

const int CHARSIZE = charSetMAX;


template<typename T>
class visualFA{
private:
string trans[CHARSIZE];
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
    edge(D from,D to,string label){
        this -> from = from;
        this -> to = to;
        this -> label = label;
    }
    edge(){
        from = to = 0;
        label = "undefined";
    }
};
    vector<T> doubleCircle;
    vector<edge<T> > edgePool;
public :
    //visualFA(vector<T> dC,vector<edge> eP);
    visualFA(){
        for (int i = 0; i < 32; ++i){
            trans[i] ="\\\\"+to_string(i);
        }
        for (int i = 32; i < min(256,CHARSIZE); ++i){
            trans[i] += (char)i;
        }
        trans['\n']="\\\\n";
        trans[eps] = "ε";
        trans['\\'] ="\\\\";
        trans['\"']="\\\"";
    }

    void addNode(T label)
    {
        doubleCircle.push_back(label);
    }

    void addEdge(T from,T to, string label)
    {
        edgePool.push_back(edge<T>(from,to,label));
    }
    void addEdge(T from,T to, int label)
    {
        edgePool.push_back(edge<T>(from,to,trans[label]));
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
        fout<<"}"<<endl;
        fout.close();
    }
};



#endif