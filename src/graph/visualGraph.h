#ifndef VISUALGRAPHNODE
#define VISUALGRAPH
#include <vector>
#include <iostream>
#include<fstream>
#include <cstdio>
#include<string>
using namespace std;

enum LR_edgeStyle{
    progress,eliminate
};

enum FA_style{
    start,end
};




class edgeStyle{
private:
    string format;
public:
    edgeStyle();
    void setFormat(string format);
};

class LR_edge:edgeStyle{
    string name;
    LR_edge(enum LR_edgeStyle type,char* name);
};


class nodeStyle{
    string format;
    void setFormat();
};


class visualGraphNode{
public :
    nodeStyle val;    
    visualGraphNode();
    visualGraphNode(nodeStyle,string);

    int printNode();
    
};

class visualGraph{
    vector<visualGraphNode*> graph;
    visualGraph();
    int printGraph();
    int addNode(visualGraphNode);
    int dfs(int,int);
};

#endif