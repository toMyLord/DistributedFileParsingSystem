//
// Created by mylord on 2019/10/31.
//

#ifndef DISTRIBUTEDFILEPARSING_SERVER_H
#define DISTRIBUTEDFILEPARSING_SERVER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <string.h>
#include <algorithm>
#include <ctime>
using namespace std;

#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE

#define MAX_BUFFER_SIZE 1024

#endif

typedef struct {
    sockaddr_in client_sock;
    int client_fd;
    string client_ip;
}ClientInfo;


class Server {
private:
    int listen_fd, listen_port, listen_size;
    sockaddr_in server_addr;
public:
    Server(int port = 5555, int size = 10);

    void Init();

    void Listen();

    int AcceptConnection(ClientInfo & client_info);

    int Write(int sock_fd, char buff[]);

    int Read(int sock_fd, char buff[]);

    int Close(int client_fd);
};


#endif //DISTRIBUTEDFILEPARSING_SERVER_H
