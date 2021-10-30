#include"NFA_Cluster.h"
    //todo:[^]
    bool NFA_Cluster::checkBracket(string &bracket,int l,int r){
        int wrong = 0;
        int last = l - 1;
        if (l>r){
            return 0;
        }
        for (int i = l; i <= r;++i){
            if (bracket[i] =='-'){
                if (i - 1 <= last ||i == r|| i == l|| bracket[i-1]>=bracket[i+1]){
                    ++wrong;
                    if (wrong == 2){
                        return 0;
                    }
                } else {
                    last = i + 1;
                }
            }
        }
        return true;
    }
    NFA_Cluster::NFA_Cluster(int head){
        this->head = head;
        this->tail = -1;
    }
    NFA_Cluster::NFA_Cluster(int head,int tail){
        this->head = head;
        this->tail = tail;
    }
    //single char,.
    NFA_Cluster::NFA_Cluster(NFA &buff,char c){
        tail = buff.add();
        head = buff.add();
        {                    
            WrLock(buff.Wrlock);
            if (c!='.'){
                buff[head].addTrans(tail,c);
            } else {
                buff[head].addMultiTrans(tail,charSetMin,charSetMAX);
                buff[head].addMultiTrans(tail,controlMin,controlMax);
            }
        }
    }
    NFA_Cluster NFA_Cluster::getTrans(NFA &buff,char c){
        int tail = buff.add();
        int head = buff.add();
        {                    
            WrLock(buff.Wrlock);
            buff[head].addTrans(tail,RE::trans(c));
        }        
        return NFA_Cluster(head,tail);
    }
    // '|' '^'
    NFA_Cluster::NFA_Cluster(NFA &buff,char op ,NFA_Cluster a ,NFA_Cluster b ){
        switch(op){
            case '|':{
                head = buff.add();
                tail = buff.add();
                {
                    WrLock(buff.Wrlock);
                    buff[head].addTrans(a.head,eps);
                    buff[head].addTrans(b.head,eps);  
                    buff[a.tail].addTrans(tail,eps);
                    buff[b.tail].addTrans(tail,eps);
                }
                return ;
            }
            break;
            case '^':{
                head = a.head;
                tail = b.tail;
                {                
                    WrLock(buff.Wrlock);
                    buff[a.tail].addTrans(b.head,eps);
                }
                return;
            }
            break;
            default:
                throw invalid_argument("only  accept '^' or '|', but found " + op);
            
        }
    }
    //copy
    NFA_Cluster NFA_Cluster::copy(NFA &buff,NFA_Cluster &origin){
        int head = buff.add();
        unordered_set<int> vis;
        unordered_map<int,int> mapping; 
        queue<int> q;
        vis.insert(origin.head);
        mapping.insert({origin.head,head}); 
        q.push(origin.head);
        while(!q.empty()){
            int now = q.front();
            vector<pair<int,int>> &&edges = buff[now].getAllTrans();
            q.pop();
            for (auto iter = edges.begin(); iter !=edges.end(); ++iter){
                if (!mapping.count(iter->second)){
                    int newNode = buff.add();
                    mapping.insert({iter->second,newNode});
                }
                buff[mapping[now]].addTrans(mapping[iter->second],iter->first);
                if (!vis.count(iter->second)){
                    q.push(iter->second);
                    vis.insert(iter->second);
                }
            }
        }
        return NFA_Cluster(mapping[origin.head],mapping[origin.tail]);
    }
    // multi {}
    NFA_Cluster::NFA_Cluster(NFA &buff,int l,int r,NFA_Cluster a ){
        //throw invalid_argument("multi{} this function is not available");
        if (l < 0 || r < l){
            throw invalid_argument("error, {l,r} must have l >= 0 && r >= l");
        }
        if (r == 1){
            if (l == 0){
                NFA_Cluster(buff,'?',a);
            } else {
                this->head = a.head;
                this->tail = a.tail;
            }
            return;
        }
        int rtHead , rtTail = a.tail,lastTail = -1;
        for (int i = 0; i < r - 1; ++i){
            NFA_Cluster &&now = NFA_Cluster::copy(buff,a);
            if (lastTail >= 0){
                buff[lastTail].addTrans(now.head,eps);
            } else {
                rtHead = now.head;
            }
            if (l <= i){
                buff[now.head].addTrans(a.tail,eps);
            }
            lastTail = now.tail;
        }

        buff[lastTail].addTrans(a.head,eps);
        if (l<r){
            buff[a.head].addTrans(a.tail,eps);
        }
        this->head = rtHead;
        this->tail = rtTail;
        // //NFA_Node _tail;
        // vector<NFA_Node> _heads(r+1);
        // tail = buff.add(_tail);
        // for (int i = l; i <= r; ++i){
        //     _heads[i].addTrans(tail,);
        // }
    }
    //single op : * ? +
    NFA_Cluster::NFA_Cluster(NFA &buff,char op ,NFA_Cluster a ){
        NFA_Node _head(buff.vFA),_tail(buff.vFA);
        // head = buff.add();
        // tail = buff.add();      
        switch(op){
            case '*':
                tail  = buff.add();            
                _head.addTrans(a.head,eps);
                _head.addTrans(tail,eps);
                head = buff.add(_head);
                {
                    WrLock(buff.Wrlock);
                    buff.pool[a.tail].addTrans(tail,eps);
                    buff.pool[a.tail].addTrans(a.head,eps);
                }                
                break;
            case '?':
                #ifndef dev
                tail  = buff.add(_tail);            
                _head.addTrans(a.head,eps);
                _head.addTrans(tail,eps);
                head = buff.add(_head);
                _tail.addTrans(head,eps);
                {
                    WrLock(buff.Wrlock);
                    buff.pool[a.tail].addTrans(tail,eps);
                }             
                #else
                tail  = a.tail;
                head = a.head;
                {
                    WrLock(buff.Wrlock);
                    buff[head].addTrans(tail,eps);
                }            
                #endif        
                break;
            case '+':
                //tail  = buff.add();            
                //_head.addTrans(a.head,eps);
                //head = buff.add(_head);
                head = a.head;
                tail = a.tail;
                {
                    WrLock(buff.Wrlock);
                    //buff.pool[a.tail].addTrans(tail,eps);
                    buff.pool[a.tail].addTrans(a.head,eps);
                }       
                break;
            default:
                // todo: throw exception 
                break;
        }
    }
   //quotation 

    NFA_Cluster::NFA_Cluster(NFA &buff ,string &quotation , int l ,int r ){ //[l,r]

        NFA_Cluster &&rt = createEmpty(buff);  
        bool trans = 0;
        for (int i = l ; i <= r; ++i){
            if (trans){
                int tailIdx = buff.add();
                {
                    WrLock(buff.Wrlock);
                    buff[rt.tail].addTrans(tailIdx,RE::trans(quotation[i]));
                }
                rt.tail = tailIdx;
                trans = false;
            
            }
            if (quotation[i] =='\\'){
                trans = true;
                continue;
            }
            int tailIdx = buff.add();
            {
                WrLock(buff.Wrlock);
                buff.pool[rt.tail].addTrans(tailIdx,quotation[i]);            
            }
            rt.tail = tailIdx;
        }
        this -> head = rt.head;
        this -> tail = rt.tail;
    }
    //bracket todo 【】
    NFA_Cluster NFA_Cluster::getBracket(NFA &buff,string &bracket,int l,int r){
    bool skip[charSetMAX+1] = {0};
    bool trans = 0;
    bool reverse = 0;
        if (l == r){
            throw invalid_argument("empty bracket is not allowed");
        }
        NFA_Cluster rt(buff.add(),buff.add());
        if (bracket[l]=='^'){
            ++l;
            reverse = 1;
            for (int i = 0; i < charSetMin; ++i){
                skip[i] = 1;
            }
            for (int i =controlMin; i <= controlMax; ++i){
                skip[i] = 0;
            }
        }
        if (!NFA_Cluster::checkBracket(bracket,l,r)){
            stringstream s;
            s<<"syntax error in bracket expression "<<bracket.substr(l,r-l+1);
            throw invalid_argument(s.str());
        }
        for (int i = l; i <= r; ++i){
            if (trans){
                if (reverse){
                    skip[RE::trans(bracket[i])] = 1;
                } else {
                    #ifdef USE_MUTITHREAD
                    WrLock(buff.Wrlock);
                    #endif
                    buff[rt.head].addTrans(rt.tail,RE::trans(bracket[i]));
                }
                trans = 0;
                continue;
            }
            if (bracket[i] =='\\'){
                trans = 1;
                continue;
            }
            if (bracket[i] != '-'){
                if (reverse){
                    skip[bracket[i]] = 1;
                } else {
                    WrLock(buff.Wrlock);
                    buff[rt.head].addTrans(rt.tail,bracket[i]);
                }
            } else {
                if (i == l||i == r|| bracket[i-1]>=bracket[i+1]){
                    if (reverse){
                        skip[bracket[i]] = 1;
                    } else {
                        WrLock(buff.Wrlock);
                        buff[rt.head].addTrans(rt.tail,'-');    
                    }
                } else {
                    if (reverse){
                        for (int j = bracket[i-1];j <= bracket[i+1];++j){
                            skip[j] = 1;
                        }
                    } else {
                        WrLock(buff.Wrlock);
                        buff[rt.head].addMultiTrans(rt.tail,bracket[i-1],bracket[i+1]);
                    }
                }
            }
        }
        {
        if (reverse){
            #ifdef USE_MULTITHREAD
            WrLock(buff.Wrlock);
            #endif
                for (int i = 0; i <= charSetMAX; ++i){
                    if (!skip[i])
                        buff[rt.head].addTrans(rt.tail,i);
                }
            }
        }
        return rt;
    }
    //todo{}
    NFA_Cluster NFA_Cluster::getBrace(NFA &buff,NFA_Cluster a,int l ,int r){
        NFA_Cluster rt(0,0);
        
        return rt;
    }
    NFA_Cluster NFA_Cluster::createEmpty(NFA &buff){
        int && idx = buff.add();
        return NFA_Cluster(idx,idx);
    }

    NFA_Cluster NFA_Cluster::createSingle(NFA &buff,NFA_Node node){
        int && idx = buff.add(node);
        return NFA_Cluster(idx,idx);
    }





NFA_Cluster NFA_Cluster::RE2NFA_Cluster(string RE,NFA &buff){
stack<NFA_Cluster> operandStack;
stack<RE_operator> operatorStack; 
RE_operator newOp('.');
int i = 0,j;
bool trans = 0;
    RE += '$';
    #ifdef DEBUG
    stringstream s;
    s<<RE<<" started 2 NFA";
    buff.logger.customMSG(s.str());
    buff.logger.save();
    #endif
    //NFA_Cluster &&head = NFA_Cluster::createEmpty(buff);
    cout<<"get RE: "<<RE<<endl;
    while(i<RE.size()){
        #ifdef DEBUG
        stringstream s;
        s<<"meet "<<RE[i]<<"the size of operandStack is "<<operandStack.size()<<" opstack is "<<operatorStack.size();    
        buff.logger.customMSG(s.str());
        buff.logger.save();
        fstream ftmp;
        ftmp.open("tmp.dot",ios::out);
        buff.vFA.print(ftmp);
        #endif
        if (trans){
                operandStack.push(NFA_Cluster::getTrans(buff,RE[i]));
                trans = 0;
                ++i;
                continue;
        }
        switch(RE[i]){
            case ')': 
                while(!operatorStack.empty()&&(char)operatorStack.top() != '('){
                    cal(buff,operandStack,operatorStack.top());
                    operatorStack.pop();
                }
                if (operatorStack.empty()){
                    throw invalid_argument("missing '(");
                }
                operatorStack.pop();
                ++i;
                break;
            case '\"':
                for (j= i + 1;RE[j]!='\"'&&j<RE.size();++j);
                if (j == RE.size()){
                    stringstream s;
                    s<<"\" didn't match in "<<RE;
                    throw invalid_argument(s.str());
                }
                operandStack.push(NFA_Cluster(buff,RE,i+1,j-1));
                i = j + 1;
                break;
            case '[': //todo
                for (j = i + 1; RE[j] != ']';++j);
                if (j == i + 1){
                    i = j + 1;
                    continue;
                }
                operandStack.push(NFA_Cluster::getBracket(buff,RE,i+1,j-1));
                i = j + 1;
                break;
            case '{': 
                //throw invalid_argument("not supported yet");
                {
                    string tmp;
                    ++i;
                    while(RE.size()>i && RE[i] != '}'){
                        tmp += RE[i];
                        ++i;
                    }
                    try {
                        if (i >= RE.size()){
                            string warning("'}' expected");
                            throw invalid_argument(warning.c_str());
                        }
                        ++i;
                        int l,r;
                        sscanf(tmp.c_str(),"%d,%d",&l,&r);
                        newOp = RE_operator('{',l,r);
                    } catch(invalid_argument e){
                        string warning(e.what());
                            warning += "in RE:";
                            warning += RE;
                            throw invalid_argument(warning);
                    };
                    if (!operatorStack.empty())
                    while (!operatorStack.empty()&&RE::newPri[newOp] < RE::oldPri[operatorStack.top()]){
                        NFA_Cluster::cal(buff,operandStack,operatorStack.top());
                        operatorStack.pop();
                    } 
                    operatorStack.push(newOp);
                    break;
                }
            case '(':
            case '|': 
            case '*':
            case '?':
            case '+': 
            case '^': 
                newOp = RE_operator(RE[i]);
                if (!operatorStack.empty())
                while (!operatorStack.empty()&&RE::newPri[newOp] < RE::oldPri[operatorStack.top()]){
                    NFA_Cluster::cal(buff,operandStack,operatorStack.top());
                    operatorStack.pop();
                } 
                operatorStack.push(RE[i]);
                ++i;
                break;
            case '$':
                while(!operatorStack.empty()){
                    #ifdef DEBUG
                    stringstream s;
                    s<<"ready to cal ";//<<operatorStack.top().op<<endl;
                    buff.logger.customMSG(s.str());
                    buff.logger.save();
                    #endif
                    cal(buff,operandStack,operatorStack.top());
                    operatorStack.pop();
                }
                ++i;
                break;
            case '\\':
                ++i;
                trans = 1;
                break;
            default: 
                operandStack.push(NFA_Cluster(buff,RE[i]));
                #ifdef DEBUG
                {
                stringstream s;
                s<<"add "<<RE[i]<<endl;
                buff.logger.customMSG(s.str());
                }
                #endif
                ++i;
                break;
        }
    }
    if (operandStack.size() != 1){
        stringstream s;
        s <<"expeceted 1 operand left but found"<<operandStack.size()<< "operands";
        s <<endl<<" the top is "<<operandStack.top().head<<" "<<operandStack.top().head<<endl;
        throw invalid_argument(s.str());
    }
    NFA_Cluster rt = operandStack.top();
    return rt;
}

NFA_Cluster NFA_Cluster::RE2NFA(string RE,NFA &buff,action _action){
    NFA_Cluster &&head = NFA_Cluster::RE2NFA_Cluster(RE[0]=='^'?RE.substr(1,RE.size()-(RE[RE.size()-1]=='$'?2:1)):RE,buff);
    int nhead = buff.add(),ntail = buff.add();
    {
        WrLock(buff.Wrlock);
        buff[nhead].addTrans(head.head,'\n');
    }
    if (RE[0] != '^'){
        WrLock(buff.Wrlock);
        buff[nhead].addTrans(head.head,eps);
    }            
    head.head = nhead;
    {
        WrLock(buff.Wrlock);
        buff[ntail].setAction(_action);
        buff[head.tail].addTrans(ntail,'\n');
    }
    if (RE[RE.size()-1] != '$'){
        WrLock(buff.Wrlock);
        buff[head.tail].addTrans(ntail,eps);
    }  
    head.tail = ntail;  
    #ifdef DEBUG
    stringstream s;
    s<<"finish with head: "<<head.head<<" and tail: "<<head.tail<<endl;
    buff.logger.customMSG(s.str());
       // system("pause");
    #endif
    return head;
}


void NFA_Cluster::cal(NFA &buff,stack<NFA_Cluster> &operandStack,RE_operator op){
NFA_Cluster operand1 = operandStack.top();
operandStack.pop();  
NFA_Cluster operand2(0,0);
if (!operandStack.empty()){
    operand2 = operandStack.top();
}{
stringstream s;
#ifdef DEBUG
    s<<"in cal"<<" "<<op.op;
    buff.logger.customMSG(s.str());
    buff.logger.save();
#endif
}
stringstream s;
    switch(op.op){
        case '|':
        case '^':
            operandStack.pop();      
            {  
            NFA_Cluster &&tmp = NFA_Cluster(buff,op,operand2,operand1);
            #ifdef DEBUG
            s<<"get "<<tmp.head<<" "<<tmp.tail<<" for ["<<operand2.head<<","<<operand2.tail<<"]"<<op.op<<" ["<<operand1.head<<","<<operand1.tail<<"]"<<endl;
            buff.logger.customMSG(s.str());
            buff.logger.save();
            #endif
            operandStack.push(tmp);
            }
            break;
        case '?':
        case '+':
        case '*':

            {  
                NFA_Cluster &&tmp = NFA_Cluster(buff,op,operand1);
                operandStack.push(tmp);
                #ifdef DEBUG
                s<<"get "<<tmp.head<<" "<<tmp.tail<<" for ["<<operand1.head<<","<<operand1.tail<<"]"<<op.op<<endl;
                buff.logger.customMSG(s.str());
                buff.logger.save();
                #endif
            }

            break;
        case '{':
            //throw invalid_argument("{ not supported yet");
            {
                NFA_Cluster &&tmp = NFA_Cluster(buff,op.l,op.r,operand1);
                operandStack.push(tmp);
                #ifdef DEBUG_OPBRACE
                s<<"get "<<tmp.head<<" "<<tmp.tail<<" for ["<<operand1.head<<","<<operand1.tail<<"]"<<op.op<<endl;
                buff.logger.customMSG(s.str());
                buff.logger.save();
                #endif
            }
            break;
        default:
            stringstream s;
            s << "unknown operator occurs" << op.op<<" founded";
            throw invalid_argument(s.str());
    };

}



/*
. 匹配任何单个字符，除\n.                       

- 表示匹配范围，如：a-z，表示匹配a-z之间的任何字符

* 匹配前面表达式的零个或多个拷贝。

[] 匹配括号内的任意字符的字符类，第一个符号是"^"，表示匹配除括号中的字符以外的任意字符。

^ 作为正则表达式的第一个字符，匹配行的开头。   例：^ab.,表示以ab开头的字符串.

$ 作为正则表达式的最后一字符，匹配行的结尾。


\ 用于转义字符                                                                      OK

+ 匹配前面表达式一次或多次出现。                                                      OK

? 匹配前面表达式零次或1次出现。                                                       OK

| 匹配前面表达式或随后表达式                                                          OK

"" 引号中的每个字符解释为字面意思


{} 指示一个模式可能出现的次数


*/
