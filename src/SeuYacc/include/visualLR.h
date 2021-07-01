#ifndef VISUALFA
#define VISUALFA
#include"CONSTANT.h"
#include"LR_Node.h"
#include"LR.h"
#include"LR_Producer.hpp"
#include"DataStructure.h"
using namespace std;


class LR;

class visualLR{
private:
map<string,string> trans;
public :
    //visualFA(vector<T> dC,vector<edge> eP);
    visualLR();
    void printNode(fstream &fout,LR_Node &node);
    void printProducer(fstream &fout,LR_Producer pro);
    void print(fstream &fout,LR &_LR);
};



#endif





