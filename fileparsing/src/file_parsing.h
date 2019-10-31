//
// Created by mylord on 2019/10/31.
//

#ifndef DISTRIBUTEDFILEPARSING_FILE_PARSING_H
#define DISTRIBUTEDFILEPARSING_FILE_PARSING_H

#include "parsing_state.h"
#include "wating_state.h"

//文件解析类，使用状态模式，在解析文件状态和等待连接状态中切换
class FileParsing {
private:
    //用来接收派生类的指针
    State * state;

public:

};


#endif //DISTRIBUTEDFILEPARSING_FILE_PARSING_H
