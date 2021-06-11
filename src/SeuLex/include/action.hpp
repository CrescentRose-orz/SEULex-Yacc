#ifndef ACTION
#define ACTION
#include<string>

using namespace std;
class action{
private:
    int index;  
    string code;
public:
    action(){
        index = -1;
    }
    action(int index ,string code){
        this->index = index;
        this->code = code;
    }
    bool operator < (const action &other) const {
        return this->index < other.index;
    }
    bool operator > (const action &other) const {
        return this->index > other.index;
    }
    bool operator == (const action &other) const {
        return this->index == other.index;
    }
    bool operator != (const action &other) const {
        return this->index != other.index;
    }
    string getCode(){
        return code;
    }
    int getIdx(){
        return index;
    }
};

#endif