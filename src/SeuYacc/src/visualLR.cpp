#include"visualLR.h"



    
    #define tI2S(x) (x<128?trans[I2S(x)]:I2S(x))

    visualLR::visualLR(){
        for (int i = 0; i < 256; ++i){
            string tmp;
            tmp +=(char)i;
            trans[tmp] ="\'testing\'";//"\'"+tmp+"\'";
        }

        trans["\n"]="\\\\n";
        //trans["] = "ε";
        trans["\\"] ="\\\\";
        trans["\""]="\\\"";
    }

    void visualLR::printNode(fstream &fout,LR_Node &node){
        fout<<"state"<<node.idx<<R"([ style = "filled" penwidth = 1 fillcolor = "white" fontname = "Courier New" shape = "Mrecord" label =<<table border="0" cellborder="0" cellpadding="3" bgcolor="white"><tr><td bgcolor="black" align="center" colspan="2"><font color="white">State #)"<<node.idx<<"</font></td></tr>";
        for (auto iter = node.producers.cbegin();iter != node.producers.cend();++iter){
            printProducer(fout,iter->second);
        }
        fout<<"</table>> ];"<<endl;
        for (auto iter = node.getAllTransBegin();iter!=node.getAllTransEnd();++iter){
            fout<<"state"<<node.idx<<"->state"<<iter->second;
            if (isTerminal(iter->first)){
                fout<<R"([ penwidth = 1 fontsize = 14 fontcolor = "grey28" label = ")"<<I2S(iter->first)<<"\" ]"<<endl;
            } else {
                fout<<R"([ penwidth = 5 fontsize = 28 fontcolor = "black" label = ")"<<I2S(iter->first)<<"\" ]"<<endl;
            }
        }
    }

    void visualLR::printProducer(fstream &fout,LR_Producer pro){
        fout<<R"(<tr><td align="left" valign="top" port="r)"<<pro.producer<<R"(">&#40; )"<<pro.producer<<R"( &#41; )";
        fout<<tI2S(getLeft(pro.producer))<<" -&gt; ";
        for (int i = 0; i < getRight(pro.producer).size();++i){
            if (i == pro.nowPlace){
                fout<<"&bull;";
            }
            fout<<tI2S(getRight(pro.producer)[i]);
        }
        if (pro.isEnd()){
            fout<<"&bull;";   
        }
        fout<<R"(</td><td bgcolor="grey" align="right">)";
        for (auto &id:pro.lookAhead){
            fout<<tI2S(id)<<" ";
        }
        fout<<R"(</td></tr>)";
    }
    void visualLR::print(fstream &fout,LR &_LR)
    {
        fout<<"digraph finite_state_machine {"<<endl<<"rankdir=LR;"<<endl;
        fout<<"  ratio = auto; "<<endl;
        for (auto &node:_LR.pool){
            printNode(fout,node);
        }
        fout<<"}"<<endl;
        fout.close();
    }


