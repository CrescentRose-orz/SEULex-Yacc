#include"visualGraph.h"

using namespace std;



void edgeStyle::setFormat(string format){
    this->format = format;
}


LR_edge::LR_edge(enum LR_edgeStyle type,char* name){
char s[100];
    if (type) {
        sprintf(s,"[ penwidth = 5 fontsize = 28 fontcolor = \"black\" label = \"%s\" ];",name);
        setFormat(s);
    } else {
        sprintf(s,"[ penwidth = 1 fontsize = 14 fontcolor = \"grey28\" label = \"%s\" ];",name);
        setFormat(s);            
    }
}


class nodeStyle{
    
};


