//
// Created by mylord on 2019/10/31.
//

#include "file_parsing.h"

FileParsing::FileParsing(char * ip, int port) : parsing_client(ip, port){
    ws = new WatingState();
    ps = new ParsingState();
}

void FileParsing::Init() {
    SpecificTime st;
    while (parsing_client.Connect() == -1) {
        sleep(1);
    }

    cout << "[" << st.getTime().c_str() << " Connected]:\tconnected to task distributer server at port 6666" << endl;

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

    SpecificTime st;
    cout << "[" << st.getTime().c_str() << " Waiting Tasks]:\tWating for tasks from task distributer server!" << endl;

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

    SpecificTime st;
    cout << "[" << st.getTime().c_str() << " Started Parsing]:\tConnected to workstation and start parsing!" << endl;

    //对文件进行解析


    fp->getClient().Write("ParsingSuccess");

    cout << "[" << st.getTime().c_str() << " Finished Parsing]:\tFinished parsing and reported to task distributer server!" << endl;


    fp->setState(fp->getWatingState());
}
