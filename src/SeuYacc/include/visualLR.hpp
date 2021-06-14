#ifndef VISUALFA
#define VISUALFA
#include"CONSTANT.h"
#include"DataStructure.h"
#include"LR_Node.h"
using namespace std;

const int CHARSIZE = charSetMAX+5;

template<typename T>
class visualLR{
private:
string trans[CHARSIZE];
template<typename D>
class edge{
private:
    int label;
    D from;
    D to;
public :
    void print(fstream &fout)
    {
        if (!isTerminal(label)){
            if (label < 128){
                fout<<from<<" -> "<<to<<" [ penwidth = 5 fontsize = 28 fontcolor = \"black\"  label =\" "<<trans[label]<<"\" ]"<<endl;
            } else {
                fout<<from<<" -> "<<to<<" [ penwidth = 5 fontsize = 28 fontcolor = \"black\"  label =\" "<<IntToStr[label]<<"\" ]"<<endl;
            }
        } else {
            fout<<from<<" -> "<<to<<" [penwidth = 1 fontsize = 14 fontcolor = \"grey28\"  label =\" "<<IntToStr[label]<<"\" ]"<<endl;            
        }
    }
    edge(D from,D to,int label){
        this -> from = from;
        this -> to = to;
        this -> label = label;
    }
    edge(){
        from = to = 0;
        label = 0;
    }
};
    vector<LR_Node> doubleCircle;
    vector<edge<int> > edgePool;
public :
    //visualFA(vector<T> dC,vector<edge> eP);
    visualLR(){
        for (int i = 0; i < charSetMin; ++i){
            trans[i] ="\\\\"+to_string(i);
        }
        for (int i = charSetMin; i < min(256,CHARSIZE); ++i){
            trans[i] += (char)i;
        }
        trans['\n']="\\\\n";
        trans[eps] = "ε";
        trans['\\'] ="\\\\";
        trans['\"']="\\\"";
        doubleCircle.clear();
        edgePool.clear();
    }

    void addNode(LR_Node node)
    {
       // doubleCircle.push_back(label);
    }

    void addEdge(T from,T to, string label)
    {

        edgePool.push_back(edge<T>(from,to,label));
    }
    void addEdge(T from,T to, int label)
    {
        if (label >= CHARSIZE){
            cerr<<"label int "<<label<<" exceeded CHARSIZE"<<CHARSIZE<<endl;
            throw invalid_argument("overflow");
        }
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