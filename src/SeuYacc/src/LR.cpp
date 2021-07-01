#include"LR.h"

// 定义acc
#define acc StrToInt.size()+1

LR::LR(){
    tail = 0;
}

LR::LR(Logger &log):basicFA<LR_Node>(log){}

int LR::add(){
    pool.push_back(LR_Node());
    pool[tail].idx = tail;
    return tail++;
    // todo vlr update
}

int LR::add(LR_Node &node){
    node.idx = tail;
    pool.push_back(node);
    return tail++;
    // todo vlr update
}

int LR::addTrans(int from, int to, int c){
    if( from < 0 || to < 0 || from >= tail || to >= tail){
        throw invalid_argument("out of bound error occurs in adding trans edge in LR");
    }
    pool[from].addTrans(to , c);
    return 0;
}


int LR::consturctLR(){
unordered_map <eclosureHash, int, hashFunction> wholeLRNode;
unordered_map <eclosureHash, int, hashFunction> vis;
queue<int> q;
int newId;
bool flag = 1;
    if (pool.empty()){
        return -1;
    }
    q.push(0);
    while (!q.empty()){
        int now = q.front();
        q.pop();
        //pool[now].
        for (auto &next:pool[now].allNexts){
            LR_Node tmp;
            auto iterPair = pool[now].nextPros.equal_range(next);
            for (auto iter = iterPair.first; iter != iterPair.second; ++iter){
                if (!pool[now].producers[iter->second].isEnd()){
                    tmp.addProducer(pool[now].producers[iter->second].move());
                }
            }
            if (vis.count(tmp.getHash())){
                newId = vis[tmp.getHash()];
            } else {
                eclosureHash oldHash = tmp.getHash();
                tmp.solveEclosure();
                if (wholeLRNode.count(tmp.getHash())){
                    newId = wholeLRNode[tmp.getHash()];
                } else {
                    newId = add(tmp);
                    q.push(newId);
                    wholeLRNode[tmp.getHash()] = newId;
                }
                vis[oldHash] = newId;
            }
            addTrans(now,newId,next);
        }
    }
    return  pool.size();
}

int LR::constructParsingTable(){
    // 首先生成y.tab.h，其中存储了终结符与int之间的映射
    ofstream out;
	out.open("y.tab.h", ios::out);
	out << "#ifndef Y_TAB_H" << endl;
	out << "#define Y_TAB_H" << endl;

    out << endl;

    out << "// epsilon" << endl;
    out << "#difine " << "" <<"epsilon -1" << endl;

    out << endl;

    out << "// terminals" << endl;
    for (int i = 0; i < TNBound; i++)
		out << "#difine " << IntToStr[i] <<" "<< i << endl;

    out << endl;

    out << "// nonterminals" << endl;
    for (int i = TNBound; i < NLBound; i++)
		out << "#difine " << IntToStr[i] <<" "<< i << endl;
    
    out << endl;
	
	out << "#endif" << endl;
	out.close();

    // 其次生成y.tab.cpp，其中存储SeuYacc的输出结果
    out.open("y.tab.c", ios::out);

    // ----------头文件部分----------
	out << "#include \"y.tab.h\"" << endl;

    // -----------命名空间-----------
    out << "using namespace std;" << endl;

    // ---------全局变量部分---------
    out << endl;
    int state_num = this->pool.size();
    int nonterminal_num = NLBound - TNBound;
    // 生成并输出GOTO表，GOTO表的结构为int[状态个数][非终结符个数]
    out << "int GOTO[" << state_num << "][" << nonterminal_num << "] = {"<< endl;
    for(int i = 0; i < state_num; i++){
        // 存储每一个状态的GOTO子表(一维数组int[非终结符个数])
        int *temp = new int[nonterminal_num];
        // 初始化每一项均为-1(出错)
        for(int j = 0; j < nonterminal_num; j++)
            temp[j] = -1;

        // 根据项目集簇生成GOTO表
        for(auto iter = this->pool[i].getAllTransBegin(); iter != this->pool[i].getAllTransEnd(); ++iter){
            // 确认其为非终结符
            if(iter->first < NLBound && iter->first >= NLBound)
                temp[iter->first - NLBound] = iter->second;


        // 将temp写入GOTO表
        out << "\t{";
        for(int j = 0; j < nonterminal_num - 1; j++)
            out << temp[j] << ", ";
        out << temp[nonterminal_num - 1] << "}," << endl;

        delete []temp;
    }

    out << "};" << endl << endl;

    // 生成并输出ACTION表，ACTION表的结构为int[状态个数][终结符个数]
    // 约定0为出错，+x代表移进到第x-1个状态，-x代表使用第x-1号产生式规约，+x其中x=StrToInt.size()+1代表acc
    int terminal_num = TNBound;
    out << "int ACTION[" << state_num << "][" << terminal_num << "] = {"<< endl;
    for(int i = 0; i < state_num; i++){
        // 存储每一个状态的ACTION子表(一维数组int[终结符个数])
        int *temp = new int[terminal_num];
        // 初始化每一项均为0(出错)
        for(int j = 0; j < terminal_num; j++)
            temp[j] = 0;

        // 根据项目集簇生成ACTION表
        // 规约项
        set<LR_Producer> pros = this->pool[i].getAllReduce();
        for(auto & pro : pros)
            for(auto & num : pro.lookAhead){
                // acc情况，即对向前看#符号进行产生式号为0(start->translation_unit)的规约
                if(pro.producer == 0 && IntToStr[num] == "#")
                    temp[num] = acc;
                else
                    temp[num] = -(pro.producer + 1);
            }
        
        // 移进项
        for(auto iter = this->pool[i].getAllTransBegin(); iter != this->pool[i].getAllTransEnd(); ++iter)
            // 确认其为非终结符
            if(iter->first < NLBound){
                // 出现移进规约冲突，报错
                if(temp[iter->first] != 0){
                    cout << "ERROR: Not A LALR grammar!\n" <<endl;
                    return 1;
                }
                temp[iter->first] = iter->second + 1;
            }

        // 将temp写入ACTION表
        out << "\t{";
        for(int j = 0; j < terminal_num - 1; j++)
            out << temp[j] << ", ";
        out << temp[terminal_num - 1] << "}," << endl;

        delete []temp;
    }

    out << "};" << endl << endl;
    
    
    // 构建成功
    return 0;
}