//
// Created by mylord on 2019/10/31.
//

#include "file_parsing.h"

FileParsing::FileParsing(char * ip, int port) : parsing_client(ip, port){
    ws = new WatingState();
    ps = new ParsingState();

    parsing_client.Connect();
    time_t curtime;
    time(&curtime);
    cout << "[Connected]:" << ctime(&curtime) << "\tconnected to " << ip << " port " << port << endl;

    setState(ws);
}

inline void FileParsing::setState(State * s) {
    state = s;
}

void FileParsing::Handler() {
    state->Handler(this);
}

WatingState * FileParsing::getWatingState() {
    return this->ws;
}

ParsingState * FileParsing::getParsingState() {
    return this->ps;
}

Client & FileParsing::getClient() {
    return this->parsing_client;
}

FileParsing::~FileParsing() {
    delete ws;
    delete ps;

    parsing_client.Close();
}

void WatingState::Handler(FileParsing * fp) {
    char buffer[MAX_BUFFER_SIZE];

    if(fp->getClient().Read(buffer) <= 0) {
        fp->getClient().Close();
        return;
    }

    strncpy(fp->workstation_ip, buffer, 16);
    fp->workstation_port = 8888;

    fp->setState(fp->getParsingState());
}



void ParsingState::Handler(FileParsing * fp) {
    Client workstation_client(fp->workstation_ip, fp->workstation_port);

    workstation_client.Connect();

    //对文件进行解析

    fp->getClient().Write("ParsingSuccess");
    fp->setState(fp->getWatingState());
}
