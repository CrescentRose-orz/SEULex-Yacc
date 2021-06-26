#include"header.h"
#include"Hash.hpp"


class LR_Producer{
private:

public:
    int producer;
    int nowPlace;
    set<int> lookAhead;
    /**
     * @brief 为该产生式+目前位置下的向前看符生成一个唯一编号
     * 
     * @param prIdx 产生式编号
     * @param nowIdx 当前点位置nowIdx belongs to [0,length]，表示在第nowIdx符号前，length表示在末尾
     * @param lookAhead 向前看符编号
     * @return long long 
     */
    static long long getIdentifier(int prIdx,int nowIdx,int lookAhead){
        long long rt = 0;
        rt = prIdx << maxLen;
        rt = (rt | nowIdx) << maxIdx;
        rt |= lookAhead;
        return rt;
    }
    /**
     * @brief 为该产生式+目前位置下的向前看符生成一个唯一编号
     * 
     * @param lookAhead 对何向前看符生成。对于每个向前看符均生成：
     * for(auto &l:example.lookAhead){xxx = example.getIdentifier(l);}
     * @return long long 
     */
    long long getIdentifier(int lookAhead){
        long long rt = 0;
        rt = producer << maxLen;
        rt = (rt | nowPlace) << maxIdx;
        rt |= lookAhead;
        return rt;
    }    
    /**
     * @brief Construct a new lr producer object
     * 
     * @param prIdx 产生式编号
     * @param nowIdx 当前点位置nowIdx belongs to [0,length]，表示在第nowIdx符号前，length表示在末尾
     */
    LR_Producer(int prIdx,int nowIdx):producer(prIdx),nowPlace(nowIdx){}
    /**
     * @brief Construct a new lr producer object with a initial lookAhead simbol
     * 
     * @param prIdx 产生式编号
     * @param nowIdx 当前点位置nowIdx belongs to [0,length]，表示在第nowIdx符号前，length表示在末尾
     * @param lookAhead 向前看符编号
     */
    LR_Producer(int prIdx,int nowIdx,int lookAhead):producer(prIdx),nowPlace(nowIdx){
        addLookAhead(lookAhead);
    }
    /**
     * @brief 添加向前看符,内部提供重复检查无需判重
     * @param[in] idx向前看符的全局int标号
     * 
     */
    void addLookAhead(int idx){
        if(!lookAhead.count(idx)){
            lookAhead.insert(idx);
        }
    }
    /**
     * @brief 添加向前看符,内部提供重复检查无需判重，且更新所属LR节点的hash。
     * @param[in] idx向前看符的全局int标号
     * @param[in] _hash 此产生式所属的LR节点的LR_Node_Hash
     * 
     */
    void addLookAhead(int idx,eclosureHash &_hash){
        if(!lookAhead.count(idx)){
            lookAhead.insert(idx);
            _hash.add(getIdentifier(idx));
        }
    }
    /**
     * @brief 下一个需匹配符号的编号。
     * 
     * @return int 若已在末尾，返回-1。
     */
    int getNext(){
        return nowPlace!=getLength()?IntToStr[producer][nowPlace]:-1;
    }
    int getLength(){
        return IntToStr[producer].length();
    }
    operator int(){
        return producer;
    }
    
};

// struct HashFunForProducer{
//     std::size_t operator()(const LR_Producer &p) const{
//         return std::hash<int>()(p.producer);
//     }
// };