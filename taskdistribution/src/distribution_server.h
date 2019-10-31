//
// Created by mylord on 2019/10/31.
//

#ifndef DISTRIBUTEDFILEPARSING_DISTRIBUTION_SERVER_H
#define DISTRIBUTEDFILEPARSING_DISTRIBUTION_SERVER_H

#include "../../src/server.h"
#include <vector>

class DistributionServer {
private:
    Server server;

    //用来存储所有连接至任务分配服务器的工作站
    vector<ClientInfo> client_info;

public:
    DistributionServer(int port, int size);

    int getListenFd();

    int AcceptConnection();

    int Write(int sock_fd, char buff[]);

    int Read(int sock_fd, char buff[]);

    int Close(int sock_fd);
};

#endif //DISTRIBUTEDFILEPARSING_DISTRIBUTION_SERVER_H
