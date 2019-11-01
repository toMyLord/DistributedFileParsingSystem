//
// Created by mylord on 2019/10/31.
//

#include "../src/server.h"
#include "../src/client.h"

int main() {
    Client distributer_client("127.0.0.1", 5555);
    Server parsing_server(8888, 1);

    distributer_client.Connect();
    parsing_server.Listen();

    while(true) {
        ClientInfo parsing_info;

        //获取发文件的前提信息
        char buffer[MAX_BUFFER_SIZE];
        gets(buffer);

        distributer_client.Write("ParsingRequest");
        int fd = parsing_server.AcceptConnection(parsing_info);

        //文件发送过程

        parsing_server.Close(fd);
    }
}