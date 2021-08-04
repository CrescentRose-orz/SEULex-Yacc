#include<iostream>
#include<string>
#include<vector>
class basicToken{
private:
    int line;
    int idx;
    std::string val;
public:
    int getPlace(int &line,int &idx);
    basicToken(int,int,std::string);

};


class basicASTNode{
private:
    std::string name;
    basicASTNode *fa;
    std::vector<basicASTNode*> childs;
public:
    virtual void genCode();
};

class stmtASTNode:basicASTNode{

};

class declASTNode:basicASTNode{

};

class typeASTNode:basicASTNode{

}
