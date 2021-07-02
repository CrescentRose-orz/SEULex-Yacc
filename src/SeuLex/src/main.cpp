#include"Lex.h"


void invalidInput(){
    cout<<"invalid input"<<endl;
    cout<<R"(
        -main 生成独立的词法分析器，默认生成与yacc的一同编译的代码
        -f fileName 指定输入文件名，否则默认为lex.l
    )"<<endl;
    system("pause");
}

int main(int argc,const char *argv[]){
string fileName;
FILE *input = NULL;
const char *file = "lex.l";
int flag = 0;
    for (int i = 0 ; i <argc; ++i){
        cout<<i<<" "<<argv[i]<<endl;
    }
    for (int i = 1; i < argc; ++i){
        if(argv[i][0]!='-'){
            cout<<"expected start with -"<<endl;
            cout<<"get "<<argv[i]<<endl;
            invalidInput();
            return 0;
        } else {
            if (strcmp(argv[i],"-main")==0){
                flag = 1;
            } else if (strcmp(argv[i],"-f")==0){
                if (i+1<= argc){
                    file = argv[++i];
                } else {
                    cout<<"fileName expected after -f"<<endl;
                    invalidInput();
                } 
            }else{
                cout<<"get "<<argv[i]<<endl;
                invalidInput();
            }
        }
    }
    cout<<"argument ok!"<<endl;
    input = fopen(file,"r");
    if (input == NULL){
        cout<<"file \""<<fileName <<"\"error"<<endl;
    }
    fclose(input);
    fileName = file;
    Lex lex(fileName,"Logger.txt");
    cout<<"begin lex"<<endl;
    lex.start(flag);
    #ifdef DEBUG
    system("pause");
    #endif
    return 0;
}