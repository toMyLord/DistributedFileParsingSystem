//
// Created by mylord on 2019/10/31.
//

#ifndef DISTRIBUTEDFILEPARSING_DISTRIBUTION_SERVER_H
#define DISTRIBUTEDFILEPARSING_DISTRIBUTION_SERVER_H

#include "../../src/server.h"
#include <vector>
#include <thread>

enum ClientState {WAITING, TRANSMITING};

typedef struct {
    ClientInfo client_info;
    ClientState client_state;
}ClientNode;


class DistributionServer {
private:
    Server server;          //选择聚合的方式，因为这里是“根据某物实现出”的理念

    //用来存储所有连接至任务分配服务器的工作站
    vector<ClientNode> client_node;

    bool isthreaded;

public:
    DistributionServer(int port, int size);

    int getListenFd();

    ClientNode getClientNode(int fd) const;

    int AcceptConnection(ClientNode & client);

    void setState(int sock_fd, ClientState state);

    int Write(int sock_fd, char buff[]);

    int Read(int sock_fd, char buff[]);

    int Close(int sock_fd);

    vector<ClientNode> getClientNode() const;

    void getAllClientInfo(string & buff);

    void SendHeartbeats(DistributionServer * server);

    static void Heartbeats_t(DistributionServer * server);

};

#endif //DISTRIBUTEDFILEPARSING_DISTRIBUTION_SERVER_H
