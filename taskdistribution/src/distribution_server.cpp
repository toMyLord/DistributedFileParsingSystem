//
// Created by mylord on 2019/10/31.
//
#include "distribution_server.h"

DistributionServer::DistributionServer(int port, int size) : server(port, size), client_node() {
    this->server.Listen();
}


int DistributionServer::getListenFd() {
    return this->server.getListenFd();
}

int DistributionServer::AcceptConnection(ClientNode & client) {
    int fd = server.AcceptConnection(client.client_info);

    client.client_state = WAITING;

    client_node.push_back(client);

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
    string ip;
    auto client_it = find_if(client_node.begin(), client_node.end(),
            [sock_fd](const ClientNode &cli){ return cli.client_info.client_fd == sock_fd; });
    ip = client_it->client_info.client_ip;
    client_node.erase(client_it);

    close(sock_fd);
}

void DistributionServer::getAllClientInfo(string & buff) {
    for(auto a : client_node) {
        buff += a.client_info.client_ip;
        buff += '\t';
        buff += a.client_state == WAITING ? "WAITING" : "TRANSIMITING";
        buff += '\n';
    }
}