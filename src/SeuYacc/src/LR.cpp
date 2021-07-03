#include"LR.h"

// 定义acc
#define acc StrToInt.size()+1

LR::LR(){
    tail = 0;
}
LR::LR(string s){
    logger = Logger(s);
    logger.init();
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
    concentricItem.insert({node.core(),node.idx});
    allKeys.insert(node.core());
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
        LR_Node nowNode = pool[now];
        q.pop();
        #ifdef DEBUG
        {
            stringstream s;
            s<<"trying node "<<now<<endl;
            logger.customMSG(s.str());
            cout<<s.str()<<endl;
        }
        #endif
        //nowNode.
        for (auto next:nowNode.allNexts){
            #ifdef DEBUG
            {
                stringstream s;
                s<<"trying node "<<now<<" possible trans "<<next<<" "<<I2S(next)<<" which has "<<nowNode.nextPros.count(next)<<" producers"<<endl;
                logger.customMSG(s.str());
                cout<<s.str()<<endl;
            }
            #endif
            LR_Node tmp;
            auto iterPair = nowNode.nextPros.equal_range(next);
            #ifdef DEBUG
            if (iterPair.first == iterPair.second){
                cout<<"indeed none"<<endl;
                continue;
            }
            #endif
            for (auto iter = iterPair.first; iter != iterPair.second; ++iter){

                {
                    #ifdef DEBUG
                    stringstream s;
                    s<<"trying node "<<now<<" possible producer "<<I2S(getLeft(nowNode.producers[iter->second]))<<"->";
                    for (auto &ss:getRight(nowNode.producers[iter->second])){
                        s<<I2S(ss)<<" ";
                    }
                    s<<endl;
                    logger.customMSG(s.str());
                    cout<<s.str()<<endl;
                    #endif
                }
                if (!nowNode.producers[iter->second].isEnd()){
                    tmp.addProducer(nowNode.producers[iter->second].move());
                }
            }
            if (vis.count(tmp.getHash())){
                #ifdef DEBUG
                cout<<"already exist!"<<endl;
                #endif
                newId = vis[tmp.getHash()];
            } else {
                eclosureHash oldHash = tmp.getHash();
                tmp.solveEclosure();
                if (wholeLRNode.count(tmp.getHash())){
                    #ifdef DEBUG
                    cout<<"whole already exist!"<<endl;
                    #endif
                    newId = wholeLRNode[tmp.getHash()];
                } else {
                    newId = add(tmp);
                    #ifdef DEBUG
                    cout<<"new eclosure "<<newId<<endl;
                    #endif
                    q.push(newId);
                    wholeLRNode[tmp.getHash()] = newId;
                }
                vis[oldHash] = newId;
            }
            #ifdef DEBUG
                {
                    stringstream s;
                    s<<"adding edge"<<now<<" to "<<newId<<" with "<<I2S(next)<<endl;
                    cout<<s.str();
                    logger.customMSG(s.str());
                }
            #endif
            addTrans(now,newId,next);
        }
    }
    stringstream s;
    #ifdef DEUBG
    s<<"totoal "<<pool.size()<<"lr1 nodes";
    #endif
    logger.customMSG(s.str());
    logger.save();
    cout<<s.str()<<endl;
    return  pool.size();
}
    void LR::printVisualLR(fstream &fout){
        visualLR vLR;
        vLR.print(fout,*this);
    }

auto LR::getConcentricBegin(eclosureHash concentric){
    auto pairIter = concentricItem.equal_range(concentric);
    return pairIter.first;
}

auto LR::getConcentricEnd(eclosureHash concentric){
    auto pairIter = concentricItem.equal_range(concentric);
    return pairIter.second;
}

LR LR::consturctLALR(){
    LR rt;
    rt.logger = logger;
    // 存储所有的映射状态
    int state_num = pool.size();
    int *temp = new int[state_num];

    // 存储原LR(1)节点id->生成的LALR节点下标id的映射
    // 初始值为状态x->-1
    // 合并同心项后所有同心项映射到同一项
    // 用于最后更改LALR中的每个node的Trans
    for(int i = 0; i < state_num; i++){
        temp[i] = -1;
    }

    // 初始节点node需要保留
    LR_Node node0 = pool[0];
    temp[0] = 0;
    rt.add(node0);

    // count对应当前LALR项目集簇中欲加入的一项的下标
    int count = 1;

    //eclosureHash coreHash = pool[i].core();
    //for (auto iter = getConcentricBegin(coreHash);iter!=end;++iter)

    // 遍历所有同心项的hash桶(所有同心项的hash值相同)
    for (auto & coreHash : allKeys){
        // 该node没有同心项，即桶中元素个数为1
        if(getConcentricBegin(coreHash) == getConcentricEnd(coreHash)){
            // 初始节点已处理过，直接跳过就行（初始节点应该没有同心项
            if(getConcentricBegin(coreHash)->second == 0){
                continue;
            }

            // 直接将该节点加入LALR项目集簇中，并改变映射temp
            LR_Node node = pool[getConcentricBegin(coreHash)->second];
            rt.add(node);
            temp[getConcentricBegin(coreHash)->second] = count;
            count++;
            continue;
        }

        // 存在同心项的情况
        // 先取第一项
        LR_Node node = pool[getConcentricBegin(coreHash)->second];
        temp[getConcentricBegin(coreHash)->second] = count;
        // 合并同心项
        for (auto  iter = getConcentricBegin(coreHash); iter != getConcentricEnd(coreHash); ++iter){
            if(iter == getConcentricBegin(coreHash)){
                continue;
            }
            // 遍历每个同心项的产生式，对合并后的节点Insert每一个产生式(效果相当于merge)
            for(auto  iter2 = pool[iter->second].producerCBegin(); iter2 != pool[iter->second].producerCEnd(); ++iter2){
                node.addProducer(iter2->second);
            }
            temp[iter->second] = count;
        }
        rt.add(node);
        count++;
    }

    // 使用temp表更新LALR每个节点的Trans
    // 生成遍历LALR中每个节点的迭代器
    for(int i = 0; i < rt.pool.size(); i++){
        for(auto iter = rt[i].state.begin(); iter != rt[i].state.end(); ++iter){
            iter->second = temp[iter->second];
        }
    }

    delete []temp;

    return rt;
}

int LR::constructParsingTable(){
    // 首先生成y.tab.h，其中存储了终结符与int之间的映射
    ofstream out;
    cout<<"begin constructParsing"<<endl;
	out.open("y.tab.h", ios::out);
	out << "#ifndef Y_TAB_H" << endl;
	out << "#define Y_TAB_H" << endl;

    out << endl;

    out << "// epsilon" << endl;
    out << "#define " << "" <<"epsilon -1" << endl;
    out <<"#define "<<"TNBound "<<TNBound<<endl;
    out << endl;

    out << "// terminals" << endl;
    for (int i = 128; i < TNBound; i++)
		out << "#define " << IntToStr[i] <<" "<< i << endl;

    out << endl;

    out << "// nonterminals" << endl;
    for (int i = TNBound; i < NLBound; i++)
		out << "#define " << IntToStr[i] <<" "<< i << endl;
    out <<"#define acc "<<acc<<endl;
    out << endl;
    out <<"#define NTerminalBase "<<TNBound<<endl;
    out <<"#define isTerminal(x) (x<"<<TNBound<<")"<<endl;

    out<<"char *I2S[]={"<<endl;
    for (int i = 0 ; i < NLBound;++i){
        if (i<128){
            if (i<30){
                out<<"\"\\\\"<<i<<"\","<<endl;
            } else{ 
                if (i =='\\'){
                    out<<"\"\\\\\","<<endl;
                } else if (i == '\"') {
                    out<<"\"\\\"\","<<endl;
                } else {
                    out<<"\""<<(char)i<<"\","<<endl;
                }
            }
            continue;
        }
        if (i!=NLBound - 1)
            out<<"\""<<I2S(i)<<"\","<<endl;
        else 
            out<<"\""<<I2S(i)<<"\"};"<<endl;
    }
    out<<"int proCnt["<<TranslationRule_Int.size()<<"]={"<<endl;
    for (int i = 0; i < TranslationRule_Int.size();++i){
        out<<TranslationRule_Int[i].second.size()<<(i!=TranslationRule_Int.size()-1?",":"};");
    }
    out<<endl;
    out<<"int proGet["<<TranslationRule_Int.size()<<"]={"<<endl;
    for (int i = 0; i < TranslationRule_Int.size();++i){
        out<<TranslationRule_Int[i].first<<(i!=TranslationRule_Int.size()-1?",":"};");
    }
    out<<endl;
    //out <<"char (*)["<<IntToStr.si






    // ---------全局变量部分---------
    out << endl;
    int state_num = this->pool.size();
    int nonterminal_num = NLBound - TNBound;
    // 存储每一个状态的GOTO子表(一维数组int[非终结符个数])
    {
    int *temp = new int[nonterminal_num];
    // 生成并输出GOTO表，GOTO表的结构为int[状态个数][非终结符个数]
    out << "int _GOTO[" << state_num << "][" << nonterminal_num << "] = {"<< endl;
    for(int i = 0; i < state_num; i++){
        // 初始化每一项均为-1(出错)
        memset(temp,-1,sizeof(int)*nonterminal_num);
        // 根据项目集簇生成GOTO表
        for(auto iter = this->pool[i].getAllTransBegin(); iter != this->pool[i].getAllTransEnd(); ++iter){
            // 确认其为非终结符
            if(iter->first < NLBound && iter->first >= TNBound)
                temp[iter->first - TNBound] = iter->second;
        }
        // 将temp写入GOTO表
        if (i < state_num - 1){
            for(int j = 0; j < nonterminal_num ; j++)
                out << temp[j] <<",";
            out<<endl;
        } else {
            for(int j = 0; j < nonterminal_num - 1; j++)
                out << temp[j] <<",";    
            out<<temp[nonterminal_num - 1]<<"};"<<endl;;
        }
    }
    delete []temp;
    }
    // 生成并输出ACTION表，ACTION表的结构为int[状态个数][终结符个数]
    // 约定0为出错，+x代表移进到第x-1个状态，-x代表使用第x-1号产生式规约，+x其中x=StrToInt.size()+1代表acc
    int terminal_num = TNBound;
    out << "int ACTION[" << state_num << "][" << terminal_num << "] = {"<< endl;
    // 存储每一个状态的ACTION子表(一维数组int[终结符个数])
    int *temp = new int[terminal_num];
    for(int i = 0; i < state_num; i++){
        // 初始化每一项均为0(出错)
        memset(temp,0,sizeof(int)*terminal_num);
        
        // 根据项目集簇生成ACTION表
        // 规约项
        set<LR_Producer> pros = this->pool[i].getAllReduce();
        for(auto & pro : pros){
            for(auto & num : pro.lookAhead){
                // acc情况，即对向前看#符号进行产生式号为0(start->translation_unit)的规约
                if (pro.nowPlace!=getRight(pro.producer).size()){
                    throw invalid_argument("ERROR NOT REDUCEBAL when acc");
                    return 1;
                }
                if(pro.producer == 0 && IntToStr[num] == "#"){
                    temp[num] = acc;
                }else{
                    if (temp[num] != 0){
                        stringstream ss;
                        ss << "[WARNING] Reduce - Reduce conflict detected!\n" <<endl;
                        ss << "Reduce:"<<I2S(getLeft(pro.producer))<<" -> ";
                        for (auto &id:getRight(pro.producer)){
                            ss << I2S(id)<<" ";
                        }
                        ss <<endl;
                        ss << "Reduce:"<<I2S(getLeft(-1-temp[num]))<<" -> ";
                        for (auto &id:getRight(-1-temp[num])){
                            ss << I2S(id)<<" ";
                        }
                        ss <<endl;
                        ss << " Handle with default disambiguating rule : earlier producer"<<endl;
                        logger.error("generating code in (LA)LR",ss.str(),0);
                        temp[num] = max (temp[num],-(pro.producer+1));
                    }
                    temp[num] = -(pro.producer + 1);
                }
            }
        }
        // 移进项
        for(auto iter = this->pool[i].getAllTransBegin(); iter != this->pool[i].getAllTransEnd(); ++iter){
            // 确认其为非终结符
            if (iter->first >= TNBound){
                continue;
            }
            if(iter->first < TNBound ){
                // 出现移进规约冲突，报错
                if(temp[iter->first] != 0){
                    stringstream ss;
                    ss << "[WARNING] Reduce - shift conflict detected!\n" <<endl;
                    ss << "Shift:"<<I2S(iter->first)<<" to node"<<iter->second<<endl;
                    for(auto & pro : pros){
                        for(auto & num : pro.lookAhead){
                                // acc情况，即对向前看#符号进行产生式号为0(start->translation_unit)的规约
                                if (num == iter->first){
                                    ss<<"Reduce:";
                                    ss<<I2S(getLeft(pro.producer))<<" -> ";
                                    for (auto id:getRight(pro.producer)){
                                        ss<<I2S(id)<<" ";
                                    }
                                    ss<<endl;
                                }
                            }
                    }
                    ss << " Handle with default disambiguating rule : Shift"<<endl;
                    logger.error("generating code in (LA)LR",ss.str(),0);
                }
                temp[iter->first] = iter->second + 1;
            }
        }
        // 将temp写入GOTO表
        if (i < state_num - 1){
            for(int j = 0; j < terminal_num ; j++)
                out << temp[j] <<",";
            out<<endl;
        } else {
            for(int j = 0; j < terminal_num - 1; j++)
                out << temp[j] <<",";    
            out<<temp[terminal_num-1]<<"};"<<endl;;
        }

    }
    delete []temp;
	out << "#endif" << endl;
	out.close();

    // 其次生成y.tab.cpp，其中存储SeuYacc的输出结果
    out.open("y.tab.c", ios::out);
    // ----------头文件部分----------
	out << "#include \"y.tab.h\"" << endl;
    out << "#include \"lex.yy.c\"" << endl;


    out << R"(
// The following are from the yacc file
#include "y.tab.h"
#include "lex.yy.c"
int vTail = 0;
char outputBuff1[5000],outputBuff2[5000];
const char* outputName =NULL;
const char* inputName = NULL;
int debugFlag = 0;
typedef struct _visualToken{
    int id;
    int value; 

}visualToken;

void translateProducer(char in[]){
int i;
char tmp[2] ={'\0','\0'};
	outputBuff1[0]='\0';
	for (i = 0 ; i < strlen(in); ++i){
		switch (in[i]){
			case '<':
				strcat(outputBuff1,"&lt;");
				break;
			case '>':
				strcat(outputBuff1,"&gt;");
				break;
			case '&':
				strcat(outputBuff1,"&amp;");
				break;
			case '\"':
				strcat(outputBuff1,"&quot;");
				break;
			case ' ':
				strcat(outputBuff1,"&emsp;");
				break;
			default:
				tmp[0] = in[i];
				strcat(outputBuff1,tmp);
		}
	} 
}
void translateText(){
int i;
char tmp[2] ={'\0','\0'};
	outputBuff2[0]='\0';
	for (i = 0 ; i < yyleng; ++i){
		switch (yytext[i]){
			case '<':
				strcat(outputBuff2,"&lt;");
				break;
			case '>':
				strcat(outputBuff2,"&gt;");
				break;
			case '&':
				strcat(outputBuff2,"&amp;");
				break;
			case '\"':
				strcat(outputBuff2,"&quot;");
				break;
			case ' ':
				strcat(outputBuff2,"&emsp;");
				break;
			default:
				tmp[0] = yytext[i];
				strcat(outputBuff2,tmp);
		}
	} 
}


int addItem(int c){
	if (c =='#'){
		return 0;
	}
	translateProducer(I2S[c]);
	translateText();
	fprintf(yyout,"\"%d\"[ shape = \"rect\" label =<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\"><tr><td bgcolor=\"grey\"  align = \"center\">Token:%s</td></tr><tr><td align=\"left\">Value:%s</td></tr></table>>];\n",vTail,outputBuff1,outputBuff2); 
	++vTail;
	return 0;
	/*fprintf(yyout,"%d:\"%s\"\n",vTail++,yytext);*/
}
int addEdge(int a,int b){
	fprintf(yyout,"\"%d\" -- \"%d\"\n",a,b);
	return 0;
}
int addRItem(visualToken token){
	translateProducer(I2S[token.value]);
	if (strcmp(I2S[token.value],"translation_unit")==0){
		fprintf(yyout,"\"%d\"[ shape = \"rect\" label =<<table border=\"0\" cellborder=\"5\" cellspacing=\"0\" cellpadding=\"4\"><tr><td bgcolor=\"black\" align = \"center\"><font color = \"white\">Symbol:%s</font></td></tr></table>>];\n",token.id,outputBuff1);	
	} else {
		fprintf(yyout,"\"%d\"[ shape = \"rect\" label =<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\"><tr><td>Symbol:%s</td></tr></table>>];\n",token.id,outputBuff1);	
	}
	++vTail;
}


int valTop = -1;
int sTop = -1;
int sSt[10000];
visualToken valSt[10000];

void yyparse(){
int c ;
int now;
visualToken token,newtoken;
	sSt[++sTop] = 0;
	do{
		c = yyLex();
	}while(c==' ');
	/*printf("get %s for %s\n",I2S[c],yytext);*/
	token.value = c;
	while(1){
		now = sSt[sTop];
		if (debugFlag)
			printf("now at %d\n",now);
		if (ACTION[now][c]==acc){
			if (debugFlag)
				printf("success! \n");
			return ;
		} 
		if (ACTION[now][c]>0){
			token.id = vTail;
			addItem(c);
			valSt[++valTop] = token;
		/*	addItemEdge(token);*/
			if (debugFlag)
				printf("moving for %s \n",I2S[c]);
			sSt[++sTop]=ACTION[now][c]-1;
			if (debugFlag){
				printf("move to %d\n",ACTION[now][c]-1);
				printf("move ok\n");
			}	
			if (yyEOF == 2){
				break; 
			}
			do{
				c = yyLex();
			}while(c==' ');
			token.value = c;
			/*printf("get %s for %s\n",I2S[c],yytext);*/
		} else if (ACTION[now][c]<0) {
			int oldNow = now;
			int newSymbol;
			if (debugFlag){
				printf("reducing\n");
				printf("reduce get %s \n",I2S[proGet[(-ACTION[oldNow][c])-1]]);
			}
			sTop-=proCnt[(-ACTION[oldNow][c])-1];
			newSymbol = proGet[(-ACTION[oldNow][c])-1];
			newtoken.value = newSymbol;
			newtoken.id = vTail;
			addRItem(newtoken);
			for (int i = 0 ; i < proCnt[(-ACTION[oldNow][c])-1];++i){
				/*addEdge(newtoken,valSt[valTop]);*/
				addEdge(newtoken.id,valSt[valTop].id);
				--valTop;
			}
			valSt[++valTop]=newtoken;
			now = sSt[sTop];
			sSt[++sTop] = _GOTO[now][newSymbol-TNBound]; 
		} else {
			if (debugFlag)
				printf("ERROR!!!!\n");
			return;
		}
	}
	if (debugFlag)
		printf("meet end of file!\n");
	return ;
}

void invalidInput(){
	printf("expected input format: \n");
	printf("-debug: ouput debug message to log \n");
	printf("-o fileName[without extension]:	syntax tree output file name,default is the same with inputFile \n");
	printf("-f fileName[with extension]: input file Name\n");
	system("pause");
}

int main(int argc, char const *argv[]){
char temp[100];
	for (int i = 1; i < argc; ++i){
		if (argv[i][0] != '-'){
			printf("expected input flag start with \'-\' but get :%s\n ",argv[i]);
			invalidInput();
			return 1;
		}
		if (strcmp(argv[i],"-debug")==0){
			debugFlag = 1;
			continue;
		}
		if (strcmp(argv[i],"-o")==0){
			if (i < argc){
				outputName = argv[++i];
			}
			continue;
		}
		if (strcmp(argv[i],"-f")==0){
			if (i < argc){
				inputName = argv[++i];
			}
			continue;
		}	
		printf("unexpected argument %s \n",argv[i]);
		invalidInput();
	}
	if (inputName == NULL){
		printf("you must set the input fileName\n!");
		invalidInput();
	}
	if (outputName == NULL){
		outputName = inputName;
	}
	if (debugFlag){
		freopen("debug.log","w",stdout);
	}
	yyin = fopen(inputName,"r");
	if (yyin == NULL){
		fprintf(stderr,"file  %s open failed!!\n",inputName);
	}
	sprintf(temp,"%s.dot",outputName);
	yyout = fopen(temp,"w");
	if (yyin == NULL){
		fprintf(stderr,"file  %s open failed!!\n",inputName);
		return 0;
	}
	if (yyout == NULL){
		fprintf(stderr,"file  %s open failed!!\n",temp);		
		return 0;
	}
	fprintf(yyout,"graph{\n");
	fprintf(stderr,"yacc parsing begin!\n");
	yyparse();
	fprintf(yyout,"}\n");
	fprintf(stderr,"yacc parsing finished!\n");
	fclose(yyout);
	sprintf(temp,"dot %s.dot -Tsvg -o %s.svg",outputName,outputName);
	system(temp);
	sprintf(temp,"dot %s.dot -Tpng -o %s.png",outputName,outputName);
	system(temp);
	if (debugFlag){
		fclose(stdout);
	}
	if (debugFlag){
		fprintf(stderr,"press any key to finished...\n");	
	}
	system("pause");
}
    
    )";

    // 构建成功
    return 0;
}
