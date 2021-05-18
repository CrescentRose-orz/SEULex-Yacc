#include"Logger.h"

/*
    flag 
        -1: log file open error;
        0: open successful not init,
        1: init successful,
        2: closed;
*/
Logger::Logger(){
    Logger(string("default.log"));
}

Logger::Logger(string fileName){
    if ((_log = fopen(fileName.c_str(),"a")) == NULL){
        cerr<<"LOG FIlE OPEN error!/n"<<endl;
        flag = -1;
    }
    this -> fileName = fileName;
    flag = 0;
}

int Logger::init(){
    if (flag == -1){
        cerr<<"LOG init failed!/N"<<endl;
    }
    _beginTime = time(NULL);
    flag = 1; 
    customMSG("Logger has been initialized.");
    cout<<"ok!"<<endl;
    return 0;
}

bool Logger::printTime(){
    if (flag!=1){
        cerr<<"LOGGER hasn't been initialized yet !\n"<<endl;
        return 0;
    }
    time_t _tmp = time(NULL);
    fprintf(_log,"<UTC:   %s >                  ", asctime(gmtime(&_tmp)));
    return 1;
}

int Logger::start(string name){
    if (!printTime()){
        return -1;
    }
    if (_timeStamp.count(name)&&_timeStamp[name]){
        fprintf(_log,"[WARNING %s event is overwritten] ",name.c_str());
    }
    fprintf(_log," %s started.\n",name.c_str());
    _timeStamp[name] = clock();
    return 0;
}


int Logger::end(string name){
    if (!printTime()){
        return -1;
    }
    if (!_timeStamp.count(name)){
        fprintf(_log,"[ERROR %s event didn't start ] ",name.c_str());
        return 1;
    }
    fprintf(_log," %s ended ,[TIME]finished in %lf s .\n",name.c_str(),(double)(clock()-_timeStamp[name])/(double)CLOCKS_PER_SEC);
    _timeStamp[name] = 0;
    return 0;
}
/*
        usage: error type&msg ,error part, error line
*/
int Logger::error(string errorName,string partName,int line = 0){
    if (!printTime()){
        return -1;
    }
    fprintf(_log," [ERROR: %s occured at %s",errorName.c_str(),partName.c_str());
    if (line){
        fprintf(_log," at line %d ",line);
    }
    fprintf(_log,"]\n");
    save(); 
    return 0;
}

int Logger::customMSG(string MSG){
    if (!printTime()){
        return -1;
    }
    fprintf(_log, "MSG: %s\n",MSG.c_str());
    return 0;
}

int Logger::close(){
    if (flag == 1){
        fclose(_log);
        return 0;
    }
    return -1;
}

    Logger::~Logger(){
        close();
    }

int Logger::save(){
    if (flag == 1){
        fclose(_log);
        _log = fopen(fileName.c_str(),"a");
    }
    return 0;
}