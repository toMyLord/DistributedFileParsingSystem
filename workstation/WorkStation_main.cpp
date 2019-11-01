//
// Created by mylord on 2019/10/31.
//

#include "../src/server.h"
#include "../src/client.h"
#include "src/send_directory.h"

int main() {
    Client distributer_client("127.0.0.1", 5555);
    Server parsing_server(8888, 1);

    distributer_client.Connect();
    parsing_server.Listen();

    SpecificTime st;
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

        parsing_server.Close(fd);
        cout << "[" << st.getTime().c_str() << " Finished Sending]:\tFinished sending and close workstation server" << endl;
    }
}