//
// Created by mylord on 2019/10/31.
//

#ifndef DISTRIBUTEDFILEPARSING_DISTRIBUTION_SERVER_H
#define DISTRIBUTEDFILEPARSING_DISTRIBUTION_SERVER_H

#include "../../src/server.h"
#include <vector>

enum ClientState {WAITING, TRANSIMITING};

typedef struct {
    ClientInfo client_info;
    ClientState client_state;
}ClientNode;


class DistributionServer {
private:
    Server server;

    //用来存储所有连接至任务分配服务器的工作站
    vector<ClientNode> client_node;

public:
    DistributionServer(int port, int size);

    int getListenFd();

    int AcceptConnection(ClientNode & client);

    void setState(int sock_fd, ClientState state);

    int Write(int sock_fd, char buff[]);

    int Read(int sock_fd, char buff[]);

    int Close(int sock_fd);

    void getAllClientInfo(string & buff);
};

#endif //DISTRIBUTEDFILEPARSING_DISTRIBUTION_SERVER_H
