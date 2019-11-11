//
// Created by mylord on 2019/10/31.
//
#include "distribution_server.h"

DistributionServer::DistributionServer(int port, int size) : server(port, size), client_node(), isthreaded(false) {
    this->server.Listen();
}


int DistributionServer::getListenFd() {
    return this->server.getListenFd();
}

ClientNode DistributionServer::getClientNode(int fd) const {
    auto client_it = find_if(client_node.begin(), client_node.end(),
                             [fd](const ClientNode &cli){ return cli.client_info.client_fd == fd; });
    return *client_it;
}

int DistributionServer::AcceptConnection(ClientNode & client) {
    int fd = server.AcceptConnection(client.client_info);

    client.client_state = WAITING;

    client_node.push_back(client);

    if(isthreaded == false) {
        thread heartbeats_t(this->Heartbeats_t, this);
        heartbeats_t.detach();
    }

    return fd;
}

void DistributionServer::setState(int sock_fd, ClientState state) {
    auto client_it = find_if(client_node.begin(), client_node.end(),
                             [sock_fd](const ClientNode &cli){ return cli.client_info.client_fd == sock_fd; });
    client_it->client_state = state;
}

int DistributionServer::Write(int sock_fd, char buff[]) {
    return server.Write(sock_fd, buff);
}

int DistributionServer::Read(int sock_fd, char buff[]) {
    return server.Read(sock_fd, buff);
}

int DistributionServer::Close(int sock_fd) {
    auto client_it = find_if(client_node.begin(), client_node.end(),
            [sock_fd](const ClientNode &cli){ return cli.client_info.client_fd == sock_fd; });

    client_node.erase(client_it);

    close(sock_fd);
}

void DistributionServer::getAllClientInfo(string & buff) {
    for(auto a : client_node) {
        buff += a.client_info.client_ip;
        buff += '\t';
        buff += a.client_state == WAITING ? "WAITING" : "TRANSMITING";
        buff += '\n';
    }
}

vector<ClientNode> DistributionServer::getClientNode() const {
    return this->client_node;
}

void DistributionServer::SendHeartbeats(DistributionServer * server) {
    //如果有连接，就每隔三秒发送一次心跳包。
    while(!server->getClientNode().empty()) {
        for (auto a : server->getClientNode()) {
            server->Write(a.client_info.client_fd, "HeartBeats");
        }
        sleep(3);
    }
}

void DistributionServer::Heartbeats_t(DistributionServer * server) {
    server->SendHeartbeats(server);
}