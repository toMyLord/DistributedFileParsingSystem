//
// Created by mylord on 2019/10/31.
//

#include "../src/server.h"
#include "../src/client.h"
#include "src/send_directory.h"
#include <thread>

Client distributer_client("127.0.0.1", 5555);
SpecificTime st;


void HeartBeatsHandler_t();

int main() {
    Server parsing_server(8888, 1);

    while(distributer_client.Connect() != 0)
        sleep(1);

    thread heartbeats_t(HeartBeatsHandler_t);
    cout << "[" << st.getTime().c_str() << " Workstation Connected]:\tWorkstation connected to distribution server!" << endl;

    heartbeats_t.detach();

    parsing_server.Listen();

    cout << "[" << st.getTime().c_str() << " Start Listening]:\tWorkstation server is listening,waiting for file parser connect!" << endl;

    while(true) {
        ClientInfo parsing_info;
        SendDirectory sd;

        //获取发文件的前提信息
        cout << "Please enter the directory you want to send:\t";
        string directory_path;
        while(true) {
            cin >> directory_path;
            if(sd.setDir(directory_path) == true)
                break;
            else
                cout << "Directory path is illegal, please enter again:\t";
        }

        distributer_client.Write("ParsingRequest");
        int fd = parsing_server.AcceptConnection(parsing_info);

        cout << "[" << st.getTime().c_str() << " Start Sending]:\tFile parser is connected and server started sending!" << endl;

        //文件发送过程
        sd.sendDir(&parsing_server, fd);

        distributer_client.Write("TransmitSuccess");

        parsing_server.Close(fd);
        cout << "[" << st.getTime().c_str() << " Finished Sending]:\tFinished sending and close workstation server" << endl;
    }
}

void HeartBeatsHandler_t() {
    char buffer[MAX_BUFFER_SIZE];

    while(true) {
        if(distributer_client.Read(buffer) == 0) {
            distributer_client.Close();
            cout << "[" << st.getTime().c_str() << " Distributer Closed]:\tDisconnected from distributer server" << endl;
            exit(0);
        }
        if (strncmp(buffer, "HeartBeats", 10) == 0) {
            //收到心跳包
//            cout << "[" << st.getTime().c_str() <<
//                " Heart Beats]:\tReceived heart beats packet from distribution server!" << endl;

            strcpy(buffer, "HeartBeats");

            distributer_client.Write(buffer);
        } else {
            //无法解析接收内容
            cout << "[" << st.getTime().c_str() <<
                 " Reply Error]:\tWorkstation server cann't parse the reply information from distribution server : " <<
                 buffer << endl;
        }
    }
}