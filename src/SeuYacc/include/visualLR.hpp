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
string trans[1000];
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
        //trans["] = "ε";
        trans['\\'] ="\\\\";
        trans['\"']="\\\"";
        doubleCircle.clear();
        edgePool.clear();
    }

    void printNode(fstream &fout,LR_Node &node){
        fout<<"state"<<node.idx<<R"([ style = "filled" penwidth = 1 fillcolor = "white" fontname = "Courier New" shape = "Mrecord" label =<<table border="0" cellborder="0" cellpadding="3" bgcolor="white"><tr><td bgcolor="black" align="center" colspan="2"><font color="white">State #)"<<node.idx<<"</font></td></tr>)";
        for (auto iter = node.producers.cbegin();iter != node.producers.cend();++iter){
            printProducer(fout,iter->second);
        }
        fout<<"</table>> ];"<<endl;
        for (auto iter = node.getAllTransBegin();iter!=node.getAllTransEnd();++iter){
            fout<<"state"<<node.idx<<"->state"<<iter->second;
            if (isTerminal(iter->first)){
                fout<<R"([ penwidth = 1 fontsize = 14 fontcolor = "grey28" label = ")"<<I2S(iter->first)<<" ]"<<endl;
            } else {
                fout<<R"([ penwidth = 5 fontsize = 28 fontcolor = "black" label = ")"<<I2S(iter->first)<<" ]"<<endl;
            }
        }
    }

    void printProducer(fstream &fout,LR_Producer pro){
        fout<<R"(<tr><td align="left" port="r")<<pro.producer<<R"(">&#40;")<<pro.producer<<R"(&#41;)";
        fout<<I2S(getLeft(pro.producer))<<" -&gt ";
        four (int i = 0; i < getRight(pro.producer).size();++i){
            if (i == pro.nowPlace){
                fout<<"&bull;";
            }
            fout<<I2S(getRight(pro.producer)[i]);
            fout<<R"(</td><td bgcolor="grey" align="right">)"
            for (auto &id:pro.lookAhead){
                fout<<I2S(id)<<" ";
            }
            fout<<R"(</td></tr>)";
        }
    }
    void print(fstream &fout,LR &_LR)
    {
        fout<<"digraph finite_state_machine {"<<endl<<"rankdir=LR;"<<endl;
        fout<<"  ratio = auto; "<<endl;
        for (auto &node:_LR.pool){
            printfNode(fout,node);
        }
        fout<<"}"<<endl;
        fout.close();
    }
};



#endif






/*







"state0" 
[ style = "filled, bold" penwidth = 5 fillcolor = "white" fontname = "Courier New" shape = "Mrecord" 
label =<
<table border="0" cellborder="0" cellpadding="3" bgcolor="white">
<tr><td bgcolor="black" align="center" colspan="2"><font color="white">State #0</font></td></tr>
<tr><td align="left" port="r0">
    &#40;0&#41; s -&gt; &bull;e $ 
</td></tr>
<tr><td align="left" port="r1">
    &#40;1&#41; e -&gt; &bull;l '=' r 
</td></tr>
<tr><td align="left" port="r2">
    &#40;2&#41; e -&gt; &bull;r 
</td></tr>
<tr><td align="left" port="r3">
    &#40;3&#41; l -&gt; &bull;'*' r 
</td></tr>
<tr><td align="left" port="r4">
    &#40;4&#41; l -&gt; &bull;'n' 
</td></tr>
<tr><td align="left" port="r5">
    &#40;5&#41; r -&gt; &bull;l 
</td></tr>
</table>
> ];


"state9" [ style = "filled" penwidth = 1 fillcolor = "white" fontname = "Courier New" shape = "Mrecord" label =<<table border="0" cellborder="0" cellpadding="3" bgcolor="white"><tr><td bgcolor="black" align="center" colspan="2"><font color="white">State #9</font></td></tr>
<tr><td align="left" port="r2">
    &#40;2&#41; e -&gt; r &bull;
</td><td bgcolor="grey" align="right">$</td></tr></table>> ];







*/