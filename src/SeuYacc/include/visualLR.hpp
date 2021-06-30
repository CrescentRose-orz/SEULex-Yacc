#ifndef VISUALFA
#define VISUALFA
#include"CONSTANT.h"
#include"DataStructure.h"
#include"LR_Node.h"
#include"LR.h"
using namespace std;

//const int CHARSIZE = 10000;
//int charSetMin = 0;
//int charSetMax = 10000;
template<typename T>
class visualLR{
private:
string trans[CHARSIZE];
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

    void printNode(fstream &fout,LR_Node &node){
        fout<<R"()"
  "state9" [ style = "filled" penwidth = 1 fillcolor = "white" fontname = "Courier New" shape = "Mrecord" label =<<table border="0" cellborder="0" cellpadding="3" bgcolor="white"><tr><td bgcolor="black" align="center" colspan="2"><font color="white">State #9</font></td></tr><tr><td align="left" port="r2">&#40;2&#41; e -&gt; r &bull;</td><td bgcolor="grey" align="right">$</td></tr></table>> ];
    }

    void print(fstream &fout,LR &_LR)
    {
        fout<<"digraph finite_state_machine {"<<endl<<"rankdir=LR;"<<endl;
        fout<<"  ratio = auto; "<<endl;
        for (auto &node:_LR.pool){
            printfNode(fout,node);
        }
    }
};



#endif