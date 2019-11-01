//
// Created by mylord on 2019/10/31.
//

#ifndef DISTRIBUTEDFILEPARSING_FILE_PARSING_H
#define DISTRIBUTEDFILEPARSING_FILE_PARSING_H

#include "../../src/client.h"

class FileParsing;

//状态模式的基类
class State {
public:
    virtual void Handler(FileParsing * fp) = 0;
    //析构函数设置成虚函数避免派生类在析构的时候只调用基类的析构函数造成不完全析构
    virtual ~State() = default;
};

class WatingState : public State {
public:
    void Handler(FileParsing * fp);

    ~WatingState() = default;
};

//文件解析状态,在这种状态下会使用记录log的类RecordingLog
class ParsingState : public State {
public:
    void Handler(FileParsing * fp);

    ~ParsingState() = default;
};


//文件解析类，使用状态模式，在解析文件状态和等待连接状态中切换
class FileParsing {
private:
    //用来接收派生类的指针
    State * state;
    Client parsing_client;

    WatingState * ws;
    ParsingState * ps;

public:
    char workstation_ip[16];
    int workstation_port;

    FileParsing(char * ip, int port = 6666);

    void Init();

    inline void setState(State * s);

    void Handler();

    WatingState * getWatingState();

    ParsingState * getParsingState();

    Client & getClient();

    ~FileParsing();
};
#endif //DISTRIBUTEDFILEPARSING_FILE_PARSING_H
