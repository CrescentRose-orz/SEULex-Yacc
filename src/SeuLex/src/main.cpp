#include"Lex.h"




int main(){
string fileName;
FILE *input = NULL;
    while (input == NULL){
        cout<<"Please enter fileName(end with .l) , or exit with EXIT"<<endl;
        #ifndef DEBUG
        cin>>fileName;
        //fileName="lex.l";
        #else
        fileName="lex.l";
        #endif
        if (fileName == "exit"){
            return 0;
        }
        input = fopen(fileName.c_str(),"r");
        if (input == NULL){
            cout<<"file \""<<fileName <<"\"error"<<endl;
        }
    }
    fclose(input);
    Lex lextest(fileName,"testLogger.txt");
    lextest.start();
    #ifdef DEBUG
    system("pause");
    #endif
    return 0;
}