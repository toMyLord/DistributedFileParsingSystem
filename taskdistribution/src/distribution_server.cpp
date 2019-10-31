//
// Created by mylord on 2019/10/31.
//
#include "distribution_server.h"

DistributionServer::DistributionServer(int port, int size) : server(port, size), client_info() {
    time_t curtime;

    this->server.Listen();

    time(&curtime);
    cout << "[Listen]:" << ctime(&curtime) << " Task distribution server start listening port " << port << endl;
}


int DistributionServer::getListenFd() {
    return this->server.getListenFd();
}

int DistributionServer::AcceptConnection() {
    ClientInfo temp;

    int fd = server.AcceptConnection(temp);

    client_info.push_back(temp);

    return fd;
}

int DistributionServer::Write(int sock_fd, char buff[]) {
    return server.Write(sock_fd, buff);
}

int DistributionServer::Read(int sock_fd, char buff[]) {
    return server.Read(sock_fd, buff);
}

int DistributionServer::Close(int sock_fd) {
    time_t curtime;
    string ip;
    auto client_it = find_if(client_info.begin(), client_info.end(),
                             [sock_fd](const ClientInfo &cli){ return cli.client_fd == sock_fd; });
    ip = client_it->client_ip;
    client_info.erase(client_it);

    close(sock_fd);

    time(&curtime);
    cout << "[Disconnected]:" << ctime(&curtime) << " " << ip << " is disconnected, fd is " << sock_fd << endl;
}