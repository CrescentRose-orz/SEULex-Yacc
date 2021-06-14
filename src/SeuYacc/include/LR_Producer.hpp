#include"header.h"
#include"Hash.hpp"


class LR_Producer{
private:

public:
    int producer;
    int nowPlace;
    set<int> lookAhead;
    LR_Producer(int ,int);
    /**
     * @brief 添加向前看符,内部提供重复检查无需判重
     * @param[in] idx 向前看符的全局int标号
     * 
     */
    void addLookAhead(int idx){
        if(!lookAhead.count(idx)){
            lookAhead.insert(idx);
        }
    }


};